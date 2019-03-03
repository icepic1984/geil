#ifndef TYPE_HPP
#define TYPE_HPP

#include "define.hpp"
#include "foreign_type.hpp"
#include "value_type.hpp"

namespace geil {
namespace detail {
struct move_sequence {
    move_sequence() = default;
    move_sequence(const move_sequence&) = delete;
    move_sequence(move_sequence&& other) {
        other.moved_from_ = true;
    };

    bool moved_from_ = false;
};

template <typename Tag, typename T, int Seq = 0>
struct type_definer : move_sequence {
    using this_t = type_definer;
    using next_t = type_definer<Tag, T, Seq + 1>;

    std::string type_name_ = {};

    type_definer(type_definer&&) = default;

    type_definer(std::string type_name) : type_name_(std::move(type_name)) {
    }

    ~type_definer() {
        if (!moved_from_) {
            auto define_name = "<" + type_name_ + ">";
            define_foreign<T>(define_name);
        }
    }

    template <int Seq2, typename Enable = std::enable_if_t<Seq2 + 1 == Seq>>
    type_definer(type_definer<Tag, T, Seq2> r)
        : move_sequence{std::move(r)}, type_name_{std::move(r.type_name_)} {
    }

    /**
     * Define a Scheme procedure `([type-name])` that returns a Scheme
     * value holding a default constructed `T` instance.
     */
    next_t constructor() && {
        define_function(type_name_, &make_foreign<T>);
        return {std::move(*this)};
    }

    /**
     * Define a Scheme procedure `([type-name] ...)` that returns a
     * Scheme value holding the result of invoking `fn(args...)`.
     */
    template <typename Fn> next_t constructor(Fn fn) && {
        define_function(type_name_, fn);
        return {std::move(*this)};
    }

    /**
     * Define a Scheme procedure `(make-[type-name])` that returns a
     * Scheme value holding a default constructed `T` instance.
     */
    next_t maker() && {
        define_funcion("make-" + type_name_, &make_foreign<T>);
        return {std::move(*this)};
    }

    /**
     * Define a Scheme procedure `(make-[type-name] ...)` that returns
     * the result of invoking `fn(...)`.
     */
    template <typename Fn> next_t maker(Fn fn) && {
        define_function("make-" + type_name_, fn);
        return {std::move(*this)};
    }

    /**
     * Define a Scheme procedure `([type-name]-[name] ...)` that returns
     * the result of invoking `fn(...)`.
     */
    template <typename Fn> next_t define(std::string name, Fn fn) && {
        define_function(type_name_ + "-" + name, fn);
        return {std::move(*this)};
    }
};

} // namespace detail

template <typename T, typename Tag = T>
detail::type_definer<Tag, T> type(std::string type_name) {
    return {type_name};
}
} // namespace geil
#endif
