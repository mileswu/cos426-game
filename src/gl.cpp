#include "gl.h"
#include "fbo.h"
#include "shader.h"
#include "world.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

#if defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#endif

using namespace std;

static World *world = NULL;
static int window_height = 850;
static int window_width = 850;
static R3Camera back_camera;
static R3Camera view_camera;
static double fps = 60;
static Framebuffer *direct_render_fbo, *processing_fbo1, *processing_fbo2;
static Shader *blur_shader_x, *blur_shader_y, *bloom_preblur_shader, *bloom_composite_shader, *bump_shader, *world_cubemap_shader, *laser_shader;
static double frame_rendertimes[100];
static int frame_rendertimes_i = 0;
static int hasgoodgpu = 0;
static GLuint world_texture, world_texture_2d, bubble_texture, bubble_texture2, particle_sprite, menu_texture, menu_on_texture, menu_off_texture, menu_ball_texture, menu_low_texture, menu_high_texture;
static int config[7] = {1, 0, 1, 1, 1, 1, 0};
static int config_pointer = 0;
static int config_maxpointer = 6;
static int paused = 0;
#define ADV_CONTROLS config[6]

void Reset(int status);

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
  	
void RedrawMenu() {
  glDisable(GL_DEPTH_TEST);
  
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glScalef(1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  

  glEnable(GL_TEXTURE_2D);
  
  glBindTexture(GL_TEXTURE_2D, menu_texture);
  DrawFullscreenQuad();
  glBindTexture(GL_TEXTURE_2D, 0);
  
  int config_i = 0;
  for(double i=445.0; i<740; i+=44.0) {
    //if(i < 524) continue;
    glPushMatrix();
    glTranslatef((705.0-512.0)/512.0, -(i-512.0)/512.0, 0.0);

    if(config_i == 0) 
      glBindTexture(GL_TEXTURE_2D, menu_high_texture);
    else
      glBindTexture(GL_TEXTURE_2D, menu_on_texture);
    if(config[config_i] == 0)
      glColor3d(11.0/255.0,1.0,1.0);
    else
      glColor3d(1.0,151.0/255.0,59.0/255.0);
    
    glBegin(GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (-0.03125, -0.03125, 0.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (0.03125, -0.03125, 0.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (0.03125, 0.03125, 0.0);
    glTexCoord2f (0.0, 1.0);
    glVertex3f (-0.03125, 0.03125, 0.0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glTranslatef(50.0/512.0, 0, 0.0);
    if(config_i == 0) 
      glBindTexture(GL_TEXTURE_2D, menu_low_texture);
    else
      glBindTexture(GL_TEXTURE_2D, menu_off_texture);
    if(config[config_i] != 0)
      glColor3d(11.0/255.0,1.0,1.0);
    else
      glColor3d(1.0,151.0/255.0,59.0/255.0);
    glBegin(GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (-0.03125, -0.03125, 0.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (0.03125, -0.03125, 0.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (0.03125, 0.03125, 0.0);
    glTexCoord2f (0.0, 1.0);
    glVertex3f (-0.03125, 0.03125, 0.0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if(config_pointer == config_i) {
      glTranslatef(-585.0/512.0, 3.0/512.0, 0.0);
      
      glColor3d(1.0,1.0, 1.0);
      
      glBindTexture(GL_TEXTURE_2D, menu_ball_texture);
      glBegin(GL_QUADS);
      glTexCoord2f (0.0, 0.0);
      glVertex3f (-0.04, -0.04, 0.0);
      glTexCoord2f (1.0, 0.0);
      glVertex3f (0.04, -0.04, 0.0);
      glTexCoord2f (1.0, 1.0);
      glVertex3f (0.04, 0.04, 0.0);
      glTexCoord2f (0.0, 1.0);
      glVertex3f (-0.04, 0.04, 0.0);
      glEnd();
      glBindTexture(GL_TEXTURE_2D, 0);
      
      /*static GLUquadricObj *glu_sphere = gluNewQuadric();
      gluQuadricTexture(glu_sphere, GL_TRUE);
      gluQuadricNormals(glu_sphere, (GLenum) GLU_SMOOTH);
      gluQuadricOrientation(glu_sphere, GLU_OUTSIDE);
      gluSphere(glu_sphere, 0.05, 32, 32);*/
    }
    
    glPopMatrix();
    config_i++;
  }
  
  
  
  
  glutSwapBuffers();
  
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

  // Simulation
  R3Vector cameradisplacement_before = world->PlayerPosition() - view_camera.eye;
  R3Vector back_cameradisplacement_before = world->PlayerPosition() - back_camera.eye;
  world->Timestep();
  if(paused == 0) {
    if (world->world_status != -1) {
      world->Simulate();
    } else {
      world->SimulateMotion();
    }
  }

  // Camera
  R3Vector cameradisplacement_after = world->PlayerPosition() - view_camera.eye;
  R3Vector back_cameradisplacement_after = world->PlayerPosition() - back_camera.eye;
  view_camera.eye.Translate(cameradisplacement_after - cameradisplacement_before);
  if((view_camera.eye - world->PlayerPosition()).Length() < (world->PlayerSize()*1.1)) {
    view_camera.eye.Translate(view_camera.towards* ((view_camera.eye - world->PlayerPosition()).Length() - (world->PlayerSize()*1.1))    );
  }
  if((view_camera.eye - world->PlayerPosition()).Length() > 30) {
    view_camera.eye.Translate(view_camera.towards* ((view_camera.eye - world->PlayerPosition()).Length() - 30)    );
  }
  back_camera.eye.Translate(back_cameradisplacement_after - back_cameradisplacement_before);
  view_camera.Load(window_width, window_height);

  // Lighting
  //glMatrixMode(GL_MODELVIEW);
  //glPopMatrix();
  //glLoadIdentity();
  int light_index = GL_LIGHT0;
  glDisable(light_index);
  GLfloat c[4];
  c[0] = 1; c[1] = 1; c[2] = 1.0; c[3] = 1;
  glLightfv(light_index, GL_DIFFUSE, c);
  glLightfv(light_index, GL_SPECULAR, c);
  c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  glLightfv(light_index, GL_POSITION, c);
  glEnable(light_index);

  //c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  c[0] = 0.2; c[1] = 0.2; c[2] = 0.2; c[3] = 1;  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c);

  // Backlight where bubbles emit
  light_index = GL_LIGHT1;
  glDisable(light_index);
  c[0] = 1; c[1] = 1; c[2] = 1; c[3] = 1;
  glLightfv(light_index, GL_DIFFUSE, c);
  glLightfv(light_index, GL_SPECULAR, c);
  //glLightfv(light_index, GL_AMBIENT, c);
  c[0] = back_camera.eye.X(); c[1] = back_camera.eye.Y(); c[2] = back_camera.eye.Z(); c[3] = 1;
  glLightfv(light_index, GL_POSITION, c);
  //glEnable(light_index);

  // Rendering of World into Framebuffer
  if(hasgoodgpu) {
    glBindFramebuffer(GL_FRAMEBUFFER, direct_render_fbo->framebuffer);
    glUseProgram(bump_shader->program);
    glBindTexture(GL_TEXTURE_CUBE_MAP, bubble_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, bubble_texture2);
    glActiveTexture(GL_TEXTURE0);
    
    double t = 0.5*glutGet(GLUT_ELAPSED_TIME)/1000.0;
    double mixer = 2.0*(t - (int)t);
    if(mixer > 1) {
      mixer = 2.0 - mixer;
    }
    t += 666;
    t *= 1.5;
    double bumpmixer = 2.0*(t - (int)t);
    if(bumpmixer > 1) {
      bumpmixer = 2.0 - bumpmixer;
    }
    bumpmixer = bumpmixer*0.5 + 0.5;
    
    glUniform1i(glGetUniformLocation(bump_shader->program, "tex"), 0);
    glUniform1i(glGetUniformLocation(bump_shader->program, "tex2"), 1);
    glUniform1f(glGetUniformLocation(bump_shader->program, "mixer"), mixer);
    glUniform1f(glGetUniformLocation(bump_shader->program, "bumpmixer"), bumpmixer);
  }
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glColor3d(1,1,1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_MULTISAMPLE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  view_camera.CalcPlanes();
  int occlusion = (ADV_CONTROLS == 1) ? 0 : 1;
  if(hasgoodgpu) {
    world->Draw(view_camera, bump_shader, occlusion);
    printProgramInfoLog(bump_shader->program);
  } else {
    world->Draw(view_camera, NULL, occlusion);
  }

  // Render powerups.
  if (hasgoodgpu) {
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  world->DrawPowerups(view_camera);

  // Render particle trails.
  glBindTexture(GL_TEXTURE_2D, particle_sprite);
  glColor4d(1,1,1,0.1);
  glEnable(GL_POINT_SPRITE);
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
  //glPointSize(15.0);
  //glBegin(GL_POINTS);
  world->DrawTrails(view_camera);
  //glVertex3f(0,0,-2);
  glDisable(GL_POINT_SPRITE);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // Render laser.
  if (hasgoodgpu) {
    glUseProgram(laser_shader->program);
  }
  R3Point playerpos = world->PlayerPosition();
  R3Vector playerdir = world->PlayerDirection();
  glLineWidth(3.0);
  glBegin(GL_LINE_STRIP);
  double length = world->PlayerSize() * 3.0;
  for (double i=0; i<length; i += 0.01) {
    glColor4d(1,0.5,0.0,1.0 - i/length);
    //glVertex3f(playerpos[0] + back_camera.towards.X()*i, playerpos[1] + back_camera.towards.Y()*i, playerpos[2] + back_camera.towards.Z()*i);
    if(ADV_CONTROLS == 1) {
      glVertex3f(playerpos[0] + back_camera.towards.X()*i,
                 playerpos[1] + back_camera.towards.Y()*i,
                 playerpos[2] + back_camera.towards.Z()*i);
    } else {
    glVertex3f(playerpos[0] + playerdir.X()*i,
               playerpos[1] + playerdir.Y()*i,
               playerpos[2] + playerdir.Z()*i);
    }
  }
  glEnd();
  glColor4d(1,1,1,1);
  
  // Render world sphere.
  if (hasgoodgpu) {
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, world_texture);
    glUseProgram(world_cubemap_shader->program);
    glUniform1i(glGetUniformLocation(world_cubemap_shader->program, "tex"), 0);
  }
  else {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, world_texture_2d);
  }
  world->DrawWorld(view_camera);
  if (hasgoodgpu) {
    glUseProgram(0);
  }
    glBindTexture(GL_TEXTURE_2D, 0);
  

  if (hasgoodgpu) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Threshold our FBO
    glBindFramebuffer(GL_FRAMEBUFFER, processing_fbo1->framebuffer);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(bloom_preblur_shader->program);
    glBindTexture(GL_TEXTURE_2D, direct_render_fbo->texture);
    glUniform1i(glGetUniformLocation(bloom_preblur_shader->program, "tex"), 0);
    DrawFullscreenQuad();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
    // Blur our FBO in x direction
    glBindFramebuffer(GL_FRAMEBUFFER, processing_fbo2->framebuffer);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(blur_shader_x->program);
    glBindTexture(GL_TEXTURE_2D, processing_fbo1->texture);
    glUniform1i(glGetUniformLocation(blur_shader_x->program, "tex"), 0);
    DrawFullscreenQuad();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
    // Blur our FBO in y direction
    glBindFramebuffer(GL_FRAMEBUFFER, processing_fbo1->framebuffer);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(blur_shader_y->program);
    glBindTexture(GL_TEXTURE_2D, processing_fbo2->texture);
    glUniform1i(glGetUniformLocation(blur_shader_y->program, "tex"), 0);
    DrawFullscreenQuad();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
    // Composite our blur-bloom FBO + original FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(bloom_composite_shader->program);
    glBindTexture(GL_TEXTURE_2D, processing_fbo1->texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, direct_render_fbo->texture);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(bloom_composite_shader->program, "tex_orig"), 1);
    glUniform1i(glGetUniformLocation(bloom_composite_shader->program, "tex_blur"), 0);
    DrawFullscreenQuad();
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    
    glUseProgram(0);
  }
  
  // OSD
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, window_width, 0, window_height);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor3d(1,1,1);
  

  double current_osd_height = window_height-50;
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
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  glViewport(0, 0, 100, 100);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  glDisable(GL_DEPTH_TEST);
  
  DrawFullscreenQuad();
  world->DrawMinimap();    
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  
  glViewport(0, 0, window_width, window_height);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  world->DrawOverlay();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  
  glutSwapBuffers();
  
}


void GoBackToMenu();
void KeyboardInput(unsigned char key, int x, int y) {
  if (key == 'q' || key == 27) {
    GoBackToMenu();
  }
  double vx = 0;
  double vy = 0;
  // move the placement of where the bubbles emit from
  if (key == 'w') { // up
    vy = 1;
  }
  else if (key == 'a') { // left
    vx = 1;
  }
  else if (key == 's') { // down
    vy = -1;
  }
  else if (key == 'd') { // right
    vx = -1;
  }
  else if (key == 'r') {
    Reset(0);
    return;
  }
  else if (key == 'p') {
    paused = (paused == 0 ? 1 : 0);
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

void MouseInput(int button, int state, int x, int y);

void KeyboardEditInput(unsigned char key, int x, int y) {
  static int v = 0;
  static double c = 5.;
  switch (key) {
  case 27:
  case 'q': GoBackToMenu(); break;
  case 'w': v = 1;  world->SetPlayerVelocity(v * c * view_camera.towards); break;
  case 's': v = -1; world->SetPlayerVelocity(v * c * view_camera.towards); break;
  case 'e': v = 0;  world->SetPlayerVelocity(v * c * view_camera.towards); break;
  case 'r': Reset(-1); break;
  case 13: {
    glutKeyboardFunc(KeyboardInput);
    glutMouseFunc(MouseInput);
    R3Vector d = world->PlayerPosition() - R3Point(0, 0, 0);
    view_camera.eye.Translate(-d);
    back_camera.eye.Translate(-d);
    world->SetPlayerPosition(R3Point(0, 0, 0));
    world->world_status = 0;
  } break;
  default: ;
  }
  return;
}

void KeyboardMenuInput(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
  case 'q': exit(0);
  case 13:  Reset(0); break;
  case 'e': Reset(-1); break;
  default: ;
  }
  return;
}

void SpecialMenuInput(int key, int x, int y) {
  if(key == GLUT_KEY_UP) {
    config_pointer = config_pointer-1;
    if(config_pointer < 0) { config_pointer = config_maxpointer; }
  }
  else if(key == GLUT_KEY_DOWN) {
    config_pointer = config_pointer+1;
    if(config_pointer > config_maxpointer) { config_pointer = 0; }
  }
  else if(key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT) {
    if(config_pointer == 3) {
      if(!GLEW_ARB_framebuffer_object || !GLEW_ARB_fragment_program) {
        return;
      }
    }
    config[config_pointer] = (config[config_pointer]+1)%2;
  }
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
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if(ADV_CONTROLS == 1)
      world->Emit(back_camera.towards);
    else
      world->Emit(view_camera.towards);
  }
}

void MouseEditInput(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    Bubble *b = world->CreateBubble();
    b->v = R3null_vector;
    b->pos = world->PlayerPosition();
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    Bubble *enemy = world->CreateEnemyBubble();
    enemy->v = R3null_vector;
    enemy->pos = world->PlayerPosition();
  }
}

void MouseMovement(int x, int y) {
  int dx = x - window_width/2;
  int dy = y - window_height/2;
  if(dx == 0 && dy == 0) return;
    
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
  
  //
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

void LoadTexture(const char *filename, GLuint *tex, int width, int height, GLuint type) {
  ifstream texture_file (filename, ios::in | ios::binary | ios::ate);
  int texture_file_size = texture_file.tellg();
  texture_file.seekg(0, ios::beg);
  char *texture_source = (char *)malloc(texture_file_size);
  texture_file.read(texture_source, texture_file_size);
  glGenTextures(1, tex);
  glBindTexture(GL_TEXTURE_2D, *tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, type, GL_UNSIGNED_BYTE, texture_source);
  texture_file.close();
  glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadCubemap(const char *filename, GLuint *tex, int width, int height) {
  ifstream texture_file (filename, ios::in | ios::binary | ios::ate);
  int texture_file_size = texture_file.tellg();
  texture_file.seekg(0, ios::beg);
  char *texture_source = (char *)malloc(texture_file_size);
  texture_file.read(texture_source, texture_file_size);
  glGenTextures(1, tex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *tex);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  //Define all 6 faces
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_source);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_source);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_source);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_source);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_source);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_source);
  
  texture_file.close();
  glBindTexture(GL_TEXTURE_2D, 0);
}

int CreateGameWindow(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(window_width, window_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_MULTISAMPLE); // | GLUT_STENCIL
#if defined(__APPLE__)
  CGSetLocalEventsSuppressionInterval(0.0);
#endif
  glutCreateWindow("Game");
  
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

  glutWarpPointer(window_width/2, window_height/2);
  glutSetCursor(GLUT_CURSOR_NONE);
  glutIdleFunc(IdleLoop);
  GoBackToMenu();
    
  world = NULL;

  if(GLEW_ARB_framebuffer_object && GLEW_ARB_fragment_program) { hasgoodgpu = 1;}
  config[3] = hasgoodgpu;
  //hasgoodgpu = 0;
  if(hasgoodgpu) {
    direct_render_fbo = new Framebuffer(window_width, window_height);
    processing_fbo1 = new Framebuffer(window_width, window_height);
    processing_fbo2 = new Framebuffer(window_width, window_height);
    blur_shader_x = new Shader("blur-x");
    blur_shader_y = new Shader("blur-y");
    bloom_preblur_shader = new Shader("bloompreblur");
    bloom_composite_shader = new Shader("bloomcomposite");
    world_cubemap_shader = new Shader("worldcubemap");
    bump_shader = new Shader("bump");
    laser_shader = new Shader("laser");
    LoadCubemap("./textures/stars.rgb", &world_texture, 1024, 1024);
    LoadCubemap("./textures/ball.rgb", &bubble_texture, 512, 512);
    LoadCubemap("./textures/ball_bw2.rgb", &bubble_texture2, 512, 512);
  }
  
  LoadTexture("./textures/stars.rgb", &world_texture_2d, 1024, 1024, GL_RGB);
  LoadTexture("./textures/particle.rgba", &particle_sprite, 128, 128, GL_RGBA);
  LoadTexture("./textures/menu.rgb", &menu_texture, 1024, 1024, GL_RGB);
  LoadTexture("./textures/menu_on.rgb", &menu_on_texture, 32, 32, GL_RGB);
  LoadTexture("./textures/menu_off.rgb", &menu_off_texture, 32, 32, GL_RGB);
  LoadTexture("./textures/menu_ball.rgb", &menu_ball_texture, 64, 64, GL_RGB);
  LoadTexture("./textures/menu_low.rgb", &menu_low_texture, 32, 32, GL_RGB);
  LoadTexture("./textures/menu_high.rgb", &menu_high_texture, 32, 32, GL_RGB);
  
  cout << glGetString(GL_VERSION) << endl;
  
  return 0;
}

void Reset(int status) {
  if (world != NULL) {
    delete world;
  }
  world = new World();
  world->LoadMeshes();
  
  view_camera.eye = R3Point(0,0,-4);
  view_camera.yfov = 0.8;
  view_camera.xfov = 0.8;
  view_camera.up = R3Vector(0, 1, 0);
  view_camera.right = R3Vector(-1, 0, 0);
  view_camera.towards = R3Vector(0,0,1);
  back_camera = view_camera;
  
  hasgoodgpu = config[3];
  world->level_of_detail = config[4];
  world->trails_enabled = config[5];
  world->powerups_enabled = config[2];
  world->num_enemies = config[1];
  world->num_bubbles = (config[0] == 1 ? 400: 100);
  world->world_status = status;

  if (status == -1) {
    world->GenerateEmptyLevel();
  } else {
    world->GenerateRandomLevel();
  }
  
  glutDisplayFunc(RedrawWindow);
  if (status == -1) {
    glutKeyboardFunc(KeyboardEditInput);
    glutMouseFunc(MouseEditInput);
  } else {
    glutKeyboardFunc(KeyboardInput);
    glutMouseFunc(MouseInput);
  }
  glutPassiveMotionFunc(MouseMovement);
  glutSpecialFunc(SpecialInput);
  glutWarpPointer(window_width/2, window_height/2);
}

void GoBackToMenu() {
  glutDisplayFunc(RedrawMenu);
  glutKeyboardFunc(KeyboardMenuInput);
  glutMouseFunc(NULL);
  glutPassiveMotionFunc(NULL);
  glutSpecialFunc(SpecialMenuInput);
}
