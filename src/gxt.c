/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include "gxt.h"

size_t gxt_strlen(const gxt_char *str)
{
    size_t len = 0;

    while (*str != 0)
    {
        len++;
        str++;
    }

    return len;
}
