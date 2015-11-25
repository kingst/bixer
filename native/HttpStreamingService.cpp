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
