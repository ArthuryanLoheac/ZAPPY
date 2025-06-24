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

Test(parse_str_arguments, valid_arguments, .init = redirect_all_std)
{
    char *args[] = {"./zappy_server", "-x", "10", "-y", "10", "-c", "5", "-f", "100"};
    parser_str_t *parser = parse_str_arguments(9, args);

    cr_assert_not_null(parser);
    cr_assert_str_eq(parser->width, "10");
    cr_assert_str_eq(parser->height, "10");
    cr_assert_str_eq(parser->clients_per_team, "5");
    cr_assert_str_eq(parser->freq, "100");
    destroy_parser_str(parser);
}
