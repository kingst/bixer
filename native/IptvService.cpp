#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "IptvService.h"

using namespace std;

static const char *testArgv[] = {"dd", "if=/dev/zero", "bs=256k", NULL};

IptvService::IptvService() : HttpStreamingService("/dev/null") {
  proc = new Process(testArgv, false, true);
}

IptvService::~IptvService() {
  proc->kill();
  proc->wait();
  delete proc;
}

void IptvService::stream(BlockingQueue<std::string> *streamData) {
  unsigned char buf[4*4096];
  int ret;

  proc->run();
  int fd = proc->stdoutFd();
  while ((ret = read(fd, buf, sizeof(buf))) > 0) {
    streamData->pushBack(string((char *) buf, ret));
  }
}