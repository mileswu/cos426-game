#include "gl.h"

void RedrawWindow() {
  
}

int CreateWindow() {
  int fake_argc = 0;
  char **fake_argv;
  glutInit(&fake_argc, fake_argv);
  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
  int GLUTwindow = glutCreateWindow("Game");
  
  glutDisplayFunc(RedrawWindow);
  
  /*glutIdleFunc(GLUTIdle);
  glutReshapeFunc(GLUTResize);
  glutKeyboardFunc(GLUTKeyboard);
  glutSpecialFunc(GLUTSpecial);
  glutMouseFunc(GLUTMouse);
  glutMotionFunc(GLUTMotion);*/
  
  return 0;
}
