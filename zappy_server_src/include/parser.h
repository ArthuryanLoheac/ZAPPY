/*
** EPITECH PROJECT, 2025
** parser.h
** File description:
** Header file for the parser functions
*/

#ifndef PARSER_H
    #define PARSER_H

typedef struct parser_s {
    int port;
    int width;
    int height;
    char **team_names;
    int clients_per_team;
    int freq;
} parser_t;

typedef struct parser_str_s {
    char *width;
    char *height;
    char *clients_per_team;
    char *freq;
} parser_str_t;

parser_t *parse_arguments(int ac, char **av);
parser_str_t *parse_str_arguments(int ac, char **av);
void display_help(void);
void display_error(const char *message);
void destroy_parser(parser_t *parser);
void destroy_parser_str(parser_str_t *parser);

#endif /* !PARSER_H */
