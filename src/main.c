/*
 * Copyright (c) 2017 Wes Hampson <thehambone93@gmail.com>
 *
 * Licensed under the MIT License. See LICENSE at top level directory.
 */

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "errwarn.h"
#include "gxtmaker.h"

#include "list.h"

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
        error(E_MISSING_INPUT_FILE);
        return GXTMAKER_EXIT_ARGUMENT_ERROR;
    }
    else if (strcmp(argv[1], "--version") == 0)
    {
        show_version_info();
        return GXTMAKER_EXIT_SUCCESS;
    }
    else if (strcmp(argv[1], "--help") == 0)
    {
        show_help_info();
        return GXTMAKER_EXIT_SUCCESS;
    }

    //int compile_status = compile(argv[1], "./a.gxt");

    //return compile_status;

    list *l;
    list_create(&l);

    const char *my_name_is = "Slim Shady";
    for (size_t i = 0; i < strlen(my_name_is); i++)
    {
        printf("Appending %c\n", my_name_is[i]);
        list_append(l, (void *) &my_name_is[i]);
    }

    iterator *it;
    iterator_create(l, &it);

    char *c;
    while (iterator_has_next(it))
    {
        iterator_next(it, &c);
        printf("%c", *c);
        if (*c == 'a' || *c == 'S')
        {
            list_remove(l, c);
        }
    }
    printf("\n");
    iterator_destroy(&it);

    list_clear(l);

    iterator_create(l, &it);
    while (iterator_has_next(it))
    {
        iterator_next(it, &c);
        printf("%c", *c);
    }
    printf("\n");
    iterator_destroy(&it);

    printf("Size: %d\n", list_size(l));
    printf("Empty: %d\n", list_empty(l));

    list_destroy(&l);

    return 0;
}
