/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"
#include "io.h"

#define CHUNK_SIZE 1024
#define GXT_KEY_MAX_LEN 8

#define START_OF_KEY        (char) '['
#define END_OF_KEY          (char) ']'
#define START_OF_COMMENT    (char) '{'
#define END_OF_COMMENT      (char) '}'
#define TOKEN_SPECIFIER     (char) '~'

struct compiler_state
{
    unsigned int src_row;   /* Spurce file line number. */
    unsigned int src_col;   /* Source file column number. */

    bool is_reading_key;    /* GXT key is being read. */
    bool is_reading_val;    /* GXT string is being read. */
    bool key_encountered;   /* Start of next GXT key located. */
    bool val_encountered;   /* Start of next GXT string located. */

    int current_key_chars_read; /* Number of chars read for current key. */
    int num_keys;               /* Number of keys read in total. */
};


static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state);
static int process_key_token(char tok, struct compiler_state *state);
static int process_value_token(char tok, struct compiler_state *state);

static bool is_whitespace(char c);

int compile(const char *src_file, const char *out_file)
{
    FILE *src = fopen(src_file, "rb");
    if (src == NULL)
    {
        /* TODO: Error function. */
        fprintf(stderr, "Unable to open file.\n");
        return 1;
    }

    int bytes_read = 0;
    int chunk_count = 0;

    struct compiler_state state = { 0 };
    state.src_row = 1;
    state.src_col = 1;

    printf("Reading %s in chunks of %d bytes...\n", src_file, CHUNK_SIZE);

    char buf[CHUNK_SIZE];
    int result;
    while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), src)) > 0)
    {
        printf("===== Chunk %d ===== \n", ++chunk_count);

        result = compile_chunk(buf, bytes_read, &state);
        if (result != 0)
        {
            return result;
        }
    }

    printf("Read %d keys in %d chunks.\n", state.num_keys, chunk_count);

    return 0;   /* TODO: Meaningful exit codes. */
}

static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state)
{
    char tok;
    int sub_result = 0;

    for (size_t i = 0; i < chunk_size; i++, state->src_col++)
    {
        tok = chunk[i];

        /* Handle CR and LF */
        switch (tok)
        {
            case '\n':
                state->src_row++;
                state->src_col = 0;
                /* TODO: insert space if reading GXT string? */

            case '\r':
                continue;
        }

        if (state->is_reading_key)
        {
            sub_result = process_key_token(tok, state);
        }
        else if (tok == START_OF_KEY)
        {
            if (state->val_encountered)
            {
                printf("Done reading value.\n");
            }

            state->is_reading_key = true;
            state->is_reading_val = false;
            state->key_encountered = true;
            state->current_key_chars_read = 0;
        }
        else if (state->is_reading_val)
        {
            sub_result = process_value_token(tok, state);
        }

        if (sub_result != 0)
        {
            return sub_result;
        }
    }

    return 0;
}

static int process_key_token(char tok, struct compiler_state *state)
{
    if (tok == END_OF_KEY)
    {
        printf("Done reading key.\n");
        state->is_reading_key = false;
        state->is_reading_val = true;
        state->key_encountered = false;
        state->val_encountered = false;
        state->num_keys++;

        return 0;
    }

    state->current_key_chars_read++;
    if (state->current_key_chars_read >= GXT_KEY_MAX_LEN)
    {
        fprintf(stderr, "gxt key exceeds maximum length (%02d:%02d)\n",
                state->src_row, state->src_col);

        return 1;   /* TODO: Meaningful exit codes. */
    }

    printf("K(%02d:%02d) = %c\n", state->src_row, state->src_col, tok);

    return 0;
}

static int process_value_token(char tok, struct compiler_state *state)
{
    if (!state->val_encountered && !is_whitespace(tok))
    {
        state->val_encountered = true;
    }

    if (state->val_encountered)
    {
        printf("V(%02d:%02d) = %c\n",
               state->src_row, state->src_col, tok);
    }

    return 0;
}

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}
