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

Test(append_client_out_buffer, append_and_check, .init = redirect_all_std)
{
    client_t client = {0};

    client.out_buffer = NULL;
    append_client_out_buffer(&client, "hello %d", 42);
    cr_assert_not_null(client.out_buffer);
    free(client.out_buffer);
}

Test(send_client_command, send_command_to_client, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    client_t client = {0};
    int fake_fd = 100;

    zappy.clients = &client;
    client.fd = fake_fd;
    client.out_buffer = NULL;
    append_client_out_buffer(&client, "hello %d", 42);
    send_client_command(&zappy, fake_fd);
    cr_assert_not_null(client.out_buffer);
    cr_assert_str_eq(client.out_buffer, "hello 42");
    free(client.out_buffer);
}

Test(send_client_command, send_command_to_nonexistent_client, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    int fake_fd = 100;

    zappy.clients = NULL;
    send_client_command(&zappy, fake_fd);
    cr_assert_null(zappy.clients);
}
