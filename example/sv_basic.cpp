#include <iostream>

#include "string_view/string_view.h"

int main()
{
  abin::string_view sv("Hello, String View Example!");
  abin::string_view sv2 = "Hello, String View Example2!";
  std::string str = "String";
  std::cout  << sv.contains(str) << '\n';
  std::cout << sv.find(str) << '\n';
  std::cout << sv << '\n';
  std::cout << sv2 << '\n';
  return 0;
}