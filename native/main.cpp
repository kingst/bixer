#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>

#include "HttpStreamingService.h"
#include "MySocket.h"
#include "MyServerSocket.h"
#include "Process.h"

using namespace std;

static const char *testArgv[] = {"./test.sh", NULL};
int PORT = 8080;

int main(int /*argc*/, char */*argv*/[]) {
  unsigned char *buf[4096];
  int ret;

  MyServerSocket *server = new MyServerSocket(PORT);
  MySocket *client;
  Process *proc = new Process(testArgv, false, true);

  while(true) {
    client = server->accept();

    HttpStreamingService::serviceRequest(client, "video/mp2t;charset=utf-8");
    proc->run();
    int fd = proc->stdoutFd();
    char chunkHeader[256];
    while((ret = read(fd, buf, sizeof(buf))) > 0) {
      HttpStreamingService::writeChunk(client, buf, ret);
    }

    HttpStreamingService::writeLastChunk(client);
  }

}
