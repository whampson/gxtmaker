/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "io.h"

#define NUM_BYTES_PER_LINE 16
#define NUM_CHARS_PER_BYTE 3

#define MIN(a, b) ((a < b) ? (a) : (b))

/**
 * Returns a pointer to the last non-empty token of a given string. Tokens are
 * split up by the specified delimiter character.
 */
static bool get_last_token(const char *str, char **tok, char delim);

void hex_dump(const void *buf, size_t size)
{
    const char *b = (char *) buf;

    char c;
    int i;
    int row_len;
    int pad_len;
    unsigned int off = 0;

    do
    {
        /* Print current offset in buffer */
        printf("%08x  ", off);

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

const char * get_filename(const char *path)
{
    char *tok;
    
    /* Get last piece of Unix-style path. */
    if (get_last_token(path, &tok, '/'))
    {
        return tok;
    }

    /* Get last piece of Windows-style path. */
    if (get_last_token(path, &tok, '\\'))
    {
        return tok;
    }

    /* Path separator not found, assume the path itself is the filename. */
    return path;
}

static bool get_last_token(const char *str, char **tok, char delim)
{
    *tok == NULL;

    size_t len = strlen(str);
    if (len == 0)
    {
        return false;
    }

    /* Start at end of string and move backwards until delimiter found.*/
    char *end = str + len - 1;
    char *curr = end;
    bool delim_found = false;
    while (curr != str)
    {
        /* We're looking for the last NON-EMPTY token, so we must ignore the
           empty token that results from the delimiter being located at the
           very end of the string. */
        if (*curr == delim && curr != end)
        {
            delim_found = true;
            break;
        }
        curr--;
    }

    if (delim_found)
    {
        size_t tokpos = (curr - str) + 1;
        *tok = (char *) (str + tokpos);
    }

    return delim_found;
}
