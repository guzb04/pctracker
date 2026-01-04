#ifndef postHandler_h
#define postHandler_h


//handles POST request from srapers, parses json and inserts it into a queue

typedef struct {
  char *name;
  float price;
  char *category;
  char *link;
}Product;

void *handlePost(void *arg);

#endif // !postHandler_h
