#ifndef UGLY_EXCEPTION_H
#define UGLY_EXCEPTION_H

#include <iostream>
#include <stdexcept>
#include <string>

namespace gl {

class exception : public std::exception {
  public:
    ~exception() {}
    exception(std::string what): _what(what) {}
    exception(const char* what): _what(what) {}

    template<typename... T>
    exception(const char* fmt, T const&... args) {
      std::stringstream ss;
      format(ss, fmt, args...);
      _what = ss.str();
    }

  public:
    const char* what() const noexcept override { return _what.c_str(); }

  protected:
    void format(std::ostream&, const char* fmt);

    template<typename T, typename... Args>
    void format(std::ostream&, const char* fmt, T const&, Args const&... args);

  protected:
    std::string _what;

};

inline void exception::format(std::ostream& os, const char* fmt) {
  os << fmt;
}

template<typename T, typename... Args>
inline void exception::format(std::ostream& os, const char* fmt, T const& value, Args const&... args) {
  while (*fmt) {  
    if (*fmt == '%') {
      if (fmt[1] == '%') {
        ++fmt;
      } else {
        // TODO: %x, %02d, etc.
        os << value;
        fmt += 2;
        format(os, fmt, args...);
        return;
      }
    }
    os << *fmt;
    ++fmt;
  }
}

} // namespace gl

#endif

