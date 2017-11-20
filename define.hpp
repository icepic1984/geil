#ifndef DEFINE_HPP_
#define DEFINE_HPP_
#include <boost/callable_traits/args.hpp>
#include <libguile.h>
#include "value_type.hpp"
#include "pack.hpp"


namespace geil {

namespace detail{

template <typename R, typename Fn>
auto define_function_helper_impl(Fn fn, pack<R>, pack<>) {
    static const Fn fn_ = fn;
    return []() -> SCM { return to_scm(fn_()); };
}

template <typename Fn, typename T1>
auto define_function_helper_impl(Fn fn, pack<void>, pack<T1>)
{
    static const Fn fn_ = fn;
    return [](SCM a1) -> SCM {
        fn_(to_cpp<T1>(a1));
        return SCM_UNSPECIFIED;
    };
}

template <typename Fn>
auto define_function_helper_impl(Fn fn, pack<void>, pack<>)
{
    static const Fn fn_ = fn;
    return []() -> SCM {
        fn_();
        return SCM_UNSPECIFIED;
    };
}

template <typename Fn, typename... Args>
auto define_function_helper(Fn fn, pack<Args...>) {
    return define_function_helper_impl(
        fn, pack<std::result_of_t<Fn(Args...)>>{}, pack<Args...>{});
}

template <typename Fn>
static void define_function(const std::string& name, Fn fn) {
    using args_t = boost::callable_traits::args_t<Fn,pack>;
    constexpr auto args_size = pack_size_v<args_t>;

    auto subr = (scm_t_subr) +define_function_helper(fn, args_t{});
    scm_c_define_gsubr(name.c_str(), args_size, 0, 0, subr);
    scm_c_export(name.c_str());
}

} // End of namespace detail

} // End of namespace geil
#endif // DEFINE_HPP_
