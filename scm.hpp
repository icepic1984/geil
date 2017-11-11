#ifndef SCM_HPP_
#define SCM_HPP_

#include <libguile.h>

namespace geil {

namespace detail {

template <typename T>
struct convert;

#define SCM_DECLTYPE_RETURN(...)                                               \
    decltype(__VA_ARGS__) {                                                    \
        return __VA_ARGS__;                                                    \
    }

#define SCM_DECLARE_NUMERIC_TYPE(cpp_name__, scm_name__)                       \
    template <> struct convert<cpp_name__> {                                   \
        static cpp_name__ to_cpp(SCM v) {                                      \
            return scm_to_##scm_name__(v);                                     \
        }                                                                      \
        static SCM to_scm(cpp_name__ v) {                                      \
            return scm_from_##scm_name__(v);                                   \
        }                                                                      \
    };

SCM_DECLARE_NUMERIC_TYPE(float, double);
SCM_DECLARE_NUMERIC_TYPE(double, double);
SCM_DECLARE_NUMERIC_TYPE(std::int8_t, int8);
SCM_DECLARE_NUMERIC_TYPE(std::int16_t, int16);
SCM_DECLARE_NUMERIC_TYPE(std::int32_t, int32);
SCM_DECLARE_NUMERIC_TYPE(std::int64_t, int64);
SCM_DECLARE_NUMERIC_TYPE(std::uint8_t, uint8);
SCM_DECLARE_NUMERIC_TYPE(std::uint32_t, uint32);
SCM_DECLARE_NUMERIC_TYPE(std::uint64_t, uint64);

template <typename T>
auto to_scm(T&& v) -> SCM_DECLTYPE_RETURN(
    convert<std::decay_t<T>>::to_scm(std::forward<T>(v)));

template <typename T>
auto to_cpp(SCM v) -> SCM_DECLTYPE_RETURN(
    convert<std::decay_t<T>>::to_cpp(v));



struct wrapper {

    wrapper() = default;

    wrapper(SCM handle) : m_handle(handle) {
    }

    SCM get() const {
        return m_handle;
    }

    operator SCM() const {
        return m_handle;
    }

    bool operator==(wrapper other) {
        return m_handle == other.m_handle;
    }
   
    bool operator!=(wrapper other) {
        return m_handle != other.m_handle;
    }

    SCM m_handle = SCM_UNSPECIFIED;
};

} // End of namespace detail

struct val : detail::wrapper
{
    using base_t = detail::wrapper;

    template <typename T, typename = std::enable_if_t<
                              (!std::is_same<std::decay_t<T>, val>{} &&
                               !std::is_same<std::decay_t<T>, SCM>{})>>
    val(T&& x) : base_t(detail::to_scm(std::forward<T>(x))) {
    }

    template <typename T, typename = std::enable_if_t<std::is_same<
                              T, decltype(detail::to_cpp<T>(SCM{}))>{}>>
    operator T() const {
        std::cout << "T()" << std::endl;
        return detail::to_cpp<T>(m_handle);
    }
};

} // End of namespace geil
#endif // SCM_HPP_
