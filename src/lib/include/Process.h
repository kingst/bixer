#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>

class Process {
 public:
  Process(const char **args, bool pipeStdin, bool pipeStdout);
  Process(std::string command, bool pipeStdin, bool pipeStdout);
  ~Process();
  bool run();
  int stdoutFd();
  void kill();
  void wait();

  static void runCommand(std::string command);

 private:
  void init(bool pipeStdin, bool pipeStdout);

  bool pipeStdin;
  bool pipeStdout;
  int pid;
  int inFd[2];
  int outFd[2];
  char **args;
};

#endif
