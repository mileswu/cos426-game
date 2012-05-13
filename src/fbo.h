#include "gl.h"

class Framebuffer {
  public:
    Framebuffer(int w, int h);
    GLuint framebuffer;
    GLuint texture;
    GLuint renderbuffer;
};