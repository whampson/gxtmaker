/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "errwarn.h"
#include "gxtmaker.h"

#define NUM_ERRORS 3

struct error
{
    const unsigned int id;
    const char *msg;
};

#define ERROR_SIZE (size_t) sizeof(struct error)

struct error errors_list[] = 
{
    /* Don't forget to update NUM_ERRORS when adding/removing entries! */
    /* Keep order same as err_ids enum; IDs need to be in ascending order for
       binary search (see show_error()) */

    { E_MISSING_INPUT_FILE, "no input file" },
    { E_FILE_NOT_FOUND, "file not found '%s'" },
    { E_FILE_UNREADABLE, "unable to read file '%s'" }
};

/**
 * Binary search comparator for finding an error by ID.
 */
static int compar_error(const void *key, const void *elem)
{
    struct error *e = (struct error *) elem;
    unsigned int *id = (unsigned int *) key;

    return *id - e->id;
}

/**
 * Prints the specified error message to the console.
 *
 * @param e_id      the ID of the error message to show
 * @param file_name the name of the file in which the error occurred
 *                  (use NULL for no file)
 * @param line_num  the line number on which the error occurred
 *                  (use non-positive number for no line)
 * @param col_num   the column number on which the error occurred
 *                  (use non-positive number for no column)
 * @param msg_args  a variable argument list containing arguments for any
 *                  special formatting in the printed message
 *
 * @return true if the error specified by e_id exists and was displayed,
 *         false otherwise
 */
static bool show_error(int e_id, const char *file_name,
                       int line_num, int col_num, va_list msg_args)
{
    /* Find error by ID. */
    struct error *e = (struct error *)
        bsearch(&e_id, errors_list, NUM_ERRORS, ERROR_SIZE, compar_error);

    if (e == NULL)
    {
        return false;
    }

    /* Print message context (can be either the program as a whole or part of a
       file being compiled) */
    if (file_name != NULL && line_num > 0 && col_num > 0)
    {
        fprintf(stderr, "%s:%d:%d: error: ", file_name, line_num, col_num);
    }
    else
    {
        fprintf(stderr, "%s: error: ", GXTMAKER_APP_NAME);
    }

    /* Print message with formatted arguments (if applicable) */
    vfprintf(stderr, e->msg, msg_args);
    fprintf(stderr, "\n");

    return true;
}

bool error(int e_id, ...)
{
    va_list msg_args;
    va_start(msg_args, e_id);

    bool shown = show_error(e_id, NULL, -1, -1, msg_args);

    va_end(msg_args);

    return shown;
}

bool error_f(int e_id, const char *file_name, int line_num, int col_num, ...)
{
    va_list msg_args;
    va_start(msg_args, col_num);

    bool shown = show_error(e_id, file_name, line_num, col_num, msg_args);

    va_end(msg_args);

    return shown;
}
