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

parser_t *parse_arguments(int ac, char **av);
void display_help(void);
void display_error(const char *message);

#endif /* !PARSER_H */
