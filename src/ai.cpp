#include "ai.h"
#include "bubble.h"
#include "geometry.h"
#include "world.h"
#include <vector>
using namespace std;

void AI::ActFromState() {
  //printf("AI working\n");
  switch (state) {
  case kIdle:     Idle();     break;
  case kAggress:  Aggress();  break;
  default:                    return;
  }
}

void NullAI::Idle() {
  // Silence is golden.
}

void NullAI::Aggress() {
  // Silence is golden.
}

void EnemyAI::Idle() {
  // If we're bigger than the player, enter attack mode.
  if (self->Mass() > world->bubbles[0]->Mass()) {
    state = kAggress;
    return;
  }

  state = kIdle;
  delay = 5.0;

  // Find the bubbles that are smaller than us and
  // larger than emitted bubble mass.
  vector<Bubble *> smaller_bubbles;
  for (vector<Bubble *>::iterator it = world->bubbles.begin(),
       ie = world->bubbles.end(); it != ie; ++it) {
    if ((*it)->Mass() < self->Mass() &&
        (*it)->Mass() > self->Mass() * World::emission_sizefactor) {
      smaller_bubbles.push_back(*it);
    }
  }

  if (smaller_bubbles.size() == 0) {
    return;
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
  //R3Vector current_a;
  //R3Vector current_v;
  R3Vector direction;
  //current_a = self->a;
  //current_a.Normalize();
  //current_v = self->v;
  //current_v.Normalize();
  direction = closest_bubble->pos - self->pos;
  direction.Normalize();
  //a = d - a - v;
  //a.Normalize();
  world->EmitAtBubble(self, -direction);
}

void EnemyAI::Aggress() {
  // If we're smaller than the player, enter idle mode.
  if (self->Mass() < target->Mass()) {
    state = kIdle;
    return;
  }

  state = kAggress;
  delay = 2.0;

  // Path toward the target and shoot stuff at it.
  R3Vector direction = target->pos - self->pos;
  direction.Normalize();
  //world->EmitAtBubble(self, direction);
}

void SwarmAI::Idle() {
  // Silence is golden.
}

void SwarmAI::Aggress() {
  // Silence is golden.
}
