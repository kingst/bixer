#include <sstream>

#include "HTTPResponse.h"

using namespace std;

HTTPResponse::HTTPResponse() {
  this->streaming = false;
  this->contentType = "text/html; charset=ISO-8859-1";
  this->headers["Server"] = "Bixer";
}

void HTTPResponse::withStreaming(bool omitChunkedHeader) {
  this->streaming = true;
  this->omitChunkedHeader = omitChunkedHeader;
}

void HTTPResponse::setHeader(string name, string value) {
  this->headers[name] = value;
}

void HTTPResponse::setBody(string data) {
  body = data;
}

void HTTPResponse::setContentType(string contentType) {
  this->contentType = contentType;
}

void HTTPResponse::setStatus(int status) {
  this->status = status;
}

string HTTPResponse::statusToString() {
  if (status == 200) {
    return "OK";
  } else {
    return "Unknown";
  }
}

string HTTPResponse::response() {
  stringstream out;
  setHeader("Content-Type", contentType);
  if (streaming) {
    if (!omitChunkedHeader) {
      setHeader("Transfer-Encoding", "chunked");
    }
  } else {
    stringstream len;
    len << body.size();
    setHeader("Content-Length", len.str());
  }

  out << "HTTP/1.1 " << status << " " << statusToString() << "\r\n";
  map<string, string>::iterator iter;
  for(iter = headers.begin(); iter != headers.end(); iter++) {
    out << iter->first << ": " << iter->second << "\r\n";
  }
  out << "\r\n";
  if (body.size() > 0 && !streaming) {
    out << body;
  }

  return out.str();
}
