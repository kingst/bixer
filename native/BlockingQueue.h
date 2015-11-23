#ifndef _BLOCKING_QUEUE_H_
#define _BLOCKING_QUEUE_H_

#include <pthread.h>

#include <queue>

template <class T> class BlockingQueue {
 public:
  
  BlockingQueue() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&queueEmpty, NULL);
    pthread_cond_init(&queueFull, NULL);
  
    maxItems = 1024;
  }

  void pushBack(T item) {
    pthread_mutex_lock(&mutex);

    while (dataQueue.size() >= maxItems) {
      pthread_cond_wait(&queueFull, &mutex);
    }

    dataQueue.push_back(item);
    
    pthread_cond_signal(&queueEmpty);
    pthread_mutex_unlock(&mutex);
  }

  T popFront() {
    pthread_mutex_lock(&mutex);

    while (dataQueue.size() == 0) {
      pthread_cond_wait(&queueEmpty, &mutex);
    }

    T item = dataQueue.front();
    dataQueue.pop_front();
  
    pthread_cond_signal(&queueFull);
    pthread_mutex_unlock(&mutex);

    return item;
  }

 private:
  std::deque<T> dataQueue;
  pthread_mutex_t mutex;
  pthread_cond_t queueEmpty;
  pthread_cond_t queueFull;
  int maxItems;
};

#endif
