CC=g++
LDFLAGS=
CFLAGS=-std=c++14 -O3

OUTPUT=todo

SRCLIST=$(wildcard ./src/*.cpp)
OBJLIST=$(patsubst ./src/%.cpp,./obj/%.o, $(SRCLIST))

$(OUTPUT): $(OBJLIST)
	g++ $(LDFLAGS) -o $(OUTPUT) $(OBJLIST)

$(OBJLIST): $(SRCLIST)
	g++ $(CFLAGS) -c -o $@ $<

clean:
	rm ./obj/*.o	
