#include "include/postHandler.h"
#include "unistd.h"
#include <alloca.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>
#include <json-c/printbuf.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void deleteProduct(Product *p) {

  free(p->name);
  free(p->category);
  free(p->link);

  free(p);
  p = NULL;
}

Product *allocateProduct(int nameSize, int catSize, int linkSize) {
  Product *p = (Product *)malloc(sizeof(Product));
  if (p == NULL) {
    return NULL;
  }

  p->name = (char *)malloc(sizeof(char) * nameSize + 1);
  p->category = (char *)malloc(sizeof(char) * catSize + 1);
  p->link = (char *)malloc(sizeof(char) * linkSize + 1);

  if (p->name == NULL || p->category == NULL || p->link == NULL) {
    deleteProduct(p);
    return NULL;
  }
  return p;
}

Product *parseJsonIntoProduct(char *json) {



  json_object *outerJson = json_tokener_parse(json);
  const char *innerString = json_object_get_string(outerJson);
  json_object *parsedJson = json_tokener_parse(innerString);

  struct json_object *title, *category, *price, *link;

  printf("Full parsed JSON:\n%s\n",
         json_object_to_json_string_ext(parsedJson, JSON_C_TO_STRING_PRETTY));
  fflush(stdout);

  json_object_object_get_ex(parsedJson, "title", &title);
  json_object_object_get_ex(parsedJson, "category", &category);
  json_object_object_get_ex(parsedJson, "price", &price);
  json_object_object_get_ex(parsedJson, "link", &link);


  const char *tempTitle = json_object_get_string(title);
  const char *tempCat = json_object_get_string(category);
  const char *tempLink = json_object_get_string(link);
  double parsedPrice = json_object_get_double(price);

  printf("%s", tempTitle);
  printf("%s", tempLink);
  printf("%s", tempCat);
  fflush(stdout);

  Product *p =
      allocateProduct(sizeof(tempTitle), sizeof(tempCat), sizeof(tempLink));

  p->name = strdup(tempTitle);
  p->category = strdup(tempCat);
  p->link = strdup(tempLink);
  p->price = parsedPrice;

  free(title);
  free(category);
  free(link);
  free(price);
  free(outerJson);
  free(parsedJson);

  return p;
}

void *handlePost(void *arg) {
  int client_fd = *(int *)arg;
  char *buffer = (char *)malloc(BUFSIZ * sizeof(char));

  ssize_t bytesReceived = recv(client_fd, buffer, BUFSIZ, 0);

  if (bytesReceived > 0) {
    regex_t regex;
    regcomp(&regex, "^POST /([^ ]*) HTTP/1", REG_EXTENDED);
    regmatch_t matches[2];

    if (regexec(&regex, buffer, 2, matches, 0) == 0) {

      char clientFDString[20];
      sprintf(clientFDString, "%d", client_fd);

      // get json separated by double newlines
      char *bodyStart = strstr(buffer, "\r\n\r\n");

      Product *p = parseJsonIntoProduct(bodyStart);

      deleteProduct(p);
    }
    printf("request is not POST");
  }
  printf("no bytes received");

  return NULL;
}
