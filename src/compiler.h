/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#ifndef _GXTMAKER_COMPILER_H_
#define _GXTMAKER_COMPILER_H_

/**
 * Translates the specified GTA Text source file into a properly-formatted GXT
 * file.
 *
 * @param src_file the path to the source file
 * @param out_file the path to the compiled file
 *
 * @return 0 if compilation was successful, nozero if unsuccessful
 */
int compile(const char *src_file, const char *out_file);

#endif /* _GXTMAKER_COMPILER_H_ */
