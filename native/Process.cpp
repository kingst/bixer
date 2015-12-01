#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "Process.h"
#include "HttpUtils.h"

#define PIPE_WRITE 1
#define PIPE_READ  0

using namespace std;

Process::Process(const char **args, bool pipeStdin, bool pipeStdout) {
  init(pipeStdin, pipeStdout);

  int argc = 0;
  while(args[argc] != NULL) {
    argc++;
  }
  
  this->args = new char *[argc + 1];
  this->args[argc] = NULL;
  for (int idx = 0; idx < argc; idx++) {
    this->args[idx] = new char [strlen(args[idx]) + 1];
    strcpy(this->args[idx], args[idx]);
  }
}

Process::Process(string command, bool pipeStdin, bool pipeStdout) {
  init(pipeStdin, pipeStdout);

  vector<string> stringArgs = HttpUtils::split(command, ' ');
  args = new char *[stringArgs.size() + 1];
  args[stringArgs.size()] = NULL;

  for (unsigned int idx = 0; idx < stringArgs.size(); idx++) {
    string stringArg = stringArgs[idx];
    char *arg = new char[stringArg.size()+1];
    strcpy(arg, stringArg.c_str());
    args[idx] = arg;
  }
}

Process::~Process() {
  int idx = 0;
  while (args[idx] != NULL) {
    delete [] args[idx];
    idx++;
  }

  delete [] args;
  if (inFd[PIPE_READ] >= 0) close(inFd[PIPE_READ]);
  if (inFd[PIPE_WRITE] >= 0) close(inFd[PIPE_WRITE]);
  if (outFd[PIPE_READ] >= 0) close(outFd[PIPE_READ]);
  if (outFd[PIPE_WRITE] >= 0) close(outFd[PIPE_WRITE]);

  // if the process is still running, let it run
}

void Process::init(bool pipeStdin, bool pipeStdout) {
  this->pid = -1;
  this->pipeStdin = pipeStdin;
  this->pipeStdout = pipeStdout;
  this->inFd[PIPE_READ] = -1;
  this->inFd[PIPE_WRITE] = -1;
  this->outFd[PIPE_READ] = -1;
  this->outFd[PIPE_WRITE] = -1;
}

bool Process::run() {
  int pret;
  const char *file = args[0];

  // check if the process is already running.
  if (pid >= 0) {
    return false;
  }

  if (pipeStdout) {
    pret = pipe(outFd); assert(pret == 0);
  }
  if (pipeStdin) {
    pret = pipe(inFd); assert(pret == 0);
  }

  pid = fork();
  if(pid == 0) {
    if (pipeStdout) {
      dup2(outFd[PIPE_WRITE], STDOUT_FILENO);
    }
    if (pipeStdin) {
      dup2(inFd[PIPE_READ], STDIN_FILENO);
    }

    execvp((char *) file, (char **) args);
    return false;
  } else {
    // clean up the unused portion of the pipe
    if (pipeStdout) {
      close(outFd[PIPE_WRITE]);
      outFd[PIPE_WRITE] = -1;
    }
    if (pipeStdin) {
      close(inFd[PIPE_READ]);
      inFd[PIPE_READ] = -1;
    }
    return true;
  }
}

void Process::kill() {
  if (pid < 0) {
    return;
  }

  int ret = ::kill(pid, SIGKILL);
}

void Process::wait() {
  if (pid < 0) {
    return;
  }

  int ret = waitpid(pid, NULL, 0);
}

int Process::stdoutFd() {
  return outFd[PIPE_READ];
}

void Process::runCommand(string command) {
  Process *proc = new Process(command, false, false);
  proc->run();
  proc->wait();
  delete proc;
}
