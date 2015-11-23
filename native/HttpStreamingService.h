#ifndef HTTP_STREAMING_SERVICE_H_
#define HTTP_STREAMING_SERVICE_H_

#include <string>

#include "MySocket.h"

class HttpStreamingService {
 public:
  static bool serviceRequest(MySocket *client, std::string contentType);
  static void writeChunk(MySocket *client, const void *buf, int numBytes);
  static void writeLastChunk(MySocket *client);
};

#endif
