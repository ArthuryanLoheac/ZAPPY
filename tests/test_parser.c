#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../zappy_server_src/include/zappy.h"
#include "../zappy_server_src/include/client.h"
#include "../zappy_server_src/include/command.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(parse_arguments, valid_arguments, .init = redirect_all_std)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(14, args);

    cr_assert_not_null(parser);
    cr_assert_eq(parser->port, 2121);
    cr_assert_eq(parser->width, 10);
    cr_assert_eq(parser->height, 10);
    cr_assert_str_eq(parser->team_names[0], "team1");
    cr_assert_str_eq(parser->team_names[1], "team2");
    cr_assert_eq(parser->clients_per_team, 5);
    cr_assert_eq(parser->freq, 100);
    cr_assert_eq(parser->nb_teams, 2);
    destroy_parser(parser);
}

Test(parse_arguments, valid_arguments_with_waring_log_level, .init = redirect_all_std)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "100", "-v"};
    parser_t *parser = parse_arguments(15, args);

    cr_assert_not_null(parser);
    cr_assert_eq(parser->port, 2121);
    cr_assert_eq(parser->width, 10);
    cr_assert_eq(parser->height, 10);
    cr_assert_str_eq(parser->team_names[0], "team1");
    cr_assert_str_eq(parser->team_names[1], "team2");
    cr_assert_eq(parser->clients_per_team, 5);
    cr_assert_eq(parser->freq, 100);
    cr_assert_eq(parser->nb_teams, 2);
    destroy_parser(parser);
}

Test(parse_arguments, valid_arguments_with_info_log_level, .init = redirect_all_std)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "100", "-vv"};
    parser_t *parser = parse_arguments(15, args);

    cr_assert_not_null(parser);
    cr_assert_eq(parser->port, 2121);
    cr_assert_eq(parser->width, 10);
    cr_assert_eq(parser->height, 10);
    cr_assert_str_eq(parser->team_names[0], "team1");
    cr_assert_str_eq(parser->team_names[1], "team2");
    cr_assert_eq(parser->clients_per_team, 5);
    cr_assert_eq(parser->freq, 100);
    cr_assert_eq(parser->nb_teams, 2);
    destroy_parser(parser);
}

Test(parse_arguments, valid_arguments_with_debug_log_level, .init = redirect_all_std)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "100", "-vvv"};
    parser_t *parser = parse_arguments(15, args);

    cr_assert_not_null(parser);
    cr_assert_eq(parser->port, 2121);
    cr_assert_eq(parser->width, 10);
    cr_assert_eq(parser->height, 10);
    cr_assert_str_eq(parser->team_names[0], "team1");
    cr_assert_str_eq(parser->team_names[1], "team2");
    cr_assert_eq(parser->clients_per_team, 5);
    cr_assert_eq(parser->freq, 100);
    cr_assert_eq(parser->nb_teams, 2);
    destroy_parser(parser);
}

Test(parse_arguments, missing_port, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-x", "10", "-y", "10", "-n", "team1",
                    "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(8, args);
}

Test(parse_arguments, invalid_port, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "70000", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(14, args);
}

Test(parse_arguments, missing_width, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-y", "10", "-n", "team1",
                    "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(8, args);
}

Test(parse_arguments, invalid_width, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "1000", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(10, args);
}

Test(parse_arguments, missing_height, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-n", "team1",
                    "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(8, args);
}

Test(parse_arguments, invalid_height, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "1000",
                    "-n", "team1", "team2", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(10, args);
}

Test(parse_arguments, missing_team_names, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(8, args);
}

Test(parse_arguments, invalid_team_name, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "GRAPHIC", "-c", "5", "-f", "100"};
    parser_t *parser = parse_arguments(10, args);
}

Test(parse_arguments, missing_clients_per_team, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-f", "100"};
    parser_t *parser = parse_arguments(8, args);
}

Test(parse_arguments, invalid_clients_per_team, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "0", "-f", "100"};
    parser_t *parser = parse_arguments(10, args);
}

Test(parse_arguments, invalid_frequency, .init = redirect_all_std, .exit_code = 84)
{
    char *args[] = {"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
                    "-n", "team1", "team2", "-c", "5", "-f", "0"};
    parser_t *parser = parse_arguments(10, args);
}
