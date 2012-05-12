#include "gl.h"

class Shader {
  public:
    Shader(char *name);
    GLuint fragment;
    GLuint vertex;
    GLuint program;
};