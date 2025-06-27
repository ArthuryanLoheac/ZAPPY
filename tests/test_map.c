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

Test(create_map, create_and_destroy_map, .init = redirect_all_std)
{
    parser_t parser = {0};
    cell_t **map = NULL;

    parser.width = 10;
    parser.height = 10;
    map = create_map(&parser);
    cr_assert_not_null(map);
    destroy_map(map);
}

Test(destroy_map, destroy_null_map, .init = redirect_all_std)
{
    cell_t **map = NULL;

    destroy_map(map);
    cr_assert_null(map);
}
