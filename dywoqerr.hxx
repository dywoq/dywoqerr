#ifndef _DYWOQERR_HXX
#define _DYWOQERR_HXX

#include <cstddef>
#include <cstring>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace dywoq {

struct errnull_t final {};

class error {
private:
  const char* __reason;

public:
  constexpr error(const char* reason) : __reason(reason) {}
  [[nodiscard]] constexpr auto reason() const noexcept -> const char* { return __reason; }
  [[nodiscard]] constexpr auto operator==(const error& other) const noexcept -> bool {
    return std::string(__reason) == std::string(other.__reason);
  }
};

template <typename T>
class result_wrapper final {
private:
  T __value;

public:
  constexpr result_wrapper(const T& value) : __value(value) {}
  constexpr result_wrapper(T&& value) : __value(static_cast<T&&>(value)) {}
  [[nodiscard]] constexpr auto operator*() const noexcept -> const T& { return __value; }
  [[nodiscard]] constexpr auto get() const noexcept -> const T& { return __value; }
};

class error_wrapper final {
private:
  using __err_type = std::variant<error, errnull_t>;
  __err_type __err;

public:
  constexpr error_wrapper(const errnull_t& e) : __err(e) {}
  constexpr error_wrapper(const char* reason) : __err(error(reason)) {}

  [[nodiscard]] constexpr auto operator*() const noexcept -> const __err_type& { return __err; }
  [[nodiscard]] constexpr auto get() const noexcept -> const __err_type& { return __err; }
  [[nodiscard]] constexpr auto is_error() const noexcept -> bool { return std::holds_alternative<error>(__err); }
  [[nodiscard]] constexpr auto is_success() const noexcept -> bool {
    return std::holds_alternative<errnull_t>(__err);
  }

  [[nodiscard]] constexpr auto reason() const noexcept -> const char* {
    if (auto* p = std::get_if<error>(&__err))
      return p->reason();
    return nullptr;
  }

  [[nodiscard]] constexpr auto operator==(const error_wrapper& other) const noexcept -> bool {
    return this->reason() && other.reason() && std::strcmp(this->reason(), other.reason()) == 0;
  }
};

template <typename... Ts>
using result = std::tuple<result_wrapper<Ts>..., error_wrapper>;

template <typename... Ts>
[[nodiscard]] constexpr auto make_result(const Ts&... values) -> result<Ts...> {
  return result<Ts...>{result_wrapper<Ts>(values)..., error_wrapper(errnull_t{})};
}

template <typename... Ts>
[[nodiscard]] constexpr auto make_error(const char* reason) -> result<Ts...> {
  return result<Ts...>{result_wrapper<Ts>(Ts{})..., error_wrapper(reason)};
}

template <std::size_t N, typename... Ts>
[[nodiscard]] constexpr auto try_get(const result<Ts...>& res) -> const
    typename std::tuple_element<N, std::tuple<result_wrapper<Ts>...>>::type::value_type* {
  const auto& err = std::get<sizeof...(Ts)>(res);
  if (err.is_success())
    return &(*std::get<N>(res));
  return nullptr;
}

template <typename... Ts>
[[nodiscard]] constexpr auto is_ok(const result<Ts...>& res) -> bool {
  return std::get<sizeof...(Ts)>(res).is_success();
}

template <typename... Ts>
[[nodiscard]] constexpr auto is_err(const result<Ts...>& res) -> bool {
  return std::get<sizeof...(Ts)>(res).is_error();
}

} // namespace dywoq

#endif
