BIN=./bin
INCLUDE=./include
CXXFLAGS=-Wall -I$(INCLUDE)

build: includes.o
	mkdir -p $(BIN) && $(CXX) $(CXXFLAGS) -lncurses -o $(BIN)/game game.cc *.o

includes.o:
	$(CXX) $(CXXFLAGS) -c $(INCLUDE)/*.cc

run: build
	$(BIN)/game

clean:
	rm -rf $(BIN) *.o

