#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <memory>

#include <asio.hpp>
#include <http_parser/http_parser.hpp>

namespace cs2gsi
{
  namespace detail
  {
    constexpr std::size_t buffer_size = 512;
  };

  template< class T >
  concept ProtocolReader = 
    requires(T a, std::span< char > buffer) 
    {
      { a.read(buffer) } -> std::same_as< void >;
    };

  template< ProtocolReader Reader>
  class Connection : public std::enable_shared_from_this< Connection < Reader > >
  {
  public:
    Connection(asio::ip::tcp::socket&& socket);

    void read();

  private:
    void on_read(std::error_code ec, std::size_t n);

    Reader reader_;
    asio::ip::tcp::socket socket_;
    std::vector< char > buffer_;
  };
}

template< cs2gsi::ProtocolReader Reader >
cs2gsi::Connection< Reader >::Connection(asio::ip::tcp::socket&& socket):
  reader_ {},
  socket_ { std::move(socket) },
  buffer_ (detail::buffer_size)
{}

template< cs2gsi::ProtocolReader Reader >
void cs2gsi::Connection< Reader >::read()
{
  using namespace std::placeholders;
  auto binded_read = std::bind(&cs2gsi::Connection< Reader >::on_read, this->shared_from_this(), _1, _2);
  socket_.async_read_some(asio::buffer(buffer_), binded_read);
}

template< cs2gsi::ProtocolReader Reader >
void cs2gsi::Connection< Reader >::on_read(std::error_code ec, std::size_t n)
{
  if (!ec)
  {
    if (n != 0)
    {
      reader_.read(buffer_);
    }
    read();
  }
  else
  {
    std::cout << ec.message();
  }
}

#endif