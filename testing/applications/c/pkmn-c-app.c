/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c.h>

#include <stdio.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    char strbuffer[256] = {0};
    if(pkmn_get_database_path(strbuffer, sizeof(strbuffer), NULL))
    {
        fputs("Failed to find database.", stderr);
    }

    printf("Database path: %s\n", strbuffer);

    return 0;
}
