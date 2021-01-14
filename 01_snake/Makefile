BIN=./bin
INCLUDE=./Include
GXX=g++ -Wall -I$(INCLUDE)

build: includes.o
	mkdir -p $(BIN) && $(GXX) -lncurses -o $(BIN)/game game.cc *.o

includes.o:
	$(GXX) -c $(INCLUDE)/*.cc

run: build
	$(BIN)/game

clean:
	rm -rf $(BIN) *.o

