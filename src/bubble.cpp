#include "bubble.h"
#include "gl.h"

Bubble::Bubble() {
    
}

double Bubble::Mass() {
  return mass;
}

void Bubble::SetSizeFromMass(double mass) {
  this->mass = mass;
  size = pow(mass, 1.0/3.0);
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
