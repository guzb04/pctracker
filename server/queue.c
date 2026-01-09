#include "include/queue.h"
#include "include/Product.h"
#include <pthread.h>
#include <stdlib.h>

queue_t *allocateQueue() {

  queue_t *q = (queue_t *)malloc(sizeof(queue_t));
  q->currentSize = 0;
  q->firstIn = NULL;
  q->lastIn = NULL;
  pthread_mutex_init(&q->queueMutex, NULL);

  return q;
}

void insertProduct(Product *p, queue_t *q) {
  if (q == NULL || p == NULL) {
    return;
  }
  queue_node_t *temp = (queue_node_t *)malloc(sizeof(queue_node_t));
  temp->product = p;

  if (q->firstIn == NULL) {
    q->firstIn = temp;
    q->lastIn = temp;
    temp->next = NULL;
    temp->prev = NULL;
    q->currentSize = 1;
    
    return;
  }

  temp->next = q->lastIn;
  temp->prev = NULL;

  q->lastIn->prev = temp;
  q->lastIn = temp;
  q->currentSize++;

  return;
}

Product *removeProduct(queue_t *q){

  if (q->firstIn == NULL) {
    return NULL;
  }
  q->currentSize--;

  Product *p = q->firstIn->product;
  queue_node_t *temp = q->firstIn;

  if (q->firstIn->prev == NULL) {
    q->firstIn = NULL;
    q->lastIn = NULL;
  }else{
    q->firstIn->prev->next =NULL;
  }

  free(temp);
  return p;
}
