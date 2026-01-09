obj_dir := ./obj

objects := $(obj_dir)/main.o $(obj_dir)/postHandler.o $(obj_dir)/queue.o $(obj_dir)/product.o

$(obj_dir):
	mkdir -p $@

all: main

main: $(objects)
	gcc $^ -o $@ -ljson-c

./obj/main.o: main.c | $(obj_dir)
	gcc -c $< -o $@ -I./server/include

# Compile postHandler.o
./obj/postHandler.o: server/postHandler.c | $(obj_dir)
	gcc -c $< -o $@ -I./server/include

./obj/queue.o: server/queue.c | $(obj_dir)
	gcc -c $< -o $@ -I./server/include

./obj/product.o: server/product.c | $(obj_dir)
	gcc -c $< -o $@ -I./server/include

clean:
	rm -f $(obj_dir)/*.o a.out

run: main
	./main

.PHONY: all clean run
