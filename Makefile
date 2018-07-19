CC=g++
LDFLAGS=
CFLAGS=-std=c++14 -O3

OUTPUT=todo

SRCLIST=$(wildcard ./src/*.cpp)
OBJLIST=$(patsubst ./src/%.cpp,./obj/%.o, $(SRCLIST))

## Uncomment -DUNICODEGLYPHS to enable special unicode check marks for completed items
## Be aware that this won't work out-of-the-box on all terminals.
DEFS= #-DUNICODEGLYPHS

$(OUTPUT): $(OBJLIST)
	g++ $(LDFLAGS) -o $(OUTPUT) $(OBJLIST)

$(OBJLIST): $(SRCLIST)
	g++ $(CFLAGS) -c -o $@ $< $(DEFS)

clean:
	rm ./obj/*.o	
