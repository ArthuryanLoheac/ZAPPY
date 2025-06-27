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

Test(send_eggs_data, send_eggs_data_simple, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    client_t client = {0};
    egg_t *egg = malloc(sizeof(egg_t));

    zappy.map = malloc(sizeof(starting_map_t));
    zappy.map->eggs = egg;
    egg->id = 1;
    egg->x = 5;
    egg->y = 5;
    egg->team_name = strdup("team1");
    egg->next = NULL;
    client.fd = 100;
    client.out_buffer = NULL;
    zappy.clients = &client;
    send_eggs_data(&zappy, &client);
    cr_assert_not_null(client.out_buffer);
    cr_assert_str_eq(client.out_buffer, "enw #1 #-1 5 5\n");
    free(client.out_buffer);
    free(egg->team_name);
    free(egg);
    free(zappy.map);
}
