/*
** EPITECH PROJECT, 2025
** parser.c
** File description:
** Functions to parse command line arguments
*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "include/parser.h"
#include "logs.h"

void destroy_parser(parser_t *parser)
{
    free(parser->team_names);
    free(parser);
}
