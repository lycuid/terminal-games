LIB=../lib
GXX=g++ -Wall -I$(LIB)

build: lib-build
	$(GXX) -lncurses -o game.out game.cc *.o

run: build
	./game.out

lib-build:
	$(GXX) -c snake.cc $(LIB)/*.cc

clean:
	rm -rf game.out *.o

