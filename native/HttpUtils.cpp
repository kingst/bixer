#include <assert.h>

#include "HttpUtils.h"

using namespace std;

map<string, string> HttpUtils::params(string query) {
  map<string, string> paramMap;

  if (query.size() == 0) {
    return paramMap;
  }

  vector<string> pairs = split(query, '&');
  for (unsigned idx = 0; idx < pairs.size(); idx++) {
    string param = pairs[idx];
    vector<string> elements = split(param, '=');
    assert(elements.size() == 2);
    // XXX FIXME we need to url decode the strings
    paramMap[elements[0]] = elements[1];
  }

  return paramMap;
}

vector<string> &HttpUtils::split(const string &s,
		      char delim,
		      vector<string> &elems) {

  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

vector<string> HttpUtils::split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}
