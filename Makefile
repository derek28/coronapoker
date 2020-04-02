CFLAGS = -O1 -std=c++11
CC = g++
DEPS = card.h deck.h pokerhand.h
OBJ = main.o pokerhand.o card.o deck.o

poker: $(OBJ) 
	$(CC) $(CFLAGS) -o poker $(OBJ) 

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:                 
	rm -f *.o

