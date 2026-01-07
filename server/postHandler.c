#include "include/postHandler.h"
#include "unistd.h"
#include <alloca.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>
#include <json-c/printbuf.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void deleteProduct(Product *p) {

  if (!p)
    return;

  free(p->name);
  free(p->category);
  free(p->link);

  free(p);
}

Product *allocateProduct(int nameSize, int catSize, int linkSize) {
  Product *p = (Product *)malloc(sizeof(Product));
  if (p == NULL) {
    return NULL;
  }

  p->name = (char *)malloc(sizeof(char) * nameSize + 1);
  p->category = (char *)malloc(sizeof(char) * catSize + 1);
  p->link = (char *)malloc(sizeof(char) * linkSize + 1);

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

  Product *p =
      allocateProduct(sizeof(tempTitle), sizeof(tempCat), sizeof(tempLink));

  free(p->name);
  free(p->category);
  free(p->link);
  p->name = strdup(tempTitle);
  p->category = strdup(tempCat);
  p->link = strdup(tempLink);
  p->price = parsedPrice;

  int i = json_object_put(outerJson);
  while (i != 1) {
    i = json_object_put(outerJson);
  }

  i = json_object_put(parsedJson);
  while (i != 1) {
    i = json_object_put(parsedJson);
  }

  return p;
}

void buildHTTPResponse(char *response, size_t *responseLength) {

  snprintf(response, BUFSIZ,
           "HTTP/1.1 200 OK \r\n"
           "Content-Type: application/json\r\n"
           "\r\n"
           "{\"status\": \"ok\"}");

  *responseLength = strlen(response);

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

      printf("%s", p->category);
      printf("%s", p->link);
      printf("%s", p->name);
      printf("%f", p->price);

      deleteProduct(p);
    } else {
      printf("request is not POST");
      fflush(stdout);
    }
    regfree(&regex);
  }

  free(buffer);
  free(arg);

  char *response = (char *)malloc(BUFSIZ * 2 * sizeof(char));
  size_t res_len;

  buildHTTPResponse(response, &res_len);

  send(client_fd, response, res_len, 0);

  free(response);
  close(client_fd);

  return NULL;
}
