#include "bubble.h"
#include "ai.h"
#include "gl.h"
#include "world.h"
#include <iostream>
using namespace std;

BubbleMaterial Bubble::default_material = {
  {1, 1, 1, 1},
  false,
  0,
  0,
  {25.0/255.0, 220.0/255.0, 250.0/255.0, 1},
};

BubbleMaterial Bubble::player_material = {
  {0, 0, 1, 1},
  true,
  3000,
  5,
  {25.0/255.0, 220.0/255.0, 250.0/255.0, 1},
};

BubbleMaterial Bubble::neutral_material = {
  {0, 1, 0, 1},
  false,
  0,
  0,
  {25.0/255.0, 220.0/255.0, 250.0/255.0, 1},
};

BubbleMaterial Bubble::enemy_material = {
  {1, 1, 0, 1},
  true,
  3000,
  5,
  {25.0/255.0, 220.0/255.0, 250.0/255.0, 1},
};

/*static BubbleMaterial player_material = {
  {0, 0, 1, 1},
};

static BubbleMaterial other_material = {
  {0, 1, 1, 1},
};*/

Bubble::Bubble() {
  //Initialize sane defaults
  density = 1.0;
  player_id = -1;
  pos = R3Point(0,0,0);
  v = R3Vector(0,0,0);
  size = 1;

  state = reg_state;
  effect_end_time = -1;

  material = &default_material;
  ai = NULL;
}

Bubble::~Bubble() {
  if (NULL != ai) {
    delete ai;
  }
}

double Bubble::Mass() {
  return 4.0/3.0*M_PI*pow(size,3.0)*density;
}

void Bubble::SetSizeFromMass(double mass) {
  size = pow(3.0*mass/4.0/M_PI/density, 1.0/3.0);
}

int Bubble::Collides(R3Mesh* mesh, R3Vector offset) {
  R3Box box = mesh -> bbox;

  R3Vector SepAxis = pos - (box.Centroid() + offset);

  double dist = SepAxis.Length();
  SepAxis.Normalize();

  double x = SepAxis.X();
  double y = SepAxis.Y();
  double z = SepAxis.Z();

  if (x >= y && x >= z && x != 0)
    SepAxis /= x;
  else if (y >= x && y >= z != 0)
    SepAxis /= y;
  else if (z != 0)
    SepAxis /= z;

  double x_len = box.XLength();
  double y_len = box.YLength();
  double z_len = box.ZLength();

  //effective radius
  SepAxis.SetX(x * x_len/2.0);
  SepAxis.SetY(y * y_len/2.0);
  SepAxis.SetZ(z * z_len/2.0);

  if (dist <= (size + SepAxis.Length()))
    return 1;	
  return 0;
}

int Bubble::Collides(Bubble *otherbubble) {
  double d = (otherbubble->pos - pos).Length(); //distance between centers
  
  if(d >= otherbubble->size + size)
    return 0; // No collision
  
  double v_total = Mass() + otherbubble->Mass();
  Bubble *bigger, *smaller;
  int bigger_this; //Is 'this' bigger? or smaller?
  if (size > otherbubble->size || (otherbubble->state == invincible_state && player_id == 0)) {
    bigger_this = 1;
    bigger = this;
    smaller = otherbubble;
  } else {
    bigger_this = 0;
    smaller = this;
    bigger = otherbubble;
  }
  
  R3Vector total_momentum = bigger->Mass() * bigger->v + smaller->Mass() * smaller->v;
  
  double bigger_mass_upperbound = v_total;
  double bigger_mass_lowerbound = bigger->Mass();
  
  // See if smaller one is totally absorbed
  bigger->SetSizeFromMass(v_total);
  if (d < bigger->size) {
    bigger->v = total_momentum/bigger->Mass();
    
    if (bigger_this) {
      return -2; //The other one was absorbed
    } else {
      return -1; //'this' was the smaller one that was absorbed
    }
  }
  
  // Midpoint bisection to find solution
  for (int i=0;i<100;i++) { //100 iterations probably sufficient to find solution
    double bigger_mass_midpoint = 0.5 * (bigger_mass_upperbound + bigger_mass_lowerbound);
    bigger->SetSizeFromMass(bigger_mass_midpoint);
    smaller->SetSizeFromMass(v_total - bigger_mass_midpoint);
    if (d < bigger->size + smaller->size) {
      bigger_mass_lowerbound = bigger_mass_midpoint;
    } else {
      bigger_mass_upperbound = bigger_mass_midpoint;
    }
  }
  
  bigger->v = (total_momentum - smaller->Mass() * smaller->v)/bigger->Mass();

  // If either bubble has an AI and one bubble is the other's target,
  // we need it to change targets pronto.
  if (ai != NULL && ai->target == otherbubble) {
    ai->target = NULL;
  }
  if (otherbubble->ai != NULL && otherbubble->ai->target == this) {
    otherbubble->ai->target = NULL;
  }

  return 1;
}

void Bubble::Draw(int lod = 0) {
  // Draw the bubble itself.
  glPushMatrix();
  glTranslated(pos[0], pos[1], pos[2]);
  static GLUquadricObj *glu_sphere = gluNewQuadric();
  gluQuadricTexture(glu_sphere, GL_TRUE);
  gluQuadricNormals(glu_sphere, (GLenum) GLU_SMOOTH);
  gluQuadricDrawStyle(glu_sphere, (GLenum) GLU_FILL);
  if(lod == 0)
    gluSphere(glu_sphere, size, 32, 32);
  else
    gluSphere(glu_sphere, size, 6, 6);
  glPopMatrix();
}
