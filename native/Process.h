#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>

class Process {
 public:
  Process(const char **args, bool pipeStdin, bool pipeStdout);
  bool run();
  int stdoutFd();
  void kill();
  void wait();

  static void runCommand(std::string command);

 private:
  bool pipeStdin;
  bool pipeStdout;
  int pid;
  int inFd[2];
  int outFd[2];
  const char **args;
};

#endif
