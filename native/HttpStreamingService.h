#ifndef HTTP_STREAMING_SERVICE_H_
#define HTTP_STREAMING_SERVICE_H_

#include <string>

#include "MySocket.h"
#include "BlockingQueue.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

class HttpStreamingService {
 public:
  HttpStreamingService(std::string contentType);
  virtual ~HttpStreamingService() {}
  virtual void stream(BlockingQueue<std::string> *streamData);

  virtual void head(HTTPRequest *request, HTTPResponse *response);
  virtual void get(HTTPRequest *request, HTTPResponse *response);

 private:
  std::string contentType;
  void getOrHead(HTTPRequest *request, HTTPResponse *response);
};

#endif
