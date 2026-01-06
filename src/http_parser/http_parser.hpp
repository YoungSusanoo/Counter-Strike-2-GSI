#ifndef HTTP_PARSER
#define HTTP_PARSER

#include <span>
#include <string>
#include <tuple>
#include <unordered_map>

namespace cs2gsi
{
  enum class HttpMethod
  {
    POST,
    GET
  };

  struct HttpRequest
  {
    HttpMethod method;
    std::unordered_map< std::string, std::string > headers;
    std::string body;
  };

  struct HttpParser
  {
    HttpParser();

    std::tuple< std::size_t, bool > parse_buffer(std::span< const char > data);
    HttpRequest get_request();
    void clear();

  private:
    using parse_method_t = bool (HttpParser::*)(char);

    parse_method_t parser_;

    std::unordered_map< std::string, std::string > headers_;
    std::string body_;

    std::string method_;
    std::string path_;
    std::string header_name_;
    std::string header_value_;
    std::size_t content_length_;

    template < parse_method_t next >
    bool skip_spaces(char c);
    bool parse_newline(char c);
    bool parse_method(char c);
    bool parse_path(char c);
    bool parse_version(char c);
    bool parse_header_name(char c);
    bool parse_header_value(char c);
    bool parse_body(char c);
    bool finalize_parse();
  };
}

#endif