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

Test(send_players_data, send_players_data_simple, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    client_t client1 = {0};
    client_t client2 = {0};

    zappy.clients = &client1;
    client1.fd = 100;
    client1.is_connected = true;
    client1.is_waiting_id = false;
    client1.is_graphic = false;
    client1.stats.id = 1;
    client1.stats.x = 5;
    client1.stats.y = 5;
    client1.stats.orientation = 0;
    client1.stats.level = 1;
    client1.stats.team_name = strdup("team1");
    client2.fd = 101;
    client2.is_connected = true;
    client2.is_waiting_id = false;
    client2.is_graphic = true;
    zappy.clients->next = &client2;
    send_players_data(&zappy, &client1);
    cr_assert_not_null(client1.out_buffer);
    cr_assert_str_eq(client1.out_buffer, "pnw #1 5 5 0 1 team1\n"
        "pin #1 5 5 0 0 0 0 0 0 0\n"
        "plv #1 1\n");
    free(client1.out_buffer);
    free(client1.stats.team_name);
}
