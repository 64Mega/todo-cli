CC=g++
LDFLAGS=-static-libgcc -static-libstdc++
CFLAGS=-std=gnu++14 -O3

OUTPUT=todo

SRCLIST=$(wildcard ./src/*.cpp)
OBJLIST=$(patsubst ./src/%.cpp,./obj/%.o, $(SRCLIST))

## Define UNICODEGLYPHS to allow for output of ✅ instead of 
## [X] for completed todos. 
## If this doesn't work in your terminal of choice, removing the
## definition will revert to the standard [X] completion box.
DEFS=-DUNICODEGLYPHS

$(OUTPUT): $(OBJLIST)
	g++ $(LDFLAGS) -o $(OUTPUT) $(OBJLIST)

$(OBJLIST): $(SRCLIST)
	g++ $(CFLAGS) -c -o $@ $< $(DEFS)

clean:
	rm ./obj/*.o	
