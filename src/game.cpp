#include <iostream>
#include "game.h"
#include "gl.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv) {
  CreateWindow();
  
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
  
  glutMainLoop();
  
  return 0;
}
