#ifndef _HTTP_UTILS_H_
#define _HTTP_UTILS_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>

class HttpUtils {
 public:
  static std::map<std::string, std::string> params(std::string query);

 private:
  static std::vector<std::string> &split(const std::string &s,
					 char delim,
					 std::vector<std::string> &elems);

  static std::vector<std::string> split(const std::string &s, char delim);
};

#endif
