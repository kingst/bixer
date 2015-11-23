#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include "HttpStreamingService.h"
#include "HTTPRequest.h"

using namespace std;

bool HttpStreamingService::serviceRequest(MySocket *client,
					  string contentType) {

  HTTPRequest *request = new HTTPRequest(client, 0);
  if(!request->readRequest()) {
    cout << "did not read request" << endl;
    return false;
  } else {    
    client->write_bytes("HTTP/1.1 200 OK\r\n");
    client->write_bytes("Content-Type: " + contentType + "\r\n");
    client->write_bytes("Transfer-Encoding: chunked\r\n");
    client->write_bytes("\r\n");
  }

  return true;
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
