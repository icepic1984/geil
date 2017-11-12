#ifndef DEFINE_HPP_
#define DEFINE_HPP_
#include <boost/callable_traits/args.hpp>
#include <libguile.h>
#include "value_type.hpp"


namespace geil {

namespace detail{

template <typename R, typename Fn>
auto define_function_helper_impl(Fn fn, std::tuple<R>, std::tuple<>) {
    static const Fn fn_ = fn;
    return []() -> SCM { return to_scm(fn_()); };
}

template <typename R, typename Fn, typename T1>
auto define_function_helper_impl(Fn fn, std::tuple<void>, std::tuple<T1>)
{
    static const Fn fn_ = fn;
    return [](SCM a1) -> SCM {
        fn_(to_cpp<T1>(a1));
        return SCM_UNSPECIFIED;
    };
}

template <typename Fn, typename... Args>
auto define_function_helper(Fn fn, std::tuple<Args...>) {
    return define_function_helper_impl(
        fn, std::tuple<std::result_of_t<Fn(Args...)>>{}, std::tuple<Args...>{});
}

template <typename Fn>
static void define_function(const std::string& name, Fn fn) {
    using args_t = boost::callable_traits::args_t<Fn>;
    constexpr auto args_size = std::tuple_size<args_t>::value;

    auto subr = (scm_t_subr) +define_function_helper(fn, args_t{});
    scm_c_define_gsubr(name.c_str(), args_size, 0, 0, subr);
    scm_c_export(name.c_str());
}

} // End of namespace detail

} // End of namespace geil
#endif // DEFINE_HPP_
