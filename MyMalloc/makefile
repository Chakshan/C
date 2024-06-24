my_malloc.o: my_malloc.c
	gcc -g -c -fPIC -o my_malloc.o my_malloc.c

libmy_malloc.so: my_malloc.o
	gcc -shared -o libmy_malloc.so my_malloc.o

test_my_malloc.o: test_my_malloc.c
	gcc -c -o test_my_malloc.o test_my_malloc.c

test_my_malloc: test_my_malloc.o
	gcc -o test_my_malloc test_my_malloc.c 

test: test_my_malloc
	LD_PRELOAD=$$(pwd)/libmy_malloc.so ./test_my_malloc