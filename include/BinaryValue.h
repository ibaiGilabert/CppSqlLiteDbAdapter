#pragma once
#include "cpp-db-adapter/IBinaryValue.h"

namespace systelab {
namespace db {
namespace sqlite {

class BinaryValue : public IBinaryValue {
public:
  BinaryValue(int size);
  BinaryValue(std::istream &inputStream);
  virtual ~BinaryValue();

  std::ostream getOutputStream() const;
  std::istream getInputStream() const;

private:
  int m_size;
  char *m_buffer;
};
}
}
}
