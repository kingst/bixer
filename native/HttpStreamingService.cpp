#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include "HttpStreamingService.h"

using namespace std;

HttpStreamingService::HttpStreamingService(string contentType) {
  this->contentType = contentType;
}

void HttpStreamingService::stream(BlockingQueue<string> */*streamData*/) {
  // don't do anything, should override
}

void HttpStreamingService::getOrHead(HTTPRequest */*request*/,
				     HTTPResponse *response) {
  if (contentType.size() > 0) {
    response->setContentType(contentType);
  }
  response->withStreaming();
}

void HttpStreamingService::head(HTTPRequest *request, HTTPResponse *response) {
  cout << "HEAD" << endl;
  getOrHead(request, response);
}

void HttpStreamingService::get(HTTPRequest *request, HTTPResponse *response) {
  cout << "GET" << endl;
  getOrHead(request, response);
}


void HttpStreamingService::writeChunk(MySocket *client,
				      const void *buf, int numBytes) {

  char chunkHeader[256];
  snprintf(chunkHeader, sizeof(chunkHeader), "%x\r\n", numBytes);
  client->write_bytes(chunkHeader);
  if (buf != NULL && numBytes > 0) {
    client->write_bytes(buf, numBytes);
  }
  client->write_bytes("\r\n");
}

void HttpStreamingService::writeLastChunk(MySocket *client) {
  writeChunk(client, NULL, 0);
}
