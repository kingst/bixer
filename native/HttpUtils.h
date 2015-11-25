#ifndef _HTTP_UTILS_H_
#define _HTTP_UTILS_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "MySocket.h"

class HttpUtils {
 public:
  static std::map<std::string, std::string> params(std::string query);
  static void writeChunk(MySocket *client, const void *buf, int numBytes);
  static void writeLastChunk(MySocket *client);

  static std::vector<std::string> split(const std::string &s, char delim);

 private:
  static std::vector<std::string> &split(const std::string &s,
					 char delim,
					 std::vector<std::string> &elems);
};

#endif
