/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdio.h>
#include <string.h>

#include "assembler.h"
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
        fprintf(stderr, "%s: error: no input files\n", GXTAS_APP_NAME);
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

    int asm_status = assemble(argv[1], "./a.gxt");

    return asm_status;
}
