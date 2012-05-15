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
    : state(0),
      rate(0.0),
      world(NULL),
      self(NULL),
      target(NULL) { }
  ~AI() { }

  // Bubble does whatever bubble is told.
  void ActFromState();

  virtual void Idle() = 0;
  virtual void Seek() = 0;
  virtual void Aggress() = 0;
  virtual void Avoid() = 0;

  int state;
  double rate;
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
