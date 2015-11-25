#ifndef _BLOCKING_QUEUE_H_
#define _BLOCKING_QUEUE_H_

#include <pthread.h>

#include <queue>
#include <stdexcept>

/**
 * Note: the semantics of close are a little strange in that
 * it will kill the queue even if there are still items in the
 * buffer.
 */

class BlockingQueueClosedException : public std::runtime_error {
 public:
  BlockingQueueClosedException() : std::runtime_error("queue closed") {}
};

template <class T> class BlockingQueue {
 public:

  BlockingQueue() {
    isClosed = false;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&queueEmpty, NULL);
    pthread_cond_init(&queueFull, NULL);

    maxItems = 1024;
    initialItems = 64;
  }

  void checkClosed() {
    if (isClosed) {
      pthread_mutex_unlock(&mutex);
      throw BlockingQueueClosedException();
    }
  }

  void pushBack(T item) {
    pthread_mutex_lock(&mutex);
    checkClosed();

    while (dataQueue.size() >= maxItems) {
      pthread_cond_wait(&queueFull, &mutex);
      checkClosed();
    }

    if (initialItems > 0) {
            initialItems--;
    }
    dataQueue.push_back(item);

    pthread_cond_signal(&queueEmpty);
    pthread_mutex_unlock(&mutex);
  }

  T popFront() {
    pthread_mutex_lock(&mutex);
    checkClosed();

    while ((dataQueue.size() == 0) || (initialItems > 0)) {
      pthread_cond_wait(&queueEmpty, &mutex);
      checkClosed();
    }

    T item = dataQueue.front();
    dataQueue.pop_front();

    pthread_cond_signal(&queueFull);
    pthread_mutex_unlock(&mutex);

    return item;
  }

  void close() {
    pthread_mutex_lock(&mutex);
    isClosed = true;
    pthread_cond_broadcast(&queueFull);
    pthread_cond_broadcast(&queueEmpty);
    pthread_mutex_unlock(&mutex);
  }

 private:
  std::deque<T> dataQueue;
  pthread_mutex_t mutex;
  pthread_cond_t queueEmpty;
  pthread_cond_t queueFull;
  unsigned int maxItems;
  unsigned int initialItems;
  bool isClosed;
};

#endif
