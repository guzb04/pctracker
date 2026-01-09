#ifndef QUEUE_H
#define QUEUE_H

#include "Product.h"
#include "postHandler.h"
#include <pthread.h>

typedef struct QueueNode {
  Product *product;
  struct QueueNode *next;
  struct QueueNode *prev;
} queue_node_t;

typedef struct Queue {
  pthread_mutex_t queueMutex;
  int currentSize;
  queue_node_t *firstIn;
  queue_node_t *lastIn;
} queue_t;

typedef struct threadArg{
  int *fd;
  queue_t *queue;
} pthread_data_t;



//alocates memory for a queue
queue_t *allocateQueue();

//inserts a Product p into a Queue q
void insertProduct(Product *p, queue_t *q);

//removes a product from the top of the Queue and returns it.
//doesn't free memory for the product
Product *removeProduct(queue_t *q);

#endif // !QUEUE_H
