#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "Process.h"

#define PIPE_WRITE 1
#define PIPE_READ  0

Process::Process(const char **args, bool pipeStdin, bool pipeStdout) {
  this->args = args;
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

int Process::stdoutFd() {
  return outFd[PIPE_READ];
}
