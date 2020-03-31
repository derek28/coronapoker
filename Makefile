CFLAGS = -O1 -std=c++11
CC = g++

all: poker

poker: 
	$(CC) $(CFLAGS) -o poker main.o

pokerhand.o: pokerhand.cpp
	$(CC) $(CFLAGS) -c pokerhand.cpp

card.o: card.cpp
	$(CC) $(CFLAGS) -c card.cpp

deck.o: deck.cpp
	$(CC) $(CFLAGS) -c deck.cpp	

clean:                 
	rm -f *.o

