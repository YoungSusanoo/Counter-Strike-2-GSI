#include <session/session.hpp>

#include <iostream>

cs2gsi::Session::Session(asio::ip::tcp::socket&& socket):
  socket_ { std::move(socket) }
{}

void cs2gsi::Session::start()
{
  using namespace std::placeholders;
  auto binded_read = std::bind(&cs2gsi::Session::read, shared_from_this(), _1, _2);
  socket_.async_read_some(asio::buffer(buffer_), binded_read);
}

void cs2gsi::Session::read(std::error_code ec, std::size_t n)
{
  if (!ec)
  {
    if (n != 0)
    {
      auto [consumed, completed] = parser_.parse_buffer(buffer_);
      if (completed)
      {
        HttpRequest request = parser_.get_request();
        std::cout << request.body;
        parser_.clear();
      }
    }
    start();
  }
  else
  {
    parser_.clear();
  };
}