#ifndef BUBBLE_H
#define BUBBLE_H

#include "geometry.h"
#include <vector>
using namespace std;

class AI;

typedef enum {
  invincible_state,
  small_sink_state,
  sink_state,
  speed_up_state,
  slow_down_state,
  reg_state,
} BubbleState;

struct BubbleMaterial {
  GLfloat color[4];
};

class Bubble {
  public:
    explicit Bubble(AI *ai_);
    ~Bubble();
    
    void Draw();

    double Mass();
    void SetSizeFromMass(double mass);
    int Collides(Bubble *otherbubble);
		int Collides(R3Mesh* mesh);

    R3Point pos;
    R3Vector v;
    R3Vector a;
    double size;
    double density;
    int player_id;

    BubbleState state;
    double effect_end_time;

    BubbleMaterial material;
    AI *ai;

  private:
  
};

#endif
