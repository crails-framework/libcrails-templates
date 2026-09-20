#pragma once
#include <string>
#include <string_view>
#include <charconv>
#include <format>
#include <iterator>

namespace Crails
{
  struct TemplateStream
  {
    std::string buffer;

    explicit TemplateStream()
    {
    }

    void reserve(size_t reserve_bytes)
    {
      buffer.reserve(reserve_bytes);
    }

    auto out_iterator() noexcept
    {
      return std::back_inserter(buffer);
    }

    TemplateStream& operator<<(std::string_view sv)
    {
      buffer.append(sv);
      return *this;
    }

    TemplateStream& operator<<(const std::string& s)
    {
      buffer.append(s);
      return *this;
    }

    TemplateStream& operator<<(const char* s)
    {
      buffer.append(s);
      return *this;
    }

    TemplateStream& operator<<(char c)
    {
      buffer.push_back(c);
      return *this;
    }

    template<typename T>
    requires (std::is_arithmetic_v<T> && !std::is_same_v<T, char>)
    TemplateStream& operator<<(T value)
    {
      std::format_to(out_iterator(), "{}", value);
      return *this;
    }

    template<typename FUNCTOR>
    requires std::is_invocable_v<FUNCTOR, TemplateStream&>
    TemplateStream& operator<<(FUNCTOR formatter)
    {
      std::forward<FUNCTOR>(formatter)(*this);
      return *this;
    }

    template<typename... ARGS>
    auto fmt(std::format_string<ARGS...> fmt_str, ARGS&&... args)
    {
      return [fmt_str, ...captured_args = std::forward<ARGS>(args)](TemplateStream& fs)
      {
        std::format_to(fs.out_iterator(), fmt_str, captured_args...);
      };
    }

    [[nodiscard]] std::string extract() &&
    {
      return std::move(buffer);
    }
  };
}

