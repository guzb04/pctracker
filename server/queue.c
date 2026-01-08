#include "include/queue.h"
#include <pthread.h>
#include <stdlib.h>


queue_t *allocateQueue(){

  queue_t *q = (queue_t*)malloc(sizeof(queue_t));
  q->currentSize = 0;
  q->queue = (queue_node_t*)malloc(sizeof(queue_node_t));
  q->queueMutex = PTHREAD_MUTEX_INITIALIZER;


  return q;

}
