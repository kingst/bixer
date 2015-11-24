#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>
#include <string>

#include "BlockingQueue.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HttpStreamingService.h"
#include "IptvService.h"
#include "MySocket.h"
#include "MyServerSocket.h"
#include "Process.h"

using namespace std;

int PORT = 8080;

struct ThreadData {
  BlockingQueue<string> *queue;
  HttpStreamingService *service;
};

void *streamService(void *arg) {
  struct ThreadData *threadData = (struct ThreadData *) arg;
  threadData->service->stream(threadData->queue);
  return NULL;
}


void *child(void *arg) {

  MySocket *client = (MySocket *) arg;

  bool requestActive = true;
  while (requestActive) {
    HTTPRequest *request = new HTTPRequest(client, PORT);
    HTTPResponse *response = new HTTPResponse();
    IptvService *service = new IptvService();

    if (!request->readRequest()) {
      // XXX FIXME throw an exception
      break;
    } else if (request->isHead()) {
      service->head(request, response);
      client->write_bytes(response->response());
    } else if (request->isGet()) {
      service->get(request, response);
      client->write_bytes(response->response());
      
      BlockingQueue<string> *queue = new BlockingQueue<string>();
      struct ThreadData *threadData = new struct ThreadData;
      threadData->queue = queue;
      threadData->service = service;
      pthread_t tid;
      pthread_create(&tid, NULL, streamService, threadData);
      pthread_detach(tid);

      string data;
      while (true) {
	data = queue->popFront();
	HttpStreamingService::writeChunk(client, data.c_str(), data.length());
      }
    }
    
    delete response;
    delete request;
  }
  
  client->close();
  delete client;

  return NULL;
}

int main(int /*argc*/, char */*argv*/[]) {

  MyServerSocket *server = new MyServerSocket(PORT);
  MySocket *client;

  pthread_t tid;
  while(true) {
    client = server->accept();
    pthread_create(&tid, NULL, child, client);
    pthread_detach(tid);
  }
}
