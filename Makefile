CC = gcc

CFLAGS = -g -Wall -lm -lSDL2 -lSDL2_image

all: generateListCode main.c ../engine/engine.h ../engine/engine.c 
	$(CC) main.c ../engine/engine.c $(CFLAGS) -o herdingSheeps

generateListCode: ../dataStructures/templates/listTemplate.h ../dataStructures/templates/listTemplate.inc
	../dataStructures/generateList.py ../engine/listHeaders ../engine/listCode actor:actor

clean:
	rm -f *.o
