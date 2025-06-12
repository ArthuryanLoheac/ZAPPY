#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../zappy_server_src/include/zappy.h"

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

Test(create_server, create_and_destroy_server, .init = redirect_all_std)
{
    server_t *server = create_server(2121);

    cr_assert_not_null(server);
    cr_assert_eq(server->port, 2121);
    cr_assert_neq(server->server_fd, -1);
    cr_assert_eq(server->nb_fds, 1);
    cr_assert_not_null(server->fds);
    destroy_server(server);
}
