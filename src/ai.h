#ifndef __AI_H__
#define __AI_H__


class Bubble;
class World;


// Each bubble owns an instance of AI (could be NULL).
class AI {
public:
  enum State {
    kIdle,
    kSeek,
    kAggress,
    kAvoid,
    kNumStates,
  };

  AI(World *world_, Bubble *self_)
    : state(0), world(world_), self(self_), target(NULL) { }

  // Each bubble gets up to one target.
  void Target(Bubble *target);

  // Bubble does whatever its current AI state tells it to.
  void ActFromState();

  // Depending on the world and target, the bubble can change goals.
  //virtual void UpdateState();

  virtual void Idle();
  virtual void Seek();
  virtual void Aggress();
  virtual void Avoid();

private:
  // Do pathing by implicitly gridding the world into unit cells.
  void UpdatePath();

  unsigned state;
  World *world;
  Bubble *self;
  Bubble *target;
};


// A stupid AI that just floats around and obeys the world physics.
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
