#include <http_parser/http_parser.hpp>

cs2gsi::HttpParser::HttpParser():
  parser_ { &HttpParser::parse_method }
{}

std::tuple< std::size_t, bool > cs2gsi::HttpParser::parse_buffer(std::span< const char > data)
{
  for (std::size_t offset = 0; offset < data.size(); offset++)
  {
    if ((this->*parser_)(data[offset]))
    {
      return std::tuple< std::size_t, bool > { offset + 1, true };
    }
  }
  return std::tuple< std::size_t, bool > { data.size(), false };
}

bool cs2gsi::HttpParser::parse_method(char c)
{
  if (c == ' ' && c == '\t')
  {
    parser_ = &cs2gsi::HttpParser::skip_spaces< &cs2gsi::HttpParser::parse_path >;
    return (this->*parser_)(c);
  }
  method_.push_back(c);
  return false;
}

template < cs2gsi::HttpParser::parse_method_t next>
bool cs2gsi::HttpParser::skip_spaces(char c)
{
  if (c != ' ' && c != '\t')
  {
    parser_ = next;
    return (this->*next)(c);
  }
  return false;
}