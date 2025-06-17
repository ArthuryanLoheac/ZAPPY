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

static parser_str_t *init_parser_str(void)
{
    parser_str_t *parser = malloc(sizeof(parser_str_t));

    if (!parser)
        display_error("Failed to allocate memory for parser");
    parser->width = NULL;
    parser->height = NULL;
    parser->clients_per_team = NULL;
    parser->freq = NULL;
    return parser;
}

parser_str_t *parse_str_arguments(int ac, char **av)
{
    parser_str_t *parser = init_parser_str();

    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "-x") == 0 && i + 1 < ac)
            parser->width = av[i + 1];
        if (strcmp(av[i], "-y") == 0 && i + 1 < ac)
            parser->height = av[i + 1];
        if (strcmp(av[i], "-c") == 0 && i + 1 < ac)
            parser->clients_per_team = av[i + 1];
        if (strcmp(av[i], "-f") == 0 && i + 1 < ac)
            parser->freq = av[i + 1];
    }
    return parser;
}

void destroy_parser_str(parser_str_t *parser)
{
    free(parser);
}
