#include <cs2gsi/server.hpp>
#include <http_parser/http_parser.hpp>
#include <iostream>

cs2gsi::Server::Server(std::string_view address, short unsigned int port):
  parser_ { new HttpParser() },
  context_ {},
  endpoint_ { asio::ip::make_address(address), port },
  socket_ { context_ },
  acceptor_ { context_, endpoint_ },
  offset_ {}
{
  accept();
}

cs2gsi::Server::~Server()
{}

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
                              if (n != 0)
                              {
                                auto [consumed, completed] = parser_->parse_buffer({ buffer_.data() + offset_, n });
                                if (completed)
                                {
                                  HttpRequest request = parser_->get_request();
                                  std::cout << request.body;
                                  parser_->clear();
                                }
                              }
                              read();
                            }
                            else
                            {
                              socket_.close();
                              accept();
                            }
                          });
}