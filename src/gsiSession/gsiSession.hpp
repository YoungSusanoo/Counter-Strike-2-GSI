#ifndef GSISESSION_HPP
#define GSISESSION_HPP

#include <span>

#include <http_parser/http_parser.hpp>

namespace cs2gsi
{
  class GsiSession
  {
  public: 
    GsiSession() = default;

    void read(std::span< char > buffer);
  private:
    HttpParser parser_;
  };
}

#endif