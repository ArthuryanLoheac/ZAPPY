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

Test(send_data, send_data_simple, .init = redirect_all_std)
{
    zappy_t zappy = {0};

    zappy.parser = parse_arguments(14, (char *[]){"./zappy_server", "-p", "2121", "-x", "10", "-y", "10",
        "-n", "team1", "team2", "-c", "5", "-f", "100"});
    zappy.parser_str = parse_str_arguments(9, (char *[]){"./zappy_server", "-x", "10", "-y", "10", "-c", "5", "-f", "100"});
    zappy.server = create_server(2121);
    zappy.map = init_starting_map(&zappy);
    add_client(&zappy, 100);
    add_client(&zappy, 101);
    send_data(&zappy, zappy.clients);
    cr_assert_not_null(zappy.clients->out_buffer);
}

Test(get_size, get_thousand_size, .init = redirect_all_std)
{
    int size = get_size(1000);

    cr_assert_eq(size, 4);
}

Test(get_size, get_hundred_size, .init = redirect_all_std)
{
    int size = get_size(100);

    cr_assert_eq(size, 3);
}

Test(get_size, get_ten_size, .init = redirect_all_std)
{
    int size = get_size(10);

    cr_assert_eq(size, 2);
}

Test(get_size, get_one_size, .init = redirect_all_std)
{
    int size = get_size(1);

    cr_assert_eq(size, 1);
}
