CC ?= gcc
CFLAGS += -Wall --pedantic -Werror
DEBUG_FLAGS += -g
LOCAL_LIB=/home/konan_akoubia/test_submodule/allocation/linked_list
LOCAL_INC=/home/konan_akoubia/test_submodule/allocation/linked_list

all:
	$(CC) $(LDFLAGS) -I$(LOCAL_INC) -o allocation $(CFLAGS) $(DEBUG_FLAGS) allocation.c main.c -llist
valgrind:
	make test && valgrind --leak-check=yes ./test_allocation
clean:
	rm -f *.o allocation test_allocation *.so *.a
