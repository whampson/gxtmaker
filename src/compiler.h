/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#ifndef _GXTMAKER_COMPILER_H_
#define _GXTMAKER_COMPILER_H_

enum compiler_status
{
    COMPILE_SUCCESS             = 0x00,
    COMPILE_FILE_UNREADABLE     = 0x80,
    COMPILE_GXT_KEY_TOO_LONG    = 0x81
};

/*
 * Translates the specified GXT source file into a proper GXT file.
 *
 * @param src_file the path to the source file
 * @param out_file the path to the compiled file
 *
 * @return 0 if compilation was successful, nozero if unsuccessful
 */
int compile(const char *src_file, const char *out_file);

#endif /* _GXTMAKER_COMPILER_H_ */
