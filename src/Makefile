CXX=g++
CXXFLAGS=-Wall -g

all: game
  
UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	GLLIBS = -framework GLUT -framework OpenGL -framework ApplicationServices
else
  ifneq (,$(findstring CYGWIN,$(UNAME)))
	GLLIBS = -lglut32 -lglut -lGLU -lGL
  else
	GLLIBS = -lglut -lGLU -lGL
  endif
endif

game: game.o gl.o geometry.o bubble.o world.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GLLIBS)

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
