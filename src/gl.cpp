#include "gl.h"
#include "world.h"
#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
using namespace std;

static World *world = NULL;
static int window_height = 500;
static int window_width = 500;
static R3Camera camera;

void RedrawWindow() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  int light_index = GL_LIGHT0;
  glDisable(light_index);
  GLfloat c[4];
  c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  glLightfv(light_index, GL_DIFFUSE, c);
  c[0] = 1; c[1] = 0; c[2] = 1; c[3] = 0;
  glLightfv(light_index, GL_POSITION, c);
  glEnable(light_index);
  
  c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  //c[0] = 0.2; c[1] = 0.2; c[2] = 0.2; c[3] = 1;  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c);
  
  camera.Load(window_width, window_height);
  
  glDisable(GL_LIGHTING);
  glColor3d(1,1,1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  world->Draw();
  
  glutSwapBuffers();
}

void KeyboardInput(unsigned char key, int x, int y) {
  if(key == 'q')
    exit(0);
  cout << "Key pressed: " << key << endl;
}

void SpecialInput(int key, int x, int y) {
  if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
    R3Point scene_center(0,0,0);
    R3Vector v = scene_center - camera.eye;
    if(key == GLUT_KEY_UP) {
      v = -0.2*v;
    } else {
      v = 0.2*v;
    }
    camera.eye.Translate(v);
  }
  glutPostRedisplay();
}

void MouseInput(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    cout << "Emit stuff" << endl;
    world->Emit(camera.towards);
  }
  cout << "Mouse button pressed: " << button << endl;
}

void MouseMovement(int x, int y) {
  int dx = x - window_width/2;
  int dy = y - window_height/2;
    
  R3Point scene_center(0,0,0);
  
  double vx = (double) dx / (double) window_width;
  double vy = (double) dy / (double) window_height;
  double theta = 4.0 * (fabs(vx) + fabs(vy));
  R3Vector vector = (camera.right * vx) + (camera.up * vy);
  R3Vector rotation_axis = camera.towards % vector;
  rotation_axis.Normalize();
  
  camera.eye.Translate(- scene_center.Vector());
  camera.eye.Rotate(rotation_axis, theta);
  camera.eye.Translate(scene_center.Vector());
  
  camera.towards.Rotate(rotation_axis, theta);
  camera.up.Rotate(rotation_axis, theta);
  camera.right = camera.towards % camera.up;
  camera.up = camera.right % camera.towards;
  camera.towards.Normalize();
  camera.up.Normalize();
  camera.right.Normalize();
  
  glutWarpPointer(window_width/2, window_height/2);
  glutPostRedisplay();
}

void IdleLoop() {
  glutPostRedisplay();
}

int CreateWindow() {
  int fake_argc = 0;
  char **fake_argv;
  glutInit(&fake_argc, fake_argv);
  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(window_width, window_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
  CGSetLocalEventsSuppressionInterval(0.0);
  int GLUTwindow = glutCreateWindow("Game");
  
  glutWarpPointer(window_width/2, window_height/2);
  glutSetCursor(GLUT_CURSOR_NONE);
  glutDisplayFunc(RedrawWindow);
  glutKeyboardFunc(KeyboardInput);
  glutMouseFunc(MouseInput);
  glutPassiveMotionFunc(MouseMovement);
  
  glutIdleFunc(IdleLoop);/*
  glutReshapeFunc(GLUTResize);*/
  glutSpecialFunc(SpecialInput);
  
  camera.eye = R3Point(0,0,-2);
  camera.yfov = 0.8;
  camera.xfov = 0.8;
  camera.up = R3Vector(0, 1, 0);
  camera.right = R3Vector(-1, 0, 0);
  camera.towards = R3Vector(0,0,1);
  
  world = new World();
  
  return 0;
}
