#ifndef CS2_GSI_SERVER
#define CS2_GSI_SERVER

#include <asio.hpp>

#include <array>

namespace cs2gsi
{
  struct Server
  {
    Server(std::string_view address, short unsigned int port);
    void start();
    void stop();
  private:
    asio::io_context context_;
    asio::ip::tcp::endpoint endpoint_;
    asio::ip::tcp::socket socket_;
    asio::ip::tcp::acceptor acceptor_;
    std::array<char, 8192> buffer_;

    void accept();
    void read();
  };
}

#endif