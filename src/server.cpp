#include <cs2gsi/server.hpp>

#include <iostream>

#include <http_parser/http_parser.hpp>
#include <connection/connection.hpp>
#include <gsiSession/gsiSession.hpp>

cs2gsi::Server::Server(std::string_view address, short unsigned int port):
  context_ {},
  endpoint_ { asio::ip::make_address(address), port },
  acceptor_ { context_, endpoint_ },
  socket_ { context_ }
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
                           std::make_shared< Connection< GsiSession > >(std::move(socket_))->read();
                         });
}