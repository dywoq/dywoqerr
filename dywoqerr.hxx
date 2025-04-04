#ifndef _DYWOQERR_HXX
#define _DYWOQERR_HXX

#include <tuple>
#include <variant>

namespace dywoq {

struct errnull_t final {};

class error {
private:
  const char* __reason;

public:
  constexpr error(const char* reason) : __reason(reason) {}
  [[nodiscard]] constexpr auto reason() const noexcept -> const char* { return __reason; }
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
};

template <typename... Ts>
using result = std::tuple<result_wrapper<Ts>..., error_wrapper>;

} // namespace dywoq

#endif
