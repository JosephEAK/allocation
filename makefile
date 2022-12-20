CC ?= gcc
CFLAGS += -Wall --pedantic -Werror
DEBUG_FLAGS += -g
LOCAL_LIB=/home/konan_akoubia/allocation/poei-linkedlist
LOCAL_INC=/home/konan_akoubia/allocation/poei-linkedlist

all:
	$(CC) $(LDFLAGS) -I$(LOCAL_INC) -L$(LOCAL_LIB) -o allocation $(CFLAGS) $(DEBUG_FLAGS) allocation.c main.c -llinkedlist
test:
	$(CC) $(LDFLAGS) -I$(LOCAL_INC) -L$(LOCAL_LIB) -o test_allocation $(CFLAGS) $(DEBUG_FLAGS) allocation.c tests/test_allocation.c $(CFILES) -lcunit -llinkedlist
lib:
	cd poei-linkedlist && make lib
valgrind:
	make test && valgrind --leak-check=yes ./test_allocation
clean:
	rm -f *.o allocation test_allocation *.so *.a
