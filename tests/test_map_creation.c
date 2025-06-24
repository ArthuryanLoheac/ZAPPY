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

Test(init_starting_map, init_starting_map_simple, .init = redirect_all_std)
{
    parser_t parser = {0};
    starting_map_t *map = NULL;

    parser.width = 10;
    parser.height = 10;
    parser.nb_teams = 2;
    parser.clients_per_team = 5;
    parser.team_names = malloc(sizeof(char *) * 2);
    parser.team_names[0] = strdup("team1");
    parser.team_names[1] = strdup("team2");
    map = init_starting_map(&(zappy_t){.parser = &parser});
    cr_assert_not_null(map);
    free_starting_map(map);
}

Test(free_starting_map, free_null_map, .init = redirect_all_std)
{
    starting_map_t *map = NULL;

    free_starting_map(map);
    cr_assert_null(map);
}
