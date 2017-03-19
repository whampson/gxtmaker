/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdio.h>
#include <string.h>

#include "gxtas.h"

void show_help_info(void)
{
    printf("%s\n", GXTAS_HELP_MESSAGE);
}

void show_version_info(void)
{
    printf("%s - %s\n", GXTAS_APP_NAME, GXTAS_APP_MOTTO);
    printf("Version %d.%d.%d%s\n", GXTAS_VERSION_MAJOR, GXTAS_VERSION_MINOR,
                                   GXTAS_VERSION_PATCH, GXTAS_VERSION_BUILD);

    printf("\n%s\n", GXTAS_COPYRIGHT_NOTICE);
    printf("\n%s\n", GXTAS_LICENSE_NOTICE);
    printf("\n%s\n", GXTAS_WARRANTY_NOTICE);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        show_help_info();
    }
    else if (strcmp(argv[1], "--version") == 0)
    {
        show_version_info();
    }
    else
    {
        show_help_info();
    }
    return 0;
}
