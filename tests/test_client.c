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

Test(add_client, add_and_remove_client, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    int fake_fd = 100;

    zappy.server = create_server(2121);
    add_client(&zappy, fake_fd);
    cr_assert_not_null(zappy.clients);
    cr_assert_eq(zappy.clients->fd, fake_fd);
    remove_client(&zappy, fake_fd);
    destroy_server(zappy.server);
}

Test(remove_client, remove_client_with_out_buffer, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    int fake_fd = 100;

    zappy.server = create_server(2121);
    add_client(&zappy, fake_fd);
    append_client_out_buffer(zappy.clients, "test buffer");
    remove_client(&zappy, fake_fd);
    cr_assert_null(zappy.clients);
    destroy_server(zappy.server);
}

Test(remove_client, remove_client_with_in_buffer, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    int fake_fd = 100;

    zappy.server = create_server(2121);
    add_client(&zappy, fake_fd);
    zappy.clients->in_buffer = strdup("test input");
    remove_client(&zappy, fake_fd);
    cr_assert_null(zappy.clients);
    destroy_server(zappy.server);
}

Test(remove_client, remove_nonexistent_client, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    int fake_fd = 100;

    zappy.server = create_server(2121);
    remove_client(&zappy, fake_fd);
    cr_assert_null(zappy.clients);
    destroy_server(zappy.server);
}

Test(destroy_clients, destroy_null_clients, .init = redirect_all_std)
{
    client_t *clients = NULL;

    destroy_clients(clients);
    cr_assert_null(clients);
}

Test(destroy_clients, destroy_one_clients, .init = redirect_all_std, .exit_code = 84)
{
    zappy_t zappy = {0};

    zappy.server = create_server(2121);
    add_client(&zappy, 100);
    zappy.clients->in_buffer = strdup("test input");
    zappy.clients->out_buffer = strdup("test buffer");
    destroy_clients(zappy.clients);
    cr_assert_null(zappy.clients);
    destroy_server(zappy.server);
}
