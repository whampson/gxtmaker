/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#ifndef _GXTMAKER_IO_H_
#define _GXTMAKER_IO_H_

#include <stdlib.h>

/**
 * Dumps the contents of a buffer to stdout byte-by-byte.
 *
 * @param buf  the buffer to print
 * @param size the size in bytes of the buffer
 */
void hex_dump(const void *buf, size_t size);

/**
 * Extracts the file name from the specified path.
 */
const char * get_filename(const char *path);

#endif /* _GXTMAKER_IO_H_ */
