#include <iostream>
#include <utility>
#include <cstdint>


int main() {
  int16_t i = 10;
  long j = 12;
  float x = 10.f;

  bool k = std::cmp_greater(i, j);

  std::cout << k << std::endl;

  return 0;
}