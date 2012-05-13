#include "ai.h"
#include "bubble.h"
#include "geometry.h"
#include "world.h"
#include <vector>
using namespace std;


void AI::ActFromState()
{
  switch (state) {
  case kIdle:     Idle();     break;
  case kSeek:     Seek();     break;
  case kAggress:  Aggress();  break;
  case kAvoid:    Avoid();    break;
  default:                    return;
  }
}


void NullAI::Idle()
{
  // Silence is golden.
}


void NullAI::Seek()
{
  // Silence is golden.
}


void NullAI::Aggress()
{
  // Silence is golden.
}


void NullAI::Avoid()
{
  // Silence is golden.
}


void EnemyAI::Idle()
{
  state = kIdle;

  // Find the bubbles that are smaller than us.
  vector<Bubble *> smaller_bubbles;
  for (vector<Bubble *>::iterator it = world->bubbles.begin(),
       ie = world->bubbles.end(); it != ie; ++it) {
    if (self->Mass() > (*it)->Mass()) {
      smaller_bubbles.push_back(*it);
    }
  }

  // Pick the closest one.
  // TODO Could also use some function of closeness and largeness.
  Bubble *closest_bubble = NULL;
  double closest_dist = INFINITY;
  for (vector<Bubble *>::iterator it = smaller_bubbles.begin(),
       ie = smaller_bubbles.end(); it != ie; ++it) {
    R3Vector distance((*it)->pos - self->pos);
    if (closest_dist > distance.Length()) {
      closest_bubble = *it;
      closest_dist = distance.Length();
    }
  }

  // Counter its current trajectory and go toward the bubble.
  // There are different ways to weight this.
  // Here, we just uniformly weight a, v, and d.
  // TODO but what we really want is for the NPC bubbles to shoot
  // their own bubble trails to accelerate.
  /*R3Vector current_a;
  R3Vector current_v;
  R3Vector direction;
  current_a = self->a;
  current_a.Normalize();
  current_v = self->v;
  current_v.Normalize();
  direction = closest_bubble->pos - self->pos;
  direction.Normalize();
  a = d - a - v;
  a.Normalize();*/
}


void EnemyAI::Seek()
{
  state = kSeek;

  // Path toward the target.
}


void EnemyAI::Aggress()
{
  state = kAggress;

  // Path toward the target and shoot stuff at it.

}


void EnemyAI::Avoid()
{
  state = kAvoid;

  // Path away from the target.

}


void SwarmAI::Idle()
{
  // Silence is golden.
}


void SwarmAI::Seek()
{
  // Silence is golden.
}


void SwarmAI::Aggress()
{
  // Silence is golden.
}


void SwarmAI::Avoid()
{
  // Silence is golden.
}

