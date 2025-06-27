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

Test(update_map, update_map_simple, .init = redirect_all_std)
{
    parser_t parser = {0};
    cell_t **map = NULL;

    parser.width = 10;
    parser.height = 10;
    map = create_map(&parser);
    map[0][0].nbr_food = 0;
    update_map(map, &parser);
    destroy_map(map);
}
