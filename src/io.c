/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#include <stdio.h>

#include "io.h"

#define NUM_BYTES_PER_LINE 16
#define NUM_CHARS_PER_BYTE 3

#define MIN(a, b) ((a < b) ? (a) : (b))

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
