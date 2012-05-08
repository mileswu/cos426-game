CXX=g++
CXXFLAGS=-Wall -g

all: game
  
UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	GLLIBS = -framework GLUT -framework OpenGL
else
  ifneq (,$(findstring CYGWIN,$(UNAME)))
	GLLIBS = -lglut32 -lglut -lGLU -lGL
  else
	GLLIBS = -lglut -lGLU -lGL
  endif
endif

game: game.o gl.o
	$(CXX) $(CXXFLAGS) $(GLLIBS) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#