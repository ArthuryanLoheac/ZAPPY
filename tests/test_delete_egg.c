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

Test(delete_egg_team_name, delete_existing_egg, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    egg_t *egg = malloc(sizeof(egg_t));

    egg->team_name = strdup("team1");
    egg->next = NULL;
    zappy.map = malloc(sizeof(starting_map_t));
    zappy.map->eggs = egg;
    delete_egg_team_name(&zappy, "team1");
    cr_assert_null(zappy.map->eggs);
    free(zappy.map);
}

Test(delete_egg_team_name, delete_non_existing_egg, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    egg_t *egg = malloc(sizeof(egg_t));

    egg->team_name = strdup("team1");
    egg->next = NULL;
    zappy.map = malloc(sizeof(starting_map_t));
    zappy.map->eggs = egg;
    delete_egg_team_name(&zappy, "team2");
    cr_assert_not_null(zappy.map->eggs);
    cr_assert_str_eq(zappy.map->eggs->team_name, "team1");
    free(zappy.map->eggs->team_name);
    free(zappy.map->eggs);
    free(zappy.map);
}

Test(delete_egg_team_name, delete_multiple_eggs, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    egg_t *egg1 = malloc(sizeof(egg_t));
    egg_t *egg2 = malloc(sizeof(egg_t));

    egg1->team_name = strdup("team1");
    egg1->next = egg2;
    egg2->team_name = strdup("team2");
    egg2->next = NULL;
    zappy.map = malloc(sizeof(starting_map_t));
    zappy.map->eggs = egg1;
    delete_egg_team_name(&zappy, "team1");
    cr_assert_not_null(zappy.map->eggs);
    cr_assert_str_eq(zappy.map->eggs->team_name, "team2");
    free(zappy.map->eggs->team_name);
    free(zappy.map->eggs);
    free(zappy.map);
}
