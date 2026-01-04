#include "include/postHandler.h"
#include <json-c/json.h>
#include <stdlib.h>

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

void *handlePost(void *arg);
