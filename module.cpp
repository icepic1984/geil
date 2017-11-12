#include "scm.hpp"
#include <iostream>

struct test
{
    int width;
    int height;
};

double empty() {
    return 10.0;
}

void foobar(double x) {
    std::cout << x << std::endl;
}

extern "C"
void init_module()

{
    geil::detail::define_foreign<test>("test_bla");
    geil::detail::make_foreign<test>();
    geil::detail::make_foreign<test>();
    geil::detail::define_function("empty-test", empty);
    //geil::detail::define_function("foobar", foobar);
    
}
