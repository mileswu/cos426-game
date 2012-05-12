#include "gl.h"
#include "world.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

#if defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#endif

using namespace std;

static World *world = NULL;
static int window_height = 500;
static int window_width = 500;
static R3Camera back_camera;
static R3Camera view_camera;
static double fps = 60;

static double frame_rendertimes[100];
static int frame_rendertimes_i = 0;

void DrawFullscreenQuad() {
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glColor3d(1,1,1);  
  glBegin(GL_QUADS);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (-1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (1.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (-1.0, 1.0, 0.0);
  glEnd();
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void RedrawWindow() {
  // FPS timing
  frame_rendertimes[frame_rendertimes_i] = glutGet(GLUT_ELAPSED_TIME);
  frame_rendertimes_i = (frame_rendertimes_i + 1)%100;
  double avg_time = 0;
  for(int i=0; i<frame_rendertimes_i; i++) {
    if(i == 0) {
      avg_time += frame_rendertimes[i] - frame_rendertimes[99];
    }
    else {
      avg_time += frame_rendertimes[i] - frame_rendertimes[i-1];
    }
  }
  for(int i=frame_rendertimes_i+1; i<100; i++) {
    avg_time += frame_rendertimes[i] - frame_rendertimes[i-1];
  }
  avg_time /= 100.0;
  stringstream fps_achieved_ss;
  fps_achieved_ss << "FPS achieved: " << 1000.0/avg_time << endl;
  
  // Initialization
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Lighting
  int light_index = GL_LIGHT0;
  glDisable(light_index);
  GLfloat c[4];
  c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  glLightfv(light_index, GL_DIFFUSE, c);
  glLightfv(light_index, GL_SPECULAR, c);
  c[0] = 1; c[1] = 0; c[2] = 1; c[3] = 0;
  glLightfv(light_index, GL_POSITION, c);
  glEnable(light_index);
  
  //c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  c[0] = 0.2; c[1] = 0.2; c[2] = 0.2; c[3] = 1;  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c);
  
  // Simulation
  R3Vector cameradisplacement_before = world->PlayerPosition() - view_camera.eye;
  R3Vector back_cameradisplacement_before = world->PlayerPosition() - back_camera.eye;
  world->Simulate();
  
  // Camera
  R3Vector cameradisplacement_after = world->PlayerPosition() - view_camera.eye;
  R3Vector back_cameradisplacement_after = world->PlayerPosition() - back_camera.eye;
  view_camera.eye.Translate(cameradisplacement_after - cameradisplacement_before);
  back_camera.eye.Translate(back_cameradisplacement_after - back_cameradisplacement_before);
  view_camera.Load(window_width, window_height);

  // Backlight where bubbles emit
  light_index = GL_LIGHT1;
  glDisable(light_index);
  c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  glLightfv(light_index, GL_DIFFUSE, c);
  glLightfv(light_index, GL_SPECULAR, c);
  //glLightfv(light_index, GL_AMBIENT, c);
  

  c[0] = back_camera.eye.X(); c[1] = back_camera.eye.Y(); c[2] = back_camera.eye.Z(); c[3] = 1;
  glLightfv(light_index, GL_POSITION, c);
  glEnable(light_index);


  // Rendering of World
  glColor3d(1,1,1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_MULTISAMPLE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  view_camera.CalcPlanes();
  world->Draw(view_camera);
  
  // OSD
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, window_width, 0, window_height);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  double current_osd_height = window_height-25;
  double line_size = 15;
  
  glRasterPos2i(20,current_osd_height);
  
  string osd_text = world->PlayerStatus() + fps_achieved_ss.str();
  for( string::iterator it = osd_text.begin(); it < osd_text.end(); it++) {
    if(*it == '\n') {
      current_osd_height -= line_size;
      glRasterPos2i(20,current_osd_height);
      continue;
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *it);
  }
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  glViewport(0, 0, 100, 100);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  
  DrawFullscreenQuad();
  world->DrawMinimap();
  glPopMatrix();
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  
  glViewport(0, 0, window_width, window_height);
  
  glutSwapBuffers();
  
}

void KeyboardInput(unsigned char key, int x, int y) {
  if(key == 'q')
    exit(0);
  double vx = 0;
  double vy = 0;
  // move the placement of where the bubbles emit from
  if (key == 'w') { // up
    vy = 1;
  }
  else if (key == 'a') { // left
    vx = -1;
  }
  else if (key == 's') { // down
    vy = -1;
  }
  else if (key == 'd') { // right
    vx = 1;
  }
  else {
    cout << "Key pressed: " << key << endl;
    return;
  }

  vx /= 10;
  vy /= 10;

  R3Point scene_center = world->PlayerPosition();
  double theta = 4.0 * (fabs(vx) + fabs(vy));
  R3Vector vector = (back_camera.right * vx) + (back_camera.up * vy);
  R3Vector rotation_axis = back_camera.towards % vector;
  rotation_axis.Normalize();
  
  back_camera.eye.Translate(- scene_center.Vector());
  back_camera.eye.Rotate(rotation_axis, theta);
  back_camera.eye.Translate(scene_center.Vector());
  
  back_camera.towards.Rotate(rotation_axis, theta);
  back_camera.up.Rotate(rotation_axis, theta);
  back_camera.right = back_camera.towards % back_camera.up;
  back_camera.up = back_camera.right % back_camera.towards;
  back_camera.towards.Normalize();
  back_camera.up.Normalize();
  back_camera.right.Normalize();


}

void SpecialInput(int key, int x, int y) {
  if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
    R3Point scene_center = world->PlayerPosition();
    R3Vector v = scene_center - view_camera.eye;
    if(key == GLUT_KEY_UP) {
      v = -0.2*v;
    } else {
      v = 0.2*v;
    }
    view_camera.eye.Translate(v);
  }
  glutPostRedisplay();
}

void MouseInput(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    world->Emit(back_camera.towards);
  }
}

void MouseMovement(int x, int y) {
  int dx = x - window_width/2;
  int dy = y - window_height/2;
    
  R3Point scene_center = world->PlayerPosition();
  
  double vx = (double) dx / (double) window_width;
  double vy = (double) dy / (double) window_height;
  double theta = 4.0 * (fabs(vx) + fabs(vy));
  R3Vector vector = (view_camera.right * vx) + (view_camera.up * vy);
  R3Vector rotation_axis = view_camera.towards % vector;
  rotation_axis.Normalize();
  
  view_camera.eye.Translate(- scene_center.Vector());
  view_camera.eye.Rotate(rotation_axis, theta);
  view_camera.eye.Translate(scene_center.Vector());
  
  view_camera.towards.Rotate(rotation_axis, theta);
  view_camera.up.Rotate(rotation_axis, theta);
  view_camera.right = view_camera.towards % view_camera.up;
  view_camera.up = view_camera.right % view_camera.towards;
  view_camera.towards.Normalize();
  view_camera.up.Normalize();
  view_camera.right.Normalize();
  
  glutWarpPointer(window_width/2, window_height/2);
  glutPostRedisplay();
}

void IdleLoop() {
  glutPostRedisplay();
}

void TimerFunc(int stuff) {
  glutPostRedisplay();
  glutTimerFunc(1000.0/fps, TimerFunc, stuff);
}

int CreateWindow() {
  int fake_argc = 0;
  char **fake_argv;
  glutInit(&fake_argc, fake_argv);
  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(window_width, window_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_MULTISAMPLE); // | GLUT_STENCIL
#if defined(__APPLE__)
  CGSetLocalEventsSuppressionInterval(0.0);
#endif
  int GLUTwindow = glutCreateWindow("Game");
  
  glutWarpPointer(window_width/2, window_height/2);
  glutSetCursor(GLUT_CURSOR_NONE);
  glutDisplayFunc(RedrawWindow);
  glutKeyboardFunc(KeyboardInput);
  glutMouseFunc(MouseInput);
  glutPassiveMotionFunc(MouseMovement);
  
  glutIdleFunc(IdleLoop);
  //glutTimerFunc(1000.0/fps, TimerFunc, 0);
  
  /*
  glutReshapeFunc(GLUTResize);*/
  glutSpecialFunc(SpecialInput);
  
  view_camera.eye = R3Point(0,0,-2);
  view_camera.yfov = 0.8;
  view_camera.xfov = 0.8;
  view_camera.up = R3Vector(0, 1, 0);
  view_camera.right = R3Vector(-1, 0, 0);
  view_camera.towards = R3Vector(0,0,1);

  back_camera = view_camera;
  
  world = new World();
  
  return 0;
}
