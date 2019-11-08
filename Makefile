INC_DIR = 	./include
OBJS1   =	coordinator.o files.o
OBJS2 	=	coach.o
OBJS3 	=	sorter.o
OBJS4	= 	heapsort.o
SOURCE  =	./src/coordinator.c ./src/heapsort.c ./src/quicksort.c ./src/coach.c ./src/files.c ./src/sorter.c
HEADER  =	./include/coordinator.h ./include/heapsort.h ./include/quicksort.h ./include/coach.h ./include/files.h ./include/sorter.h
OUT1    =	coordinator
OUT2	=	coach
OUT3	=	sorter
OUT4	=	heapsort
CC      =	gcc
FLAGS   =       -Wall   -g      -c  -std=c99 	-I$(INC_DIR)	-D_XOPEN_SOURCE=700

all: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

$(OUT1): $(OBJS1)
	$(CC)   -g  -lm    $(OBJS1) -o    $@

$(OUT2): $(OBJS2)
	$(CC)   -g  -lm    $(OBJS2) -o    $@

$(OUT3): $(OBJS3)
	$(CC)   -g  -lm    $(OBJS3) -o    $@

$(OUT4): $(OBJS4)
	$(CC)   -g  -lm    $(OBJS4) -o    $@

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

sorter.o: ./src/sorter.c
	$(CC)   $(FLAGS)        ./src/sorter.c

clean:
	rm -f $(OBJS1) $(OUT1) $(OBJS2) $(OUT2) $(OBJS3) $(OUT3) $(OBJS4) $(OUT4) 

count:
	wc $(SOURCE) $(HEADER)

