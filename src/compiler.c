/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"
#include "errwarn.h"
#include "gxt.h"
#include "io.h"
#include "list.h"

#define CHUNK_SIZE 1024

#define START_OF_KEY        (char) '['
#define END_OF_KEY          (char) ']'
#define START_OF_COMMENT    (char) '{'
#define END_OF_COMMENT      (char) '}'
#define TOKEN_SPECIFIER     (char) '~'

struct compiler_state
{
    unsigned int src_row;   /* Source file line number. */
    unsigned int src_col;   /* Source file column number. */

    bool is_reading_key;    /* GXT key is being read. */
    bool is_reading_val;    /* GXT string is being read. */
    bool is_reading_comment;
    bool key_encountered;   /* Start of next GXT key located. */
    bool val_encountered;   /* Start of next GXT string located. */

    int current_key_chars_read; /* Number of chars read for current key. */
    int num_keys;               /* Number of keys read in total. */

    list *gxt_str_buf;
};

/*struct gxt_tabl
{
    struct gxt_tkey *tkey;
    struct gxt_tdat *tdat;
};*/

/* Basic compilation process (ideas, GTA III format)
 *   1) Process file in chunks
 *      a) Store keys in list, w/ pointers to their corresponding values
 *         (build TKEY)
 *      b) Store values in list (build TDAT)
 *         i) Translate characters on-the-fly, i.e. convert to GTA charset
 *            a) Need to construct a table of GTA charset, conversion will be
 *               ASCII->GTA or Unicode->GTA
 *            b) LATER: pass input encoding as compiler flag if something other
 *               than UTF-8/ASCII
 *            c) Japanese??? perhaps spaceeinstein can help with this
 *        ii) Store strings in a linked list as they can be of any size
 *            (should maybe see if an upper limit is imposed by game executable and
 *            set a max size based on that)
 *   2) Sort TKEY alphabetically (LATER: introduce a compiler flag to disable
 *      this if wanted)
 *   3) Output the GXT file
 */

/* TODO (simplified)
 *   1) Process comments
 *   2) Devise a way to store keys and values
 *   3) Build key and value tables using above
 *   4) Output GXT file
 *   5) Develop methods for source file error checking
 */


static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state);
static int process_key_token(char tok, struct compiler_state *state);
static int process_value_token(char tok, struct compiler_state *state);
static int process_comment_token(char tok, struct compiler_state *state);

static bool is_whitespace(char c);

int compile(const char *src_file, const char *out_file)
{
    FILE *src = fopen(src_file, "rb");
    if (src == NULL)
    {
        error(E_FILE_UNREADABLE, src_file);
        return COMPILE_FILE_UNREADABLE;
    }

    int bytes_read = 0;
    int chunk_count = 0;

    struct compiler_state state = { 0 };
    state.src_row = 1;
    state.src_col = 1;
    list_create(&state.gxt_str_buf);

    printf("Reading %s in chunks of %d bytes...\n", src_file, CHUNK_SIZE);

    char buf[CHUNK_SIZE];
    int result;
    while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), src)) > 0)
    {
        //printf("===== Chunk %d ===== \n", chunk_count);
        chunk_count++;

        result = compile_chunk(buf, bytes_read, &state);
        if (result != COMPILE_SUCCESS)
        {
            break;
        }
    }

    if (result == COMPILE_SUCCESS)
    {
        printf("Read %d keys in %d chunks.\n", state.num_keys, chunk_count);
    }

    /* Clear remaining chars in buffer */
    iterator *it;
    iterator_create(state.gxt_str_buf, &it);
    gxt_char *c;

    while (iterator_has_next(it))
    {
        iterator_next(it, (void **) &c);
    free(c);
    }
    iterator_destroy(&it);

    list_destroy(&state.gxt_str_buf);

    fclose(src);

    return result;
}

