#ifndef GL_H
#define GL_H
#define GLEW_STATIC
#if defined(_WIN32) || defined(__CYGWIN__)
# ifndef USE_OPENGL32
#  define USE_OPENGL32
# endif
# ifndef NOMINMAX
#  define NOMINMAX
# endif
#define _STDCALL_SUPPORTED
#define _M_IX86
# include <windows.h>
# include <GL/glew.h>
# include <GL/glut.h>
#elif defined(__APPLE__)
# include <GL/glew.h>
# include <GLUT/glut.h>
#else 
# include <GL/glew.h>
# include <GL/glut.h>
#endif

int CreateGameWindow(int argc, char **argv);

#endif
