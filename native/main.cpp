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

using namespace std;

#define PIPE_WRITE 1
#define PIPE_READ  0

static const char *testArgv[] = {"./test.sh", NULL};

int createProc(const char *argv[]) {
  int pret;
  const char *file = argv[0];
  int outFd[2];
  int pid;

  pret = pipe(outFd); assert(pret == 0);

  pid = fork();
  if(pid == 0) {
    dup2(outFd[PIPE_WRITE], STDOUT_FILENO);

    execvp((char *) file, (char **) argv);
    assert(0);
  } else {
    close(outFd[PIPE_WRITE]);
    return outFd[PIPE_READ];
  }

  return -1;
}

int PORT = 8080;

int main(int /*argc*/, char */*argv*/[]) {
  unsigned char *buf[4096];
  int ret;

  MyServerSocket *server = new MyServerSocket(PORT);
  MySocket *client;

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

    int fd = createProc(testArgv);
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
