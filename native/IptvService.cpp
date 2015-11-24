#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string>

#include "IptvService.h"
#include "Process.h"

using namespace std;

static const char *testArgv[] = {"./test.sh", NULL};

void IptvService::stream(BlockingQueue<std::string> *streamData) {
  Process *proc = new Process(testArgv, false, true);
  unsigned char buf[4*4096];
  int ret;

  proc->run();
  int fd = proc->stdoutFd();
  while ((ret = read(fd, buf, sizeof(buf))) > 0) {
    streamData->pushBack(string((char *) buf, ret));
  }
}
