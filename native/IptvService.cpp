#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>

#include "IptvService.h"

using namespace std;

static const char *testArgv[] = {"./test.sh", NULL};
static const char *hdhomerunArgv[] = {"hdhomerun_config", "2010C9BA", "save",
				      "/tuner0", "-", NULL};

IptvService::IptvService() : HttpStreamingService("video/mp2t") {
  proc = NULL;
}

IptvService::~IptvService() {
  if (proc != NULL) {
    proc->kill();
    proc->wait();
    delete proc;
  }
}

void IptvService::get(HTTPRequest *request, HTTPResponse *response) {
  map<string, string> params = request->getParams();

  if (params.find("channel") != params.end() &&
      params.find("program") != params.end()) {
    // XXX FIXME check these two params to make sure they're numbers
    Process::runCommand("hdhomerun_config 2010C9BA set /tuner0/channel auto:" +
			params["channel"]);
    Process::runCommand("hdhomerun_config 2010C9BA set /tuner0/program " +
			params["program"]);
    proc = new Process(hdhomerunArgv, false, true);
  } else {
    proc = new Process(testArgv, false, true);
  }

  HttpStreamingService::get(request, response);
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
