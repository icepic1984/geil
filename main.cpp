#include <iostream>
#include <string>
#include <libguile.h>
#include "scm.hpp"

const std::string hello = "hello";

static SCM say_hello(void) {
    return scm_from_locale_string(hello.c_str());
}

struct test {

    int width;
    int height;
};

static SCM test_type;

void init_test_type(void)
{
    SCM name = scm_from_utf8_symbol("image");
    SCM slots = scm_list_1(scm_from_utf8_symbol("data"));
    scm_t_struct_finalize finalizer = nullptr;
    test_type = scm_make_foreign_object_type(name, slots, finalizer);
};

SCM make_test(SCM s_width, SCM s_height)
{
    int width = scm_to_int(s_width);
    int height = scm_to_int(s_height);

    test *t = static_cast<test*>(scm_gc_malloc(sizeof(test), "test"));

    t->width = width;
    t->height = height;

    return scm_make_foreign_object_1(test_type, t);
}

void foo(const double& a) {
    std::cout << "foo" << std::endl;
    std::cout << a << std::endl;
}

void forbar(std::string a)
{}

void bar(double a) {
    std::cout << "bar" << std::endl;
    std::cout << a << std::endl;
}


static void inner_main(void* data, int argc, char** argv)
{
    //init_test_type();
    scm_c_define_gsubr("say-hello", 0,0,0, (void**) say_hello);
    scm_c_define_gsubr("make-test", 2,0,0, (void**) make_test);
    

    geil::val b(10.0);
    foo(b);
    bar(b);

    forbar(b);
    scm_shell(argc, argv);
}

int main(int argc, char* argv[]) {

    
    scm_boot_guile(argc, argv, inner_main, 0);
    return 0;
}
