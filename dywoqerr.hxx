#ifndef _DYWOQERR_HXX
#define _DYWOQERR_HXX

namespace dywoq {

struct errnull_t final {};

class error {
private:
  const char* __reason;

public:
  constexpr error(const char* __reason) : __reason(__reason) {}
  [[nodiscard]] constexpr auto reason() { return __reason; }
};

template <typename T>
class result_wrapper final {
private:
  T __value;

public:
  constexpr result_wrapper(const T& __value) : __value(__value) {}
  [[nodiscard]] constexpr auto operator*() -> T { return __value; }
};

} // namespace dywoq

#endif