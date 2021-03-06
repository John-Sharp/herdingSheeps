CC = gcc

CFLAGS = -g -Wall -lm -lSDL2 -lSDL2_image

all: generateListCode main.c ../engine/engine.h ../engine/engine.c ../inputProcessor/inputProcessor.h ../inputProcessor/inputProcessor.c ../coll/coll.h ../coll/coll.c  ../jTypes/jTypes.h
	$(CC) main.c ../engine/engine.c ../coll/coll.c ../jTypes/jTypes.c ../inputProcessor/inputProcessor.c $(CFLAGS) -o herdingSheeps

generateListCode: ../dataStructures/templates/listTemplate.h ../dataStructures/templates/listTemplate.inc
	../dataStructures/generateList.py ../engine/listHeaders ../engine/listCode actor:actor preLogicCallBack:preLogicCallBack
	../dataStructures/generateList.py ../coll/listHeaders ../coll/listCode jcObject:jcObject jcPairing:jcPairing jcRegisteredCollHandler:jcRegisteredCollHandler

clean:
	rm -f *.o
