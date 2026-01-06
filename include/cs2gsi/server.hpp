#ifndef CS2_GSI_SERVER
#define CS2_GSI_SERVER

#include <asio.hpp>
#include <vector>

namespace cs2gsi
{
  class HttpParser;
  struct Server
  {
    Server(std::string_view address, short unsigned int port);
    ~Server();
    void start();
    void stop();

  private:
    std::unique_ptr< HttpParser > parser_;

    asio::io_context context_;
    asio::ip::tcp::endpoint endpoint_;
    asio::ip::tcp::socket socket_;
    asio::ip::tcp::acceptor acceptor_;
    std::array<char, 8192> buffer_;

    std::size_t offset_;

    void accept();
    void read();
  };
}

#endif