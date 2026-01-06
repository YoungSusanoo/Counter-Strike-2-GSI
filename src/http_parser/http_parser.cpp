#include <http_parser/http_parser.hpp>

cs2gsi::HttpParser::HttpParser():
  parser_ { &HttpParser::parse_method },
  content_length_ {}
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

cs2gsi::HttpRequest cs2gsi::HttpParser::get_request()
{
  HttpMethod method;
  if (method_ == "GET")
  {
    method = HttpMethod::GET;
  }
  else if (method_ == "POST")
  {
    method = HttpMethod::POST;
  }
  return { method, headers_, body_ };
}

void cs2gsi::HttpParser::clear()
{
  parser_ = &HttpParser::parse_method;
  headers_.clear();
  body_.clear();
  method_.clear();
  path_.clear();
  header_name_.clear();
  header_value_.clear();
  content_length_ = 0;
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

bool cs2gsi::HttpParser::parse_path(char c)
{
  if (c == ' ' && c == '\t')
  {
    parser_ = &cs2gsi::HttpParser::skip_spaces< &cs2gsi::HttpParser::parse_version >;
    return (this->*parser_)(c);
  }
  path_.push_back(c);
  return false;
}

bool cs2gsi::HttpParser::parse_version(char c)
{
  if (c == '\r')
  {
    parser_ = &cs2gsi::HttpParser::parse_newline;
  }
  return false;
}

bool cs2gsi::HttpParser::parse_header_name(char c)
{
  if (c == '\r')
  {
    parser_ = &cs2gsi::HttpParser::parse_newline;
  }
  else if (c == ':')
  {
    parser_ = &cs2gsi::HttpParser::skip_spaces< &cs2gsi::HttpParser::parse_header_value >;
  }
  else
  {
    header_value_.push_back(c);
    return (this->*parser_)(c);
  }
  return false;
}

bool cs2gsi::HttpParser::parse_header_value(char c)
{
  if (c == '\r')
  {
    parser_ = &cs2gsi::HttpParser::parse_newline;
  }
  else
  {
    header_value_.push_back(c);
    return (this->*parser_)(c);
  }
  return false;
}

bool cs2gsi::HttpParser::parse_body(char c)
{
  if (body_.length() == content_length_)
  {
    return finalize_parse();
  }
  body_.push_back(c);
  return false;
}

bool cs2gsi::HttpParser::finalize_parse()
{
  if (headers_.empty())
  {
    return false;
  }
  if (header_name_.empty())
  {
    if (headers_.contains("Content-Length"))
    {
      content_length_ = std::stoull(headers_["Content-Length"]);
      parser_ = &cs2gsi::HttpParser::parse_body;
    }
    else
    {
      return true;
    }
  }
  else
  {
    headers_.emplace(header_name_, header_value_);
    header_name_.clear();
    header_value_.clear();
    parser_ = &cs2gsi::HttpParser::parse_header_name;
  }
  return false;
  // sanitize returning
}

template < cs2gsi::HttpParser::parse_method_t next >
bool cs2gsi::HttpParser::skip_spaces(char c)
{
  if (c != ' ' && c != '\t')
  {
    parser_ = next;
    return (this->*next)(c);
  }
  return false;
}

bool cs2gsi::HttpParser::parse_newline(char c)
{
  if (c == '\n')
  {}
  return false;
}