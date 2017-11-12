#ifndef FOREIGN_TYPE_HPP_
#define FOREIGN_TYPE_HPP_

#include <libguile.h>
#include <utility>
#include <string>
#include <cassert>
#include "value_type.hpp"

namespace geil {

namespace detail {

template <typename T>
struct foreign_type_storage
{
    static SCM data;
};

template <typename T>
SCM foreign_type_storage<T>::data = SCM_UNSPECIFIED;

template <typename T>
void define_foreign(const std::string& name)
{
    using storage_t = detail::foreign_type_storage<T>;
    assert(storage_t::data == SCM_UNSPECIFIED);
    auto foreign_type = scm_make_foreign_object_type(
        scm_from_utf8_symbol(name.c_str()),
        scm_list_1(scm_from_utf8_symbol("data")),
        nullptr);
    scm_c_define(name.c_str(), foreign_type);
    storage_t::data = foreign_type;
    scm_c_export(name.c_str());
}

template <typename T, typename... Args>
val make_foreign(Args&& ...args)
{
    using storage_t = foreign_type_storage<T>;
    assert(storage_t::data != SCM_UNSPECIFIED &&
           "can not convert to undefined type");
    return scm_make_foreign_object_1(storage_t::data,
                                     new (scm_gc_malloc(sizeof(T), "scmpp"))
                                         T(std::forward<Args>(args)...));
}

} // End of namespace detail

}


#endif // SCM_HPP_
