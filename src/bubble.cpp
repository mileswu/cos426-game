#include "bubble.h"
#include "gl.h"
#include <iostream>
using namespace std;

Bubble::Bubble() {
    
}

double Bubble::Mass() {
  return pow(size, 3);
}

void Bubble::SetSizeFromMass(double mass) {
  size = pow(mass, 1.0/3.0);
}

int Bubble::Collides(Bubble *otherbubble) {
  double d = (otherbubble->pos - pos).Length(); //distance between centers
  
  if(d >= otherbubble->size + size)
    return 0; // No collision
  
  double v_total = Mass() + otherbubble->Mass();
  
  // Solve sphere equalization equation
  double quadratic_a = 3.0*d;
  double quadratic_b = -3.0*d*d;
  double quadratic_c = d*d*d - v_total;
  double new_size = (-quadratic_b + sqrt(quadratic_b*quadratic_b - 4.0*quadratic_a*quadratic_c))/2.0/quadratic_a;
    
  if(new_size < d - new_size ) { // Swap so larger is new_size
    new_size = d - new_size;
  }
  
  if(size > otherbubble->size) {
    size = new_size;
    otherbubble->size = d - new_size;
  }
  else {
    size = d - new_size;
    otherbubble->size = new_size;
  }
  
  /*cout << "d: " << d << " size: " << size + otherbubble->size << endl;
  cout << "mass b: " << v_total << " mass now: " << Mass() + otherbubble->Mass() << endl;*/
  
  if(otherbubble->size < 0) { //Other absorbed fully
    SetSizeFromMass(v_total);
    return -2;
  }
  else if(size < 0) { //Absorbed fully
    otherbubble->SetSizeFromMass(v_total);
    return -1;
  }
  else //Normal collision
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