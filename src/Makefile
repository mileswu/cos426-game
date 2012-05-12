CXX=g++
CXXFLAGS=-Wall -g -I. 
CFLAGS=-Wall -g -I. 
MAKE=make

all: game
  
UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	GLLIBS = -framework OpenGL -framework GLUT -framework ApplicationServices
else
  ifneq (,$(findstring CYGWIN,$(UNAME)))
	GLLIBS = -lglut32 -lglut -lGLU -lGL
  else
	GLLIBS = -lglut -lGLU -lGL
  endif
endif


R3/libR3.a: 
	    $(MAKE) -C R3

R2/libR2.a: 
	    $(MAKE) -C R2

game: game.o gl.o geometry.o bubble.o world.o R3/libR3.a R2/libR2.a glew.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GLLIBS)

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
