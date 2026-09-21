#include <crails/template_stream.hpp>
#include <iomanip>
#include <sstream>

#undef NDEBUG
#include <cassert>

using namespace Crails;

int main()
{
  // Basics
  {
    TemplateStream stream;
    std::string str_val = "world";

    stream << "Hello " << std::string_view("there ") << str_val << '!';
    assert(std::move(stream).extract() == "Hello there world!");
  }

  // Arithmetic types
  {
    TemplateStream stream;
    int a = 42;
    double b = 3.14;

    stream << "Int: " << a << ", Double: " << b;
    assert(std::move(stream).extract() == "Int: 42, Double: 3.14");
  }

  // std::format support
  {
    TemplateStream stream;
    int request_id = 8192;
    std::string user = "Roger";

    stream << "HTTP/1.1 200 OK\r\n"
           << stream.fmt("X-Request-ID: {:#x}\r\n", request_id)
           << stream.fmt("User: {:>8}\r\n", user);
    std::string expected = "HTTP/1.1 200 OK\r\n"
                           "X-Request-ID: 0x2000\r\n"
                           "User:    Roger\r\n";
    assert(std::move(stream).extract() == expected);
  }

  // std::ostream support
  {
    TemplateStream stream;

    stream << [](std::ostream& ss) { ss << "quoted: " << std::quoted("coucou"); };
    assert(std::move(stream).extract() == "quoted: \"coucou\"");
  }

  // defer support
  {
    TemplateStream stream;

    stream << [](TemplateStream& ss) { ss << "amount: " << ss.fmt("{} euros", 9.99); };
    assert(std::move(stream).extract() == "amount: 9.99 euros");
  }

  return 0;
}
