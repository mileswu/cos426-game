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
  
  R3Vector total_momentum = bigger->Mass() * bigger->v + smaller->Mass() * smaller->v;
  
  double bigger_mass_upperbound = v_total;
  double bigger_mass_lowerbound = bigger->Mass();
  
  // See if smaller one is totally absorbed
  bigger->SetSizeFromMass(v_total);
  if(d < bigger->size) {
    bigger->v = total_momentum/bigger->Mass();
    
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
  
  bigger->v = (total_momentum - smaller->Mass() * smaller->v)/bigger->Mass();
  
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


bool Bubble::inView(R3Camera camera) {

/*  R3Point upright_point, upleft_point, bottomleft_point, bottomright_point;
  R3Vector upright_v, upleft_v, bottomleft_v, bottomright_v;

  upright_v = camera.towards + tan(camera.xfov)*camera.right + tan(camera.yfov)*camera.up;
  bottomleft_v = camera.towards - tan(camera.xfov)*camera.right - tan(camera.yfov)*camera.up;
  upleft_v = camera.towards - tan(camera.xfov)*camera.right + tan(camera.yfov)*camera.up;
  bottomright_v = camera.towards + tan(camera.xfov)*camera.right - tan(camera.yfov)*camera.up;

  upright_point = camera.eye + upright_v;
  bottomleft_point = camera.eye + bottomleft_v;
  upleft_point = camera.eye + upleft_v;
  bottomright_point = camera.eye + bottomright_v;

  R3Plane right = R3Plane(camera.eye, upright_point, bottomright_point);
  R3Plane left = R3Plane(camera.eye, upleft_point, bottomleft_point);
  R3Plane top = R3Plane(camera.eye, upright_point, upleft_point);
  R3Plane bottom = R3Plane(camera.eye, bottomright_point, bottomleft_point);
  R3Plane camera_plane = R3Plane(upright_point, upleft_point, bottomright_point);

  R3Vector rn = right.Normal();
  R3Vector ln = left.Normal();
  R3Vector tn = top.Normal();
  R3Vector bn = bottom.Normal();
  R3Vector cn = camera_plane.Normal();

  if (rn.Dot(camera.right) < 0) right.Flip();
  if (ln.Dot(camera.right) > 0) left.Flip();
  if (tn.Dot(camera.up) < 0) top.Flip();
  if (bn.Dot(camera.up) > 0) bottom.Flip();
  if (cn.Dot(camera.towards) < 0) camera_plane.Flip();
*/
  double dist;
  dist = R3SignedDistance(camera.right_plane, pos);
  if (dist > 0 && !(fabs(dist) < size)) return false;
  dist = R3SignedDistance(camera.left_plane, pos);
  if (dist > 0 && !(fabs(dist) < size)) return false;
  dist = R3SignedDistance(camera.top_plane, pos);
  if (dist > 0 && !(fabs(dist) < size)) return false;
  dist = R3SignedDistance(camera.bottom_plane, pos);
  if (dist > 0 && !(fabs(dist) < size)) return false;


  if (R3SignedDistance(camera.camera_plane, pos) < 0) return false;

  return true;

}
