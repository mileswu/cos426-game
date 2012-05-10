#include "bubble.h"
#include "gl.h"
#include <iostream>
using namespace std;

Bubble::Bubble() {
  //Initialize sane defaults
  density = 1.0;
  player_id = -1;
  pos = R3Point(0,0,0);
  v = R3Vector(0,0,0);
  size = 1;
}

double Bubble::Mass() {
  return 4.0/3.0*M_PI*pow(size,3.0)*density;
}

void Bubble::SetSizeFromMass(double mass) {
  size = pow(3.0*mass/4.0/M_PI/density, 1.0/3.0);
}

int Bubble::Collides(Bubble *otherbubble) {
  double d = (otherbubble->pos - pos).Length(); //distance between centers
  
  if(d >= otherbubble->size + size)
    return 0; // No collision
  
  double v_total = Mass() + otherbubble->Mass();
  Bubble *bigger, *smaller;
  int bigger_this; //Is 'this' bigger? or smaller?
  if(size > otherbubble->size) {
    bigger_this = 1;
    bigger = this;
    smaller = otherbubble;
  }
  else {
    bigger_this = 0;
    smaller = this;
    bigger = otherbubble;
  }
  
  double bigger_mass_upperbound = v_total;
  double bigger_mass_lowerbound = bigger->Mass();
  
  // See if smaller one is totally absorbed
  bigger->SetSizeFromMass(v_total);
  if(d < bigger->size) {
    if(bigger_this) {
      return -2; //The other one was absorbed
    }
    else {
      return -1; //'this' was the smaller one that was absorbed
    }
  }
  
  // Midpoint bisection to find solution
  for(int i=0;i<100;i++) { //100 iterations probably sufficient to find solution
    double bigger_mass_midpoint = 0.5 * (bigger_mass_upperbound + bigger_mass_lowerbound);
    bigger->SetSizeFromMass(bigger_mass_midpoint);
    smaller->SetSizeFromMass(v_total - bigger_mass_midpoint);
    if(d < bigger->size + smaller->size) {
      bigger_mass_lowerbound = bigger_mass_midpoint;
    }
    else {
      bigger_mass_upperbound = bigger_mass_midpoint;
    }
  }
  
  return 1;
}

void Bubble::Draw() {
  glPushMatrix();
  glTranslated(pos[0], pos[1], pos[2]);
  static GLUquadricObj *glu_sphere = gluNewQuadric();
  gluQuadricTexture(glu_sphere, GL_TRUE);
  gluQuadricNormals(glu_sphere, (GLenum) GLU_SMOOTH);
  gluQuadricDrawStyle(glu_sphere, (GLenum) GLU_FILL);
  gluSphere(glu_sphere, size, 32, 32);
  glPopMatrix();
}
