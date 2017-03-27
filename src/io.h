/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#ifndef _IO_H_
#define _IO_H_

#include <stdlib.h>

/**
 * Dumps the contents of a buffer to stdout.
 *
 * @param buf  the buffer to print
 * @param size the size in bytes of the buffer
 */
void hex_dump(const void *buf, size_t size);

#endif /* _IO_H_ */
