#include "scm.hpp"
#include "type.hpp"
#include <iostream>

struct test {
    int width;
    int height;

    int getHeight() {
        return height;
    }

    int getWidth() {
        return width;
    }

    void setWidth(int i) {
        width = i;
    }
    void setHeight(int i) {
        height = i;
    }
};

double empty() {
    return 10.0;
}

void foobar(double x) {
    std::cout << x << std::endl;
}

void bar() {
    std::cout << "fuck" << std::endl;

    geil::val v(10);
    geil::detail::to_scm(v);
}
extern "C" void init_module()

{
    // geil::detail::define_foreign<test>("test_bla");
    // geil::detail::make_foreign<test>();
    geil::detail::make_foreign<test>();
    // geil::detail::define_function("empty-test", empty);
    // geil::detail::define_function("foobar", foobar);
    geil::detail::define_function("get", &test::getHeight);
    geil::type<test>("testg")
        .constructor()
        .define("setWidth", &test::setWidth)
        .define("setHeight", &test::setHeight)
        .define("getWidth", &test::getWidth)
        .define("getHeight", &test::getHeight);
}
