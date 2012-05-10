#include "world.h"
#include "gl.h"
#include <iostream>
#include <map>

using namespace std;

static double emission_speed = 5.0;
static double emission_sizefactor = 0.05;

World::World() {
  for(int i=0; i<1; i++) {
    Bubble *b = new Bubble();
    b->pos = R3Point(0,0,0);
    b->v = R3Vector(0,0,0);
    b->size = 1;
    bubbles.push_back(b);
  }
  Bubble *b = new Bubble();
  b->pos = R3Point(0,0,3);
  b->v = R3Vector(0,0,0);
  b->size = 0.4;
  bubbles.push_back(b);
  
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

void World::PrintPlayerStatus() {
  cout << "Player size: " << bubbles[0]->size << endl;
  cout << "Bubbles left (excluding yourself): " << bubbles.size() - 1 << endl;
  double max_size = 0;
  for(vector<Bubble *>::iterator it=bubbles.begin()+1; it < bubbles.end(); it++) {
    if((*it)->size > max_size) {
      max_size = (*it)->size;
    }
  }
  cout << "Largest bubble size (excluding yourself): " << max_size << endl;
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
  }
  
  // Collisions
  map< pair<Bubble *, Bubble *>, int > collisionpairs;
  for(vector<Bubble *>::iterator it=bubbles.begin(); it < bubbles.end(); it++) {  
    for(vector<Bubble *>::iterator it2=bubbles.begin(); it2 < bubbles.end(); it2++) {
      if(it == it2) continue;
      //Prevent pairs being considered bothways round
      if(collisionpairs.count(make_pair(*it, *it2)) == 1) continue;
      
      int retval = (*it)->Collides(*it2);
      if(retval == -1) {
        it = bubbles.erase(it);
      }
      else if(retval == -2) {
        it2 = bubbles.erase(it2);  
      } else {
        collisionpairs[make_pair(*it, *it2)] = 1;
        collisionpairs[make_pair(*it2, *it)] = 1;
      }
    }
  }
}

void World::Draw() {  
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