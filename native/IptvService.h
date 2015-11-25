#ifndef _IPTVSERVICE_H_
#define _IPTVSERVICE_H_

#include "HttpStreamingService.h"
#include "Process.h"

class IptvService : public HttpStreamingService {
 public:
  IptvService();
  virtual ~IptvService();

  virtual void stream(BlockingQueue<std::string> *streamData);

 private:
  Process *proc;

};

#endif
