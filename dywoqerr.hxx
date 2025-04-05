#ifndef _DYWOQERR_HXX
#define _DYWOQERR_HXX

#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>

namespace dywoq {

struct errnull_t final {};

class error final {
private:
  std::string __reason;

public:
  constexpr error(std::string_view __reason) noexcept : __reason(__reason) {}
  constexpr error(const char* __reason) noexcept : __reason(__reason) {}
  [[nodiscard]] constexpr auto reason() const noexcept -> const std::string& { return __reason; }
};

template <typename T>
class result_wrapper final {
private:
  using __CleanT = std::decay_t<T>;
  __CleanT __obj;

public:
  template <typename U>
  constexpr result_wrapper(U&& __obj) noexcept(std::is_nothrow_constructible_v<__CleanT, U>)
      : __obj(std::forward<U>(__obj)) {}
  [[nodiscard]] constexpr auto operator*() const noexcept { return __obj; }
  [[nodiscard]] constexpr auto operator*() noexcept -> __CleanT&& { return std::move(__obj); }
};

class error_wrapper {
private:
  std::variant<error, errnull_t> __err;

public:
  constexpr error_wrapper(errnull_t __err) noexcept : __err(__err) {}
  constexpr error_wrapper(const char* __err) noexcept : __err(error{__err}) {}
  constexpr error_wrapper(error __err) noexcept : __err(std::move(__err)) {}
  [[nodiscard]] constexpr operator bool() const noexcept { return std::holds_alternative<error>(__err); }
  [[nodiscard]] constexpr bool has_error() const noexcept { return std::holds_alternative<error>(__err); }
  [[nodiscard]] constexpr auto reason() const noexcept -> const char* {
    if (has_error()) {
      auto __got_error_class = std::get<0>(__err);
      static auto __str      = __got_error_class.reason();
      return __str.c_str();
    }

    return "";
  }
};

template <typename... Ts>
using result = std::tuple<result_wrapper<Ts>..., error_wrapper>;

} // namespace dywoq

#endif