
#ifndef Product_h
#define Product_h

typedef struct {
  char *name;
  float price;
  char *category;
  char *link;
} Product;

//reserves memory for a product with a given size for each char* property
Product *allocateProduct(int nameSize, int catSize, int linkSize);

//frees memory of a given product
void deleteProduct(Product *p);

#endif // !PRODUCT_H
