#include <iostream>

#include <cs2gsi/server.hpp>

int main()
{
  cs2gsi::Server server("127.0.0.1", 3000);
  server.start();
}
