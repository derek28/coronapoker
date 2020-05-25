#CFLAGS = -O1 -std=c++11
CFLAGS = -g -DDEBUG -fstack-protector-all -std=c++11
CC = g++
DEPS = card.h deck.h pokerhand.h player.h game.h strength.h human_player.h random_player.h ehs_player.h
OBJ     = main.o pokerhand.o card.o deck.o player.o game.o strength.o misc.o random_player.o ehs_player.o
OBJTEST = test.o pokerhand.o card.o deck.o player.o game.o strength.o misc.o human_player.o random_player.o server.o
OBJHUMANVSBOT = humanvsbot.o pokerhand.o card.o deck.o player.o game.o strength.o misc.o human_player.o random_player.o ehs_player.o server.o
OBJBENCHMARK = benchmark.o pokerhand.o card.o deck.o player.o game.o misc.o random_player.o

all: poker test humanvsbot benchmark

poker: $(OBJ) 
	$(CC) $(CFLAGS) -o poker $(OBJ)     

test: $(OBJTEST)
	$(CC) $(CFLAGS) -o test $(OBJTEST)

humanvsbot: $(OBJHUMANVSBOT)
	$(CC) $(CFLAGS) -o humanvsbot $(OBJHUMANVSBOT)

benchmark: $(OBJBENCHMARK)
	$(CC) $(CFLAGS) -o benchmark $(OBJBENCHMARK)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:                 
	rm -f *.o

