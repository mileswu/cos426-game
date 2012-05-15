#ifndef SHADER_H
#define SHADER_H

#include "gl.h"

class Shader {
  public:
    Shader(const char *name);
    GLuint fragment;
    GLuint vertex;
    GLuint program;
};

#endif
