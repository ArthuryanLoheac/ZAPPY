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

void display_help(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height "
        "-n name1 name2 ... -c clientsNb -f freq\n\n"
        "option\tdescription\n"
        "-p port\tport number\n"
        "-x width\twidth of the world\n"
        "-y height\theight of the world\n"
        "-n name1 name2 ...\tnames of the teams\n"
        "-c clientsNb\tnumber of authorized clients per team\n"
        "-f freq\treciprocal of time unit for execution of actions"
        " (0 =< freq < 200)\n"
        "-v, -vv, -vvv\tverbose level of the logs\n");
    exit(84);
}

void display_error(const char *message)
{
    LOG_FATAL("%s", message);
    exit(84);
}

static int parse_int(char *value, int min, int max)
{
    int num = atoi(value);

    if (num < min || num > max) {
        printf("Value must be between %d and %d\n", min, max);
        display_help();
    }
    return num;
}

static int count_teams(char **av, int start, int ac)
{
    int count = 0;

    for (int i = start; i < ac && av[i][0] != '-'; i++)
        count++;
    if (count == 0) {
        printf("No team names provided\n");
        display_help();
    }
    return count;
}

static char **parse_teams(char **av, int *index, int ac, int *nb_teams)
{
    char **teams = NULL;
    int start = *index + 1;
    int count = count_teams(av, start, ac);

    teams = malloc(sizeof(char *) * (count + 1));
    if (!teams)
        display_error("Failed to allocate memory for team names");
    for (int i = 0; i < count; i++) {
        if (strcmp(av[start + i], "GRAPHIC") == 0) {
            printf("The team GRAPHIC is reserved for the graphical client.\n");
            display_help();
        }
        teams[i] = av[start + i];
    }
    teams[count] = NULL;
    *index = start + count - 1;
    *nb_teams = count;
    return teams;
}

static parser_t *init_parser(void)
{
    parser_t *parser = malloc(sizeof(parser_t));

    LOG_DEBUG("Parsing arguments");
    if (!parser)
        display_error("Failed to allocate memory for parser");
    parser->port = 0;
    parser->width = 0;
    parser->height = 0;
    parser->team_names = NULL;
    parser->clients_per_team = 0;
    parser->freq = 100;
    parser->nb_teams = 0;
    return parser;
}

static void parse_one_arg(int ac, char **av, parser_t *parser, int i)
{
    if (strcmp(av[i], "-p") == 0 && i + 1 < ac)
        parser->port = parse_int(av[i + 1], 1024, 65535);
    if (strcmp(av[i], "-x") == 0 && i + 1 < ac)
        parser->width = parse_int(av[i + 1], 1, 10000);
    if (strcmp(av[i], "-y") == 0 && i + 1 < ac)
        parser->height = parse_int(av[i + 1], 1, 10000);
    if (strcmp(av[i], "-n") == 0)
        parser->team_names = parse_teams(av, &i, ac, &parser->nb_teams);
    if (strcmp(av[i], "-c") == 0 && i + 1 < ac)
        parser->clients_per_team = parse_int(av[i + 1], 1, 200);
    if (strcmp(av[i], "-f") == 0 && i + 1 < ac)
        parser->freq = parse_int(av[i + 1], 1, 200);
    if (strcmp(av[i], "-v") == 0)
        set_minimum_log_level(WARNING);
    if (strcmp(av[i], "-vv") == 0)
        set_minimum_log_level(INFO);
    if (strcmp(av[i], "-vvv") == 0)
        set_minimum_log_level(DEBUG);
}

static int check_teams_line(char **team_names, int nb_teams, int i)
{
    for (int j = i + 1; j < nb_teams; j++) {
        if (strcmp(team_names[i], team_names[j]) == 0) {
            printf("Teams must have different names.\n");
            return 1;
        }
    }
    return 0;
}

static int check_teams(char **team_names, int nb_teams)
{
    if (nb_teams < 2) {
        printf("At least two teams are required.\n");
        return 1;
    }
    for (int i = 0; i < nb_teams; i++) {
        if (check_teams_line(team_names, nb_teams, i) == 1)
            return 1;
    }
    return 0;
}

parser_t *parse_arguments(int ac, char **av)
{
    parser_t *pars = init_parser();

    for (int i = 1; i < ac; i++)
        parse_one_arg(ac, av, pars, i);
    if (pars->port == 0 || pars->width == 0 || pars->height == 0 ||
        pars->team_names == NULL || pars->clients_per_team == 0 ||
        pars->freq > 200 || check_teams(pars->team_names, pars->nb_teams))
        display_help();
    return pars;
}
