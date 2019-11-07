INC_DIR = 	./include
OBJS    =	coordinator.o heapsort.o quicksort.o
SOURCE  =	./src/coordinator.c ./src/heapsort.c ./src/quicksort.c
HEADER  =	./include/coordinator.h ./include/heapsort.h ./include/quicksort.h
OUT     =	mysort
CC      =	gcc
FLAGS   =       -Wall   -g      -c  -std=c99 	-I$(INC_DIR)	-D_XOPEN_SOURCE=700

$(OUT): $(OBJS)
	$(CC)   -g  -lm    $(OBJS) -o    $@		-lm

coordinator.o: ./src/coordinator.c
	$(CC)   $(FLAGS)        ./src/coordinator.c

heapsort.o: ./src/heapsort.c
	$(CC)   $(FLAGS)        ./src/heapsort.c

quicksort.o: ./src/quicksort.c
	$(CC)   $(FLAGS)        ./src/quicksort.c

clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

