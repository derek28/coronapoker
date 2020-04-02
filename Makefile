CFLAGS = -O1 -std=c++11
CC = g++
DEPS = card.h deck.h pokerhand.h
OBJ = main.o pokerhand.o card.o deck.o

poker: $(OBJ) 
	$(CC) $(CFLAGS) -o poker $(OBJ) 

pokerhand.o: 
	$(CC) $(CFLAGS) -c pokerhand.cpp

card.o: 
	$(CC) $(CFLAGS) -c card.cpp

deck.o: 
	$(CC) $(CFLAGS) -c deck.cpp

.PHONY: clean
clean:                 
	rm -f *.o

