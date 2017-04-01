/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

    list *tdat_buf;
    list *tdat;

    uint32_t tdat_offset;

    struct gxt_key *key_buf;
    list *tkey;
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
 *   1) Handle nested comments
 *   2) Trim GXT strings (leading and trailng whitespace)
 *   3) Character encoding
 *   4) Output GXT file
 *   5) Develop methods for source file error checking
 */


static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state);
static int process_key_token(unsigned char tok, struct compiler_state *state);
static int process_value_token(unsigned char tok, struct compiler_state *state);
static int process_comment_token(unsigned char tok, struct compiler_state *state);

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
    list_create(&state.tdat_buf);
    list_create(&state.tdat);
    list_create(&state.tkey);

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

    fclose(src);

    /* TODO: Ensure clean exit when compilation fails */

    printf("Read %d keys in %d chunks.\n", (int) list_size(state.tkey), chunk_count);

    /* Clear remaining chars in TDAT buffer */
    iterator *it;
    iterator_create(state.tdat_buf, &it);
    gxt_char *c;

    while (iterator_has_next(it))
    {
        iterator_next(it, (void **) &c);
        free(c);
    }
    iterator_destroy(&it);

    /* Clear TKEY buffer */
    free(state.key_buf);

    /* Build TKEY*/
    size_t tkey_size = list_size(state.tkey) * sizeof(struct gxt_key);
    printf("sizeof(TKEY) = %lu\n", tkey_size);

    struct gxt_key *tkey = (struct gxt_key *) malloc(tkey_size);
    struct gxt_key *k;

    int i = 0;
    iterator_create(state.tkey, &it);
    while (iterator_has_next(it))
    {
        iterator_next(it, (void **) &k);
        tkey[i++] = *k;
        free(k);
    }
    iterator_destroy(&it);

    /* Dump TKEY */
    //hex_dump((void *) tkey, tkey_size);
    //printf("\n\n");

    /* Build TDAT */
    size_t tdat_size = 0;
    iterator_create(state.tdat, &it);
    while (iterator_has_next(it))
    {
        iterator_next(it, (void **) &c);
        tdat_size += (gxt_strlen(c) + 1) * sizeof(gxt_char);
    }
    printf("sizeof(TDAT) = %lu\n", tdat_size);
    iterator_destroy(&it);

    gxt_char *tdat = (gxt_char *) malloc(tdat_size);
    i = 0;

    iterator_create(state.tdat, &it);
    while (iterator_has_next(it))
    {
        iterator_next(it, (void **) &c);
        for (size_t j = 0; j < gxt_strlen(c); j++)
        {
            tdat[i++] = c[j];
        }
        tdat[i++] = 0;
        free(c);
    }
    iterator_destroy(&it);

    /* Dump TDAT */
    //hex_dump((void *) tdat, tdat_size);
    //printf("\n");

    struct gxt_block_header tkey_header;
    strncpy(tkey_header.sig, "TKEY", 4);
    tkey_header.size = tkey_size;

    struct gxt_block_header tdat_header;
    strncpy(tdat_header.sig, "TDAT", 4);
    tdat_header.size = tdat_size;

    FILE *dest = fopen(out_file, "wb");
    if (dest == NULL)
    {
        error(E_FILE_UNREADABLE, out_file);
        return COMPILE_FILE_UNREADABLE;
    }

    fwrite(&tkey_header, sizeof(struct gxt_block_header), 1, dest);
    fwrite(tkey, tkey_size, 1, dest);
    fwrite(&tdat_header, sizeof(struct gxt_block_header), 1, dest);
    fwrite(tdat, tdat_size, 1, dest);

    fclose(dest);

    free(tdat);
    free(tkey);

    list_destroy(&state.tdat_buf);
    list_destroy(&state.tdat);
    list_destroy(&state.tkey);

    return result;
}

static int compile_chunk(const char *chunk, size_t chunk_size,
                         struct compiler_state *state)
{
    unsigned char tok;
    int result = 0;

    for (size_t i = 0; i < chunk_size; i++, state->src_col++)
    {
        tok = (unsigned char) chunk[i];

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
                if (state->is_reading_comment)
                {
                    break;
                }

                if (state->val_encountered)
                {
                    //printf("Done reading value.\n");

                    size_t len = list_size(state->tdat_buf);
                    gxt_char *buf = (gxt_char *) malloc((len + 1) * sizeof(gxt_char));

                    iterator *it;
                    iterator_create(state->tdat_buf, &it);

                    int pos = 0;
                    gxt_char *c;
                    while (iterator_has_next(it))
                    {
                        iterator_next(it, (void **) &c);
                        buf[pos++] = *c;
                        free(c);
                    }
                    buf[len] = 0;

                    iterator_destroy(&it);
                    list_clear(state->tdat_buf);

                    list_append(state->tdat, (void *) buf);
                    state->tdat_offset += sizeof(gxt_char); /* NUL char */

                    list_append(state->tkey, (void *) state->key_buf);

                    //free(buf);
                }

                state->key_buf = (struct gxt_key *) calloc(1, sizeof(struct gxt_key));
                state->key_buf->offset = state->tdat_offset;

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
                //state->key_encountered = true;
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

static int process_key_token(unsigned char tok, struct compiler_state *state)
{
    if (tok == END_OF_KEY && state->is_reading_key && !state->is_reading_comment)
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

    state->key_buf->name[state->current_key_chars_read] = tok;
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

static int process_value_token(unsigned char tok, struct compiler_state *state)
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

        list_append(state->tdat_buf, (void *) c);

        state->tdat_offset += sizeof(gxt_char);
    }

    return COMPILE_SUCCESS;
}

static int process_comment_token(unsigned char tok, struct compiler_state *state)
{
    if (tok == END_OF_COMMENT && state->is_reading_comment)
    {
        //printf("Done reading comment.\n");
        state->is_reading_key = false;
        state->is_reading_val = true;
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
