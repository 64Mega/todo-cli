CC=g++
LDFLAGS=
CFLAGS=-std=c++14 -g -O0

OUTPUT=todo.exe

SRCLIST=$(wildcard ./src/*.cpp)
OBJLIST=$(patsubst ./src/%.cpp,./obj/%.o, $(SRCLIST))

DEFS=DUMMY

$(OUTPUT): $(OBJLIST)
	g++ $(LDFLAGS) -o $(OUTPUT) $(OBJLIST)

$(OBJLIST): $(SRCLIST)
	g++ $(CFLAGS) -c -o $@ $< -D$(DEFS)

clean:
	rm ./obj/*.o	
