#include "server/include/postHandler.h"
#include "server/include/queue.h"
#include "sys/socket.h"
#include <bits/pthreadtypes.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_util.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {

  json_object *env = json_object_from_file("env.json");
  json_object *portObject;
  json_object_object_get_ex(env, "port", &portObject);

  int port = json_object_get_int(portObject);

  int i = json_object_put(env);
  while (i != 1) {
    i = json_object_put(env);
  }

  int serverFD;
  struct sockaddr_in serverAddress;

  if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) <
      0) {
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  if (listen(serverFD, 20) < 0) {
    perror("listen error");
    exit(EXIT_FAILURE);
  }

  printf("running on port %d\n", port);
  queue_t *productQueue = allocateQueue();

  pthread_mutex_init(&productQueue->queueMutex, NULL);

  while (1) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int client_fd = accept(serverFD, (struct sockaddr *)&clientAddress,
                           &clientAddressLength);

    if (client_fd < 0) {
      perror("accept error");
      continue;
    }

    pthread_data_t *arguments = (pthread_data_t*)malloc(sizeof(pthread_data_t));
    arguments->queue = productQueue;
    arguments->fd = &client_fd;


    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handlePost, (void *)arguments);
    pthread_detach(thread_id);
  }
}
