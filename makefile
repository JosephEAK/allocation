CC ?= gcc
CFLAGS += -Wall --pedantic -g 
LIB_PATH = .

all: clean lib
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o allocation $(CFLAGS) main.c -lcunit -lallocation

lib:
	$(CC) -fPIC -c allocation.c
	$(CC) -shared -o liballocation.so allocation.o -lcunit

test: clean lib
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o test_allocation $(CFLAGS) test_allocation.c -lallocation -lcunit -lm

clean:
	rm -f *.o allocation liballocation.so liballocation.a
	
.PHONY:
	test
