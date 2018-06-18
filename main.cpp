#include "BinaryValue.h"
#include <iostream>

int main() {
  systelab::db::sqlite::BinaryValue binary_value(1);

  std::cout << "Hello World! Just created an unary BinaryValue" << std::endl;
  return 0;
}
