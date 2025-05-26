#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test (readFile , not_existing, .init=redirect_all_std)
{
    char s[] = "test\n";
    std::cout << s;
    fflush(stdout);
    cr_assert_stdout_eq_str("test\n");
}

