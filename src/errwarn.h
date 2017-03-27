/*
* Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
*
* Licensed under the MIT License. See LICENSE at top level directory.
*/

#ifndef _GXTMAKER_ERRWARN_H_
#define _GXTMAKER_ERRWARN_H_

#include <stdbool.h>

enum error_ids
{
    E_MISSING_INPUT_FILE,
    E_FILE_NOT_FOUND,       /* Requires 1 string argument */
    E_FILE_UNREADABLE       /* Requires 1 string argument */
};

/*enum warn_ids
{

};*/

/**
 * Prints an error message to the standard error stream in the context of the
 * entire program.
 *
 * @param e_id the ID of the error to be printed (see err_ids enum)
 * @param ...  error message format arguments (if applicable)
 */
bool error(int e_id, ...);

/**
 * Prints an error message to the standard error stream in the context of a
 * specific file being processed.
 *
 * @param e_id      the ID of the error to be printed (see err_ids enum)
 * @param file_name the name of the file in which the error occurred
 * @param line_num  the line number on which the error occurred
 * @param col_num   the column number on which the error occurred
 * @param ...       error message format arguments (if applicable)
 */
bool error_f(int e_id, const char *file_name, int line_num, int col_num, ...);

#endif /* _GXTMAKER_ERRWARN_H_ */
