#include "world.h"
#include "gl.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>

using namespace std;

static double emission_speed = 5.0;
static double emission_sizefactor = 0.05;

double rand(double max, int plusminus = 0, double min = 0) {
  double retval = ((double)rand())/RAND_MAX;
  retval *= max-min;
  retval += min;
  if(plusminus == 1 && rand()%2 == 0)
    retval *= -1;
  return retval;
}

R3Vector randvector(double max, double min = 0) {
  for(;;) {
    R3Vector retval(rand(max, 1), rand(max, 1), rand(max, 1));
    if(retval.Length() < max && retval.Length() > min)
      return retval;
  }
}


R3Point randpoint(double max, double min = 0) {
  return randvector(max, min).Point();
}


World::World() {
  //Player bubble
  Bubble *b = new Bubble();
  b->player_id = 0;
  bubbles.push_back(b);
  
  for(int i=0; i<200; i++) {
    b = new Bubble();
    b->pos = randpoint(30);
    b->v = randvector(0.1);
    b->size = rand(1.2, 0.1);
    bubbles.push_back(b);
  }
  
  // Initialize time
  lasttime_updated.tv_sec = 0;
  lasttime_updated.tv_usec = 0;

  
}

void World::Emit(R3Vector camera_direction) {
  camera_direction.Normalize();
  Bubble *b = bubbles[0];
  
  double total_mass = b->Mass();
  R3Vector total_momentum = b->Mass() * b->v;
  R3Vector orig_v = b->v;
  
  Bubble *b_emitted = new Bubble();
  bubbles.push_back(b_emitted);
  b_emitted->SetSizeFromMass(total_mass*emission_sizefactor);
  b->SetSizeFromMass(total_mass*(1-emission_sizefactor));
  
  b_emitted->pos = b->pos - camera_direction*(b->size + b_emitted->size);
  b_emitted->v = orig_v - emission_speed*camera_direction;

  b->v = (total_momentum - b_emitted->Mass()*b_emitted->v)/b->Mass();
}

R3Point World::PlayerPosition() {
  return bubbles[0]->pos;
}

string World::PlayerStatus() {
  stringstream ss;
  ss << "Player size: " << bubbles[0]->size << endl;
  ss << "Bubbles left (excluding yourself): " << bubbles.size() - 1 << endl;
  double max_size = 0;
  for(vector<Bubble *>::iterator it=bubbles.begin()+1; it < bubbles.end(); it++) {
    if((*it)->size > max_size) {
      max_size = (*it)->size;
    }
  }
  ss << "Largest bubble size (excluding yourself): " << max_size << endl;
  return ss.str();
}

void World::Simulate() {
  struct timeval curtime;
  double timestep = 0;
  gettimeofday(&curtime, NULL);
  if(lasttime_updated.tv_sec != 0) {
    timestep = (curtime.tv_sec - lasttime_updated.tv_sec) + 1.0E-6F * (curtime.tv_usec - lasttime_updated.tv_usec);
  }
  lasttime_updated = curtime;
  
  
  // A calculation
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {
    (*it)->a = R3Vector(0,0,0);
  }
  
  // V update
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {
    (*it)->v += (*it)->a*timestep;
  }
  
  // P update
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {
    (*it)->pos += (*it)->v*timestep;
  }
  
  // Collisions
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {  
    for(vector<Bubble *>::iterator it2=it+1; it2 < bubbles.end(); it2++) {      
      int retval = (*it)->Collides(*it2);
      if(retval == -1) {
        it = bubbles.erase(it);
      }
      else if(retval == -2) {
        it2 = bubbles.erase(it2);  
      }
    }
  }
}

void World::Draw() {  
  glEnable(GL_LIGHTING);
  int light_index = GL_LIGHT0 + 10;
  
  GLfloat c[4];
  double player_size = bubbles[0]->size;
  
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {
    if(it == bubbles.begin()) {
      c[0] = 0; c[1] = 0; c[2] = 1; c[3] = 1;
    }
    else {
      double s = (*it)->size;
      if(s < 0.8*player_size) {
        c[0] = 0; c[1] = 1; c[2] = 0; c[3] = 1;
      }
      else if(s > 1.2*player_size) {
        c[0] = 1; c[1] = 0; c[2] = 0; c[3] = 1;
      }
      else {
        double f = (s - 0.8*player_size)/(0.4*player_size);
        c[0] = f; c[1] = 1-f; c[2] = 0; c[3] = 1;
      }
    }
    /* Lighting idea
    if(0) {
    glDisable(light_index);
    glLightfv(light_index, GL_DIFFUSE, c);
    glLightfv(light_index, GL_SPECULAR, c);
    GLfloat gl_pos[4];
    gl_pos[0] = (*it)->pos[0]; gl_pos[1] = (*it)->pos[1]; gl_pos[2] = (*it)->pos[2]; gl_pos[3] = 0;
    glLightfv(light_index, GL_POSITION, gl_pos);
    glEnable(light_index);
    light_index++;
    }*/
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
    GLfloat shininess[1]; shininess[0] = 75;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    (*it)->Draw();
  }
  glDisable(GL_LIGHTING);
}