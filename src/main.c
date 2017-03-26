/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "gxtmaker.h"

void show_help_info(void)
{
    printf("%s\n", GXTMAKER_HELP_MESSAGE);
}

void show_version_info(void)
{
    printf("%s - %s\n", GXTMAKER_APP_NAME, GXTMAKER_APP_MOTTO);
    printf("Version %d.%d.%d%s\n",
           GXTMAKER_VERSION_MAJOR, GXTMAKER_VERSION_MINOR,
           GXTMAKER_VERSION_PATCH, GXTMAKER_VERSION_BUILD);

    printf("\n%s\n", GXTMAKER_COPYRIGHT_NOTICE);
    printf("\n%s\n", GXTMAKER_LICENSE_NOTICE);
    printf("\n%s\n", GXTMAKER_WARRANTY_NOTICE);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "%s: error: no input files\n", GXTMAKER_APP_NAME);
        return 1;   /* TODO: Meaningful exit codes. */
    }
    else if (strcmp(argv[1], "--version") == 0)
    {
        show_version_info();
        return 0;
    }
    else if (strcmp(argv[1], "--help") == 0)
    {
        show_help_info();
        return 0;
    }

    int compile_status = compile(argv[1], "./a.gxt");

    return compile_status;
}
