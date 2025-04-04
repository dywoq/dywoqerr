#ifndef _DYWOQERR_HXX
#define _DYWOQERR_HXX

namespace dywoq {

struct errnull_t final {};

class error final {
private:
  const char* __reason;

public:
  constexpr error(const char* __reason) : __reason(__reason) {}
  [[nodiscard]] constexpr auto reason() { return __reason; }
};

template <typename T>
class result final {
private:
};

} // namespace dywoq

#endif