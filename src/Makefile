CXX      := g++
CXXFLAGS := -Wall -g -I. -I../deps/SFML-2.0-rc/include
CFLAGS   := -Wall -g -I. 
MAKE     := make

all: game
  
UNAME := $(shell uname)
ifneq (,$(findstring Darwin,$(UNAME)))
	GLLIBS := -framework OpenGL -framework GLUT -framework ApplicationServices -lsfml-audio -L../deps/SFML-2.0-rc/lib #-framework sfml-audio 
else
  ifneq (,$(findstring CYGWIN,$(UNAME)))
	GLLIBS := -lglut32 -lglu32 -lopengl32 -lsfml-audio -lsfml-main -L../deps/SFML-2.0-rc/lib
  else
	GLLIBS := -lglut -lGLU -lGL
  endif
endif

R3/libR3.a: 
	$(MAKE) -C R3

R2/libR2.a: 
	$(MAKE) -C R2

game: game.o gl.o geometry.o bubble.o world.o ai.o R3/libR3.a R2/libR2.a glew.o fbo.o shader.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GLLIBS)

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
