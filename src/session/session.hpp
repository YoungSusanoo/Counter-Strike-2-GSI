#ifndef SESSION_HPP
#define SESSION_HPP

#include <memory>

#include <asio.hpp>
#include <http_parser/http_parser.hpp>

namespace cs2gsi
{
  class Session : public std::enable_shared_from_this< Session >
  {
  public:
    Session(asio::ip::tcp::socket&& socket);

    void start();

  private:
    void read(std::error_code ec, std::size_t n);
    HttpParser parser_;
    asio::ip::tcp::socket socket_;
    std::array< char, 8192 > buffer_;
  };
}

#endif