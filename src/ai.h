#ifndef AI_H
#define AI_H

#include "bubble.h"
#include "geometry.h"
#include "world.h"


// Each bubble owns an instance of AI (could be NULL).
class AI {
public:
  enum State {
    kIdle = 0,
    kSeek,
    kAggress,
    kAvoid,
    kNumStates,
  };

  AI()
    : state(0), world(NULL), self(NULL), target(NULL) { }
  ~AI() { }

  // Bubble has to call AI->Get_ rather than AI modifying bubble state.
  // FIXME action instead of acceleration?
  //R3Vector &GetAcceleration() { return a; }

  // Bubble does whatever its current AI state tells it to.
  void ActFromState();

  virtual void Idle() = 0;
  virtual void Seek() = 0;
  virtual void Aggress() = 0;
  virtual void Avoid() = 0;

  unsigned state;
  World *world;
  Bubble *self;
  Bubble *target;
};


// A stupid AI that just floats around and obeys the world physics.
// Also known as `apathetic AI'.
class NullAI : public AI {
public:
  void Idle();
  void Seek();
  void Aggress();
  void Avoid();
};


// Enemy is a broad term, basically it chases and attacks its target.
class EnemyAI : public AI {
public:
  void Idle();
  void Seek();
  void Aggress();
  void Avoid();
};


// This cell is in a group of boid-like cells.
class SwarmAI : public AI {
public:
  void Idle();
  void Seek();
  void Aggress();
  void Avoid();
};


#endif