static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state)
{
    char tok;
    int result = 0;

    for (size_t i = 0; i < chunk_size; i++, state->src_col++)
    {
        tok = chunk[i];

        /* Check whether compiler needs to switch processing mode */
        switch (tok)
        {
            /* Only act on LF characters, ignore pesky Windows CR characters */
            /* TODO: Check line-ending compliance with current input file
               encoding */
            case '\n':
                state->src_row++;
                state->src_col = 0;
                /* TODO: insert space if reading GXT string? */

            case '\r':
                continue;

            case START_OF_KEY:
                if (state->val_encountered)
                {
                    //printf("Done reading value.\n");

                    size_t len = list_size(state->gxt_str_buf) + 1;
                    //printf("%lu\n", len);
                    gxt_char *buf = (gxt_char *) malloc(len * sizeof(gxt_char));

                    iterator *it;
                    iterator_create(state->gxt_str_buf, &it);

                    int pos = 0;
                    gxt_char *c;
                    while (iterator_has_next(it))
                    {
                        iterator_next(it, (void **) &c);
                        //printf("%lc", *c);
                        buf[pos++] = *c;
                        free(c);
                    }
                    buf[len - 1] = 0;
                    //printf("\n");

                    iterator_destroy(&it);
                    list_clear(state->gxt_str_buf);

                    //for (size_t i = 0; i < gxt_strlen(buf); i++)
                    //{
                    //        printf("%c", (char) buf[i]);
                    //}
                    //printf("\n");

                    free(buf);
                }

                state->is_reading_key = true;
                state->is_reading_val = false;
                state->is_reading_comment = false;
                state->key_encountered = true;
                state->current_key_chars_read = 0;
                continue;

            case START_OF_COMMENT:
                state->is_reading_key = false;
                state->is_reading_val = false;
                state->is_reading_comment = true;
                state->key_encountered = true;
                continue;
        }

        if (state->is_reading_key)
        {
            result = process_key_token(tok, state);
        }
        else if (state->is_reading_val)
        {
            result = process_value_token(tok, state);
        }
        else if (state->is_reading_comment)
        {
            result = process_comment_token(tok, state);
        }

        if (result != 0)
        {
            break;
        }
    }

    return result;
}

static int process_key_token(char tok, struct compiler_state *state)
{
    if (tok == END_OF_KEY)
    {
        //printf("Done reading key.\n");
        state->is_reading_key = false;
        state->is_reading_val = true;
        state->is_reading_comment = false;
        state->key_encountered = false;
        state->val_encountered = false;
        state->num_keys++;

        return COMPILE_SUCCESS;
    }

    state->current_key_chars_read++;
    if (state->current_key_chars_read >= GXT_KEY_MAX_LEN)
    {
        fprintf(stderr, "gxt key exceeds maximum length (%02d:%02d)\n",
                state->src_row, state->src_col);

        return COMPILE_GXT_KEY_TOO_LONG;
    }

    //printf("K(%02d:%02d) = %c\n", state->src_row, state->src_col, tok);

    return COMPILE_SUCCESS;
}

static int process_value_token(char tok, struct compiler_state *state)
{
    if (!state->val_encountered && !is_whitespace(tok))
    {
        state->val_encountered = true;
    }

    if (state->val_encountered)
    {
        //printf("V(%02d:%02d) = %c\n",
        //       state->src_row, state->src_col, tok);

        gxt_char *c = (gxt_char *) malloc(sizeof(gxt_char));
        *c = tok;

        list_append(state->gxt_str_buf, (void *) c);
    }

    return COMPILE_SUCCESS;
}

static int process_comment_token(char tok, struct compiler_state *state)
{
    if (tok == END_OF_COMMENT)
    {
        //printf("Done reading comment.\n");
        state->is_reading_key = false;
        state->is_reading_val = false;
        state->is_reading_comment = false;
        state->key_encountered = false;
        //state->val_encountered = false;

        return COMPILE_SUCCESS;
    }
    //printf("C(%02d:%02d) = %c\n",
    //       state->src_row, state->src_col, tok);

    return COMPILE_SUCCESS;
}

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}
