INC_DIR = 	./include
OBJS1   =	coordinator.o files.o heapsort.o quicksort.o
OBJS2 	=	coach.o
SOURCE  =	./src/coordinator.c ./src/heapsort.c ./src/quicksort.c ./src/coach.c ./src/files.c
HEADER  =	./include/coordinator.h ./include/heapsort.h ./include/quicksort.h ./include/coach.h ./include/files.h
OUT1    =	coordinator
OUT2	=	coach
CC      =	gcc
FLAGS   =       -Wall   -g      -c  -std=c99 	-I$(INC_DIR)	-D_XOPEN_SOURCE=700

all: $(OUT1) $(OUT2)

$(OUT1): $(OBJS1)
	$(CC)   -g  -lm    $(OBJS1) -o    $@

$(OUT2): $(OBJS2)
	$(CC)   -g  -lm    $(OBJS2) -o    $@

coordinator.o: ./src/coordinator.c
	$(CC)   $(FLAGS)        ./src/coordinator.c

heapsort.o: ./src/heapsort.c
	$(CC)   $(FLAGS)        ./src/heapsort.c

quicksort.o: ./src/quicksort.c
	$(CC)   $(FLAGS)        ./src/quicksort.c

coach.o: ./src/coach.c
	$(CC)   $(FLAGS)        ./src/coach.c

files.o: ./src/files.c
	$(CC)   $(FLAGS)        ./src/files.c

clean:
	rm -f $(OBJS1) $(OUT1) $(OBJS2) $(OUT2)

count:
	wc $(SOURCE) $(HEADER)

