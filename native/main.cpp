#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <iostream>

#include "HTTPRequest.h"
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

    HTTPRequest *request = new HTTPRequest(client, PORT);
    if(!request->readRequest()) {
      cout << "did not read request" << endl;
    } else {    
      client->write_bytes("HTTP/1.1 200 OK\r\n");
      client->write_bytes("Content-Type: video/mp2t;charset=utf-8\r\n");
      client->write_bytes("Transfer-Encoding: chunked\r\n");
      client->write_bytes("\r\n");
    }

    proc->run();
    int fd = proc->stdoutFd();
    char chunkHeader[256];
    while((ret = read(fd, buf, sizeof(buf))) > 0) {
      snprintf(chunkHeader, sizeof(chunkHeader), "%x\r\n", ret);
      client->write_bytes(chunkHeader);
      client->write_bytes(buf, ret);
      client->write_bytes("\r\n");
    }

    client->write_bytes("0\r\n\r\n");
  }

}
