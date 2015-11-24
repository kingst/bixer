#ifndef _IPTVSERVICE_H_
#define _IPTVSERVICE_H_

#include "HttpStreamingService.h"

class IptvService : public HttpStreamingService {
 public:
  IptvService() : HttpStreamingService("/dev/null") {}
  
  virtual void stream(BlockingQueue<std::string> *streamData);
};

#endif
