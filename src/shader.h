#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

class Shader {
  public:
    Shader(char *name);
    GLuint fragment;
    GLuint vertex;
    GLuint program;
};

#endif
