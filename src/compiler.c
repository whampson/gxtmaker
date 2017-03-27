/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"

#define CHUNK_SIZE 1024

#define NUM_BYTES_PER_LINE 16
#define NUM_CHARS_PER_BYTE 3

#define GXT_KEY_MAX_LEN 8

#define MIN(a, b) ((a < b) ? (a) : (b))

struct compiler_state
{
    unsigned int src_row;
    unsigned int src_col;

    bool is_reading_key;
    bool is_reading_val;
    bool key_encountered;
    bool val_encountered;

    int current_key_chars_read;
    int num_keys;
};

static void hex_dump(const void *buf, size_t size);
static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state);
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
        //hex_dump(buf, bytes_read);
        //printf("\n");

        result = compile_chunk(buf, bytes_read, &state);
        if (result)
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
    char c;
    size_t i;
    for (i = 0; i < chunk_size; i++, state->src_col++)
    {
        c = chunk[i];
        if (c == '\r')
        {
            /* Skip those pesky Windows carriage returns. */
            continue;
        }
        if (c == '\n')
        {
            state->src_row++;
            state->src_col = 0;
            continue;
        }

        if (state->is_reading_key)
        {
            if (c == ']')
            {
                printf("Done reading key.\n");
                state->is_reading_key = false;
                state->is_reading_val = true;
                state->key_encountered = false;
                state->val_encountered = false;
                state->num_keys++;
                continue;
            }

            state->current_key_chars_read++;
            if (state->current_key_chars_read >= GXT_KEY_MAX_LEN)
            {
                fprintf(stderr, "gxt key exceeds maximum length (%02d:%02d)\n",
                        state->src_row, state->src_col);

                return 1;   /* TODO: Meaningful exit codes. */
            }

            printf("K(%02d:%02d) = %c\n", state->src_row, state->src_col, c);
        }
        else if (c == '[')
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

        if (state->is_reading_val)
        {
            if (!state->val_encountered && !is_whitespace(c))
            {
                state->val_encountered = true;
            }
            
            if (state->val_encountered)
            {
                printf("V(%02d:%02d) = %c\n",
                       state->src_row, state->src_col, c);
            }
        }
    }

    return 0;
}

bool is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}

/**
 * Dumps the contents of a buffer to stdout.
 */
static void hex_dump(const void *buf, size_t size)
{
    const char *b = (char *) buf;

    char c;
    int i;
    int row_len;
    int pad_len;
    int off = 0;

    do
    {
        /* Print current offset in buffer */
        printf("%04x  ", off);

        /* Print a line of bytes in hexadecimal */
        row_len = MIN(NUM_BYTES_PER_LINE, size - off);
        for (i = 0; i < row_len; i++)
        {
            printf("%02x", b[off + i] & 0xFF);
            if (i != row_len - 1)
            {
                printf(" ");
            }
        }

        /* Print padding spaces */
        pad_len = (NUM_BYTES_PER_LINE - row_len) * NUM_CHARS_PER_BYTE;
        for (i = 0; i < pad_len; i++)
        {
            printf(" ");
        }
        printf("  ");

        /* Print bytes in ASCII representation */
        /* Replace non-printing characters with '.' */
        for (i = 0; i < row_len; i++)
        {
            c = b[off + i];
            if (c < 0x20 || c > 0x7F)
            {
                printf(".");
            }
            else
            {
                printf("%c", c);
            }
        }

        /* Increase offset and print newline */
        off += row_len;
        if (off < size)
        {
            printf("\n");
        }
    }
    while (off < size);
}
