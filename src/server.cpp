#include <cs2gsi/server.hpp>

#include <iostream>

cs2gsi::Server::Server(std::string_view address, short unsigned int port):
  context_ {},
  endpoint_ { asio::ip::make_address(address), port },
  socket_ { context_ },
  acceptor_ { context_, endpoint_ }
{
  accept();
}

void cs2gsi::Server::start()
{
  context_.run();
}

void cs2gsi::Server::accept()
{
  acceptor_.async_accept(socket_,
                         [this](std::error_code e)
                         {
                           read();
                         });
}

void cs2gsi::Server::read()
{
  socket_.async_read_some(asio::buffer(buffer_),
                          [this](std::error_code ec, std::size_t n)
                          {
                            if (!ec)
                            {
                              std::cout.write(buffer_.data(), n);
                              read();
                            }
                            else
                            {
                              socket_.close();
                              accept();
                            }
                          });
}