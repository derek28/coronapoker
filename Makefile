#CFLAGS = -O1 -std=c++11
CFLAGS = -g -fstack-protector-all -std=c++11
CC = g++
DEPS = card.h deck.h pokerhand.h player.h game.h
OBJ = main.o pokerhand.o card.o deck.o player.o game.o
OBJTEST = test.o pokerhand.o card.o deck.o player.o game.o

all: poker test

poker: $(OBJ) 
	$(CC) $(CFLAGS) -o poker $(OBJ)     

test: $(OBJTEST)
	$(CC) $(CFLAGS) -o test $(OBJTEST)
    
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:                 
	rm -f *.o

