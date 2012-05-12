#include "gl.h"
#include "fbo.h"
#include "shader.h"
#include "world.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

#if defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#endif

using namespace std;

static World *world = NULL;
static int window_height = 500;
static int window_width = 500;
static R3Camera camera;
static double fps = 60;
static Framebuffer *direct_render_fbo, *bloom_preblur_fbo, *bloom_blurred_fbo;
static Shader *blur_shader, *bloom_preblur_shader, *bloom_composite_shader;

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

void printShaderInfoLog(GLuint obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
	        free(infoLog);
	    }
	}
	
	void printProgramInfoLog(GLuint obj)
  	{
  	    int infologLength = 0;
  	    int charsWritten  = 0;
  	    char *infoLog;

  		glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

  	    if (infologLength > 0)
  	    {
  	        infoLog = (char *)malloc(infologLength);
  	        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
  			printf("%s\n",infoLog);
  	        free(infoLog);
  	    }
  	}
  

void RedrawWindow() {
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
  R3Vector cameradisplacement_before = world->PlayerPosition() - camera.eye;
  world->Simulate();
  
  // Camera
  R3Vector cameradisplacement_after = world->PlayerPosition() - camera.eye;
  camera.eye.Translate(cameradisplacement_after - cameradisplacement_before);
  camera.Load(window_width, window_height);
  
  // Rendering of World into Framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, direct_render_fbo->framebuffer);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glColor3d(1,1,1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  world->Draw();
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // Threshold our FBO
  glBindFramebuffer(GL_FRAMEBUFFER, bloom_preblur_fbo->framebuffer);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(bloom_preblur_shader->program);
  glBindTexture(GL_TEXTURE_2D, direct_render_fbo->texture);
  glUniform1i(glGetUniformLocation(bloom_preblur_shader->program, "tex"), 0);
  DrawFullscreenQuad();
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  
  // Blur our FBO
  glBindFramebuffer(GL_FRAMEBUFFER, bloom_blurred_fbo->framebuffer);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(blur_shader->program);
  glBindTexture(GL_TEXTURE_2D, bloom_preblur_fbo->texture);
  glUniform1i(glGetUniformLocation(blur_shader->program, "tex"), 0);
  DrawFullscreenQuad();
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  
  // Composite our blur-bloom FBO + original FBO
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(bloom_composite_shader->program);
  glBindTexture(GL_TEXTURE_2D, bloom_blurred_fbo->texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, direct_render_fbo->texture);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(bloom_composite_shader->program, "tex_orig"), 1);
  glUniform1i(glGetUniformLocation(bloom_composite_shader->program, "tex_blur"), 0);
  DrawFullscreenQuad();
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  
  printProgramInfoLog(bloom_composite_shader->program);
  
  // OSD
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, window_width, 0, window_height);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor3d(1,1,1);
  

  double current_osd_height = window_height-25;
  double line_size = 15;
  
  glRasterPos2i(20,current_osd_height);
  
  string osd_text = world->PlayerStatus();
  for( string::iterator it = osd_text.begin(); it < osd_text.end(); it++) {
    if(*it == '\n') {
      current_osd_height -= line_size;
      glRasterPos2i(20,current_osd_height);
      continue;
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *it);
  }
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  glutSwapBuffers();
}

void KeyboardInput(unsigned char key, int x, int y) {
  if(key == 'q')
    exit(0);
  cout << "Key pressed: " << key << endl;
}

void SpecialInput(int key, int x, int y) {
  if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
    R3Point scene_center = world->PlayerPosition();
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
    world->Emit(camera.towards);
  }
}

void MouseMovement(int x, int y) {
  int dx = x - window_width/2;
  int dy = y - window_height/2;
    
  R3Point scene_center = world->PlayerPosition();
  
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH); // | GLUT_STENCIL
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
  
  //glutIdleFunc(IdleLoop);
  glutTimerFunc(1000.0/fps, TimerFunc, 0);
  
  /*
  glutReshapeFunc(GLUTResize);*/
  glutSpecialFunc(SpecialInput);
  
  camera.eye = R3Point(0,0,-2);
  camera.yfov = 0.8;
  camera.xfov = 0.8;
  camera.up = R3Vector(0, 1, 0);
  camera.right = R3Vector(-1, 0, 0);
  camera.towards = R3Vector(0,0,1);
  
  world = new World();
  
  direct_render_fbo = new Framebuffer(window_width, window_height);
  bloom_preblur_fbo = new Framebuffer(window_width, window_height);
  bloom_blurred_fbo = new Framebuffer(window_width, window_height);
  blur_shader = new Shader("shader_blur");
  bloom_preblur_shader = new Shader("shader_bloompreblur");
  bloom_composite_shader = new Shader("shader_bloomcomposite");
  
  return 0;
}
