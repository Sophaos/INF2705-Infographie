CONTEXT=sdl2
#CONTEXT=glfw3

CXXFLAGS += -DFENETRE_$(CONTEXT)
CXXFLAGS += -g -W -Wall -Wno-unused-parameter -Wno-deprecated-declarations -Wvla -Wno-c++11-extensions # -pedantic -std=c++11
CXXFLAGS += $(shell pkg-config --cflags glew)
CXXFLAGS += $(shell pkg-config --cflags $(CONTEXT))

LDFLAGS += -g
LDFLAGS += $(shell pkg-config --libs glew)
LDFLAGS += $(shell pkg-config --libs $(CONTEXT))

ifeq "$(shell uname)" "Darwin"
  LDFLAGS += -framework OpenGL
  ifeq "$(CONTEXT)" "glfw3"
    LDFLAGS += -lobjc -framework Foundation -framework Cocoa
  endif
endif

SRC=main

exe : $(SRC).exe
run : exe
	./$(SRC).exe
$(SRC).exe : *.cpp *.h
	$(CXX) $(CXXFLAGS) -o$@ *.cpp $(LDFLAGS)

clean :
	rm -rf *.o *.exe *.exe.dSYM
