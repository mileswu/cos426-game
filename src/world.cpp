#include "world.h"
#include "gl.h"
#include <iostream>
using namespace std;

World::World() {
  cout << "Initializing World" << endl;
  for(int i=0; i<1; i++) {
    Bubble *b = new Bubble();
    b->pos = R3Point(0,0,0);
    b->v = R3Vector(0,0,0);
    b->size = 1;
    bubbles.push_back(b);
  }
  Bubble *b = new Bubble();
  b->pos = R3Point(0,0,1);
  b->v = R3Vector(0,0,0);
  b->size = 0.4;
  bubbles.push_back(b);
  
}

void World::Emit(R3Vector camera_direction) {
  
  Bubble *b = bubbles[0];
  R3Vector orig_v = b->v;
  
  camera_direction.Normalize();
  b->v += 0.1*camera_direction;
  
  Bubble *b2 = new Bubble();
  b2->pos = b->pos;
  b2->v = orig_v - 0.6*camera_direction;
  b2->size = 0.1;
  bubbles.push_back(b2);
}

void World::Simulate() {
  struct timeval curtime;
  gettimeofday(&curtime, NULL);
  double timestep = (curtime.tv_sec - lasttime_updated.tv_sec) + 1.0E-6F * (curtime.tv_usec - lasttime_updated.tv_usec);
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
    
    for(vector<Bubble *>::iterator it2=bubbles.begin(); it2 < bubbles.end(); it2++) {
      if(it == it2) continue;
      // Check for collisions
    }
  }
}

void World::Draw() {
  Simulate();
  
  glEnable(GL_LIGHTING);
  GLfloat c[4];
  c[0] = 1; c[1] = 0; c[2] = 0; c[3] = 1;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
  
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {
    (*it)->Draw();
  }
  glDisable(GL_LIGHTING);
}