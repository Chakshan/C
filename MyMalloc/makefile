my_malloc.o: my_malloc.c
	gcc -g -c -fPIC -o my_malloc.o my_malloc.c

libmy_malloc.dylib: my_malloc.o
	gcc -dynamiclib -o libmy_malloc.dylib my_malloc.o

test_my_malloc.o: test_my_malloc.c
	gcc -c -o test_my_malloc.o test_my_malloc.c

test_my_malloc: test_my_malloc.o libmy_malloc.dylib
	gcc -o test_my_malloc test_my_malloc.c -L. -lmy_malloc

test: test_my_malloc
	export DYLD_LIBRARY_PATH=$$(pwd):$$DYLD_LIBRARY_PATH
	./test_my_malloc
