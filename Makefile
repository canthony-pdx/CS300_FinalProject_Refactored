CPP = g++
CPPFLAGS = -g -Wall -Werror

frupal: frupal.cpp
	$(CPP) $(CPPFLAGS) -o frupal array.cpp controls.cpp screen.cpp player.cpp loader.cpp frupal.cpp  -lncurses

clean:
	-rm -f frupal
