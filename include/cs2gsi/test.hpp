#ifndef test
#define test

#include <iostream>
#include <asio.hpp>

void dsfsdfsdf()
{
  asio::io_context io;

  asio::steady_timer t(io, asio::chrono::seconds(5));
  t.wait();

  std::cout << "Hello, world!" << std::endl;
}

#endif