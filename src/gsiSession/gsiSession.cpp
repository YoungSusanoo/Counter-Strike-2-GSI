#include <gsiSession/gsiSession.hpp>

#include <iostream>

void cs2gsi::GsiSession::read(std::span< char > buffer)
{
  auto [consumed, completed] = parser_.parse_buffer(buffer);
  if (completed)
  {
    HttpRequest request = parser_.get_request();
    std::cout << request.body;
    parser_.clear();
  }
}