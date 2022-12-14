CC ?= gcc
CFLAGS += -Wall --pedantic -Werror
DEBUG_FLAGS += -g
LIB_PATH = .

all: clean lib
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o allocation $(CFLAGS) $(DEBUG_FLAGS) main.c -lcunit -lallocation

lib:
	$(CC) -fPIC -c $(CFLAGS) allocation.c
	$(CC) -shared -o liballocation.so *.o
valgrind:
	make test && valgrind --leak-check=yes ./test_allocation
test: clean lib
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o test_allocation $(CFLAGS) $(DEBUG_FLAGS) test_allocation.c -lallocation -lcunit
clean:
	rm -f *.o allocation test_allocation *.so *.a
.PHONY:
	test
