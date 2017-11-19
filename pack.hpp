#ifndef PACK_HPP_
#define PACK_HPP_

#include <tuple>

namespace geil {

namespace detail {

struct none_t;

template<typename... TS>
struct pack{};

template <typename Pack>
struct pack_size;

template <typename... Ts>
struct pack_size<pack<Ts...>>
{
    static constexpr auto value = sizeof...(Ts);
};

template <typename Pack> struct pack_last { using type = none_t; };

template <typename T, typename... Ts>
struct pack_last<std::tuple<T, Ts...>> : pack_last<std::tuple<Ts...>> {};

template <typename T> struct pack_last<std::tuple<T>> { using type = T; };

template <typename Pack> using pack_last_t = typename pack_last<Pack>::type;

} // End of namespace detail

} // End of namespace geil

#endif // PACK_HPP_
