BIN=./bin
LIB=./lib
GXX=g++ -Wall -I$(LIB)

build: lib-build
	mkdir -p $(BIN) && $(GXX) -lncurses -o $(BIN)/game game.cc *.o

run: build
	$(BIN)/game

lib-build:
	$(GXX) -c snake.cc $(LIB)/*.cc

clean:
	rm -rf $(BIN) *.o

