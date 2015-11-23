#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>
#include <string>

#include "BlockingQueue.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HttpStreamingService.h"
#include "MySocket.h"
#include "MyServerSocket.h"
#include "Process.h"

using namespace std;

static const char *testArgv[] = {"./test.sh", NULL};
int PORT = 8080;

struct ThreadData {
  BlockingQueue<string> queue;
  bool shouldRun;
  MySocket *client;
};

void *tvCaptureThread(void *arg) {
  struct ThreadData *threadData = (struct ThreadData *) arg;
  Process *proc = new Process(testArgv, false, true);
  unsigned char buf[4*4096];
  int ret;

  proc->run();
  int fd = proc->stdoutFd();
  while (threadData->shouldRun && (ret = read(fd, buf, sizeof(buf))) > 0) {
    threadData->queue.pushBack(string((char *) buf, ret));
  }

  return NULL;
}

void *kodiThread(void *arg) {
  struct ThreadData *threadData = (struct ThreadData *) arg;
  string data;
  while (threadData->shouldRun) {
    data = threadData->queue.popFront();
    HttpStreamingService::writeChunk(threadData->client,
				     data.c_str(), data.length());
  }

  return NULL;
}

void handleGet(MySocket *client) {
  struct ThreadData *threadData = new struct ThreadData;
  threadData->shouldRun = true;
  threadData->client = client;

  pthread_t kodi, capture;
  pthread_create(&capture, NULL, tvCaptureThread, threadData);
  pthread_create(&kodi, NULL, kodiThread, threadData);

  pthread_join(kodi, NULL);
  threadData->shouldRun = false;
  pthread_join(capture, NULL);

  delete threadData;
}

void *child(void *arg) {

  MySocket *client = (MySocket *) arg;

  bool requestActive = true;
  while (requestActive) {
    HTTPRequest *request = new HTTPRequest(client, PORT);
    HTTPResponse *response = new HTTPResponse();
    response->setContentType("video/mp2t;charset=utf-8");
    
    if (!request->readRequest()) {
      cout << "could not read request" << endl;
      requestActive = false;
    } else if (request->isHead() || request->isGet()) {
      response->withStreaming(request->isHead());
      string responseString = response->response();
      if (!client->write_bytes(responseString)) {
	requestActive = false;
      }
      
      if (request->isGet()) {
	handleGet(client);
      } else {
	requestActive = false;
      }

    } else {
      cout << "unsupported method" << endl;
      return NULL;
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
