INC_DIR = 	./include
OBJS1   =	coordinator.o files.o
OBJS2 	=	coach.o	files.o
OBJS3	= 	heapsort.o	files.o
OBJS4	= 	quicksort.o	files.o
SOURCE  =	./src/coordinator.c ./src/heapsort.c ./src/quicksort.c ./src/coach.c ./src/files.c
HEADER  =	./include/coordinator.h ./include/heapsort.h ./include/quicksort.h ./include/coach.h ./include/files.h
OUT1    =	coordinator
OUT2	=	coach
OUT3	=	heapsort
OUT4	=	quicksort
CC      =	gcc
FLAGS   =       -Wall   -c  -std=c99 	-I$(INC_DIR)	-D_XOPEN_SOURCE=700

debug:      FLAGS    += -g
release:    FLAGS    += -O3

all: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

debug:      all
release:    all

$(OUT1): $(OBJS1)
	$(CC)   -lm    $(OBJS1) -o    $@

$(OUT2): $(OBJS2)
	$(CC)   -lm    $(OBJS2) -o    $@

$(OUT3): $(OBJS3)
	$(CC)   -lm    $(OBJS3) -o    $@

$(OUT4): $(OBJS4)
	$(CC)   -lm    $(OBJS4) -o    $@

$(OUT5): $(OBJS5)
	$(CC)   -lm    $(OBJS5) -o    $@

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
	rm -f $(OBJS1) $(OUT1) $(OBJS2) $(OUT2) $(OBJS3) $(OUT3) $(OBJS4) $(OUT4)

count:
	wc $(SOURCE) $(HEADER)

