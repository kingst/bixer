#include <string>

#include "MySocket.h"

/*
class MockSocket : public MySocket {
 public:
  MOCK_METHOD1(write, void(std::string data));
};
*/

class FakeSocket : public MySocket {
 public:
  std::string output;
  virtual void write(std::string data) {
    output += data;
  }
};
