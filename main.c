#include "sys/socket.h"
#include <json-c/json.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT 5440

void *handlePost(void *arg);

int main(int argc, char *argv[]) {
  int serverFD;
  struct sockaddr_in serverAddress;

  if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) <
      0) {
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  if (listen(serverFD, 20) < 0) {
    perror("listen error");
    exit(EXIT_FAILURE);
  }

  while (1) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int *clientFD = malloc(sizeof(int));

    if (accept(serverFD, (struct sockaddr *)&clientAddress,
               &clientAddressLength) < 0) {
      perror("accept error");
      continue;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handlePost, (void *)clientFD);
    pthread_detach(thread_id);
  }
}


