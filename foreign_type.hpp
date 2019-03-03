#ifndef FOREIGN_TYPE_HPP_
#define FOREIGN_TYPE_HPP_

#include "value_type.hpp"
#include <cassert>
#include <libguile.h>
#include <string>
#include <utility>

namespace geil {

namespace detail {

template <typename T> struct foreign_type_storage { static SCM data; };

template <typename T> SCM foreign_type_storage<T>::data = SCM_UNSPECIFIED;

template <typename T> void define_foreign(const std::string& name) {
    using storage_t = detail::foreign_type_storage<T>;
    assert(storage_t::data == SCM_UNSPECIFIED);
    auto foreign_type = scm_make_foreign_object_type(
        scm_from_utf8_symbol(name.c_str()),
        scm_list_1(scm_from_utf8_symbol("data")), nullptr);
    scm_c_define(name.c_str(), foreign_type);
    storage_t::data = foreign_type;
    scm_c_export(name.c_str());
}

template <typename T, typename... Args> val make_foreign(Args&&... args) {
    using storage_t = foreign_type_storage<T>;
    assert(storage_t::data != SCM_UNSPECIFIED &&
           "can not convert to undefined type");
    return scm_make_foreign_object_1(storage_t::data,
                                     new (scm_gc_malloc(sizeof(T), "scmpp"))
                                         T(std::forward<Args>(args)...));
}

template <typename T, typename... Args> T& get_foreign(val v) {
    using storage_t = foreign_type_storage<T>;
    assert(storage_t::data != SCM_UNSPECIFIED &&
           "can not convert to undefined type");
    scm_assert_foreign_object_type(storage_t::data, v);
    return *(T*)scm_foreign_object_ref(v, 0);
}

template <typename T> struct convert_foreign_type {
    template <typename U> static SCM to_scm(U&& v) {
        return make_foreign<T>(std::forward<U>(v));
    }
    static T& to_cpp(SCM v) {
        return get_foreign<T>(v);
    }
};

// Assume that every other type is foreign
template <typename T>
struct convert<T, std::enable_if_t<!std::is_fundamental<T>::value &&
                                   // only value types are supported at
                                   // the moment but the story might
                                   // change later...
                                   !std::is_pointer<T>::value>>
    : convert_foreign_type<T> {};
} // End of namespace detail

} // namespace geil

#endif // SCM_HPP_
