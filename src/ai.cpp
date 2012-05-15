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
  // If we're bigger than the player, plus with the needed
  // mass to get to the player, then enter attack mode.
  double fuel = self->Mass() * World::emission_sizefactor
                             / (1.0 - World::emission_sizefactor);
  if (self->Mass() > world->bubbles[0]->Mass() + fuel) {
    state = kAggress;
    delay = 1.0;
    target = world->bubbles[0];
    return;
  }

  state = kIdle;
  delay = 2.0;

  // If the target is too far or too big, then choose a new one.
  if (NULL != target) {
    if (self->Mass() < target->Mass() + fuel) {
      target = NULL;
    }
    R3Vector distance = target->pos - self->pos;
    double cutoff_dist = World::world_size * 0.25;
    if (cutoff_dist < distance.Length()) {
      target = NULL;
    }
  }

  // Find the bubbles that are smaller than us,
  // not too small, larger than emitted bubble mass,
  // and close enough.
  if (NULL == target) {
    //vector<Bubble *> smaller_bubbles;
    Bubble *largest_bubble = NULL;
    double largest_mass = 0.0;
    double cutoff_dist = World::world_size * 0.1;
    for (vector<Bubble *>::iterator it = world->bubbles.begin(),
        ie = world->bubbles.end(); it != ie; ++it) {
      if ((*it)->Mass() < self->Mass() - fuel &&
          //(*it)->Mass() > 0.5 * self->Mass() &&
          (*it)->Mass() > self->Mass() * World::emission_sizefactor) {
        //smaller_bubbles.push_back(*it);
        R3Vector distance = (*it)->pos - self->pos;
        if (largest_mass < (*it)->Mass() &&
            cutoff_dist > distance.Length()) {
          largest_bubble = *it;
          largest_mass = (*it)->Mass();
        }
      }
    }
    //if (smaller_bubbles.size() == 0) {
    //  return;
    //}
    target = largest_bubble;
  }
  if (NULL == target) {
    return;
  }

  // Pick the closest one.
  // TODO Could also use some function of closeness and largeness.
  /*Bubble *closest_bubble = NULL;
  double closest_dist = INFINITY;
  for (vector<Bubble *>::iterator it = smaller_bubbles.begin(),
       ie = smaller_bubbles.end(); it != ie; ++it) {
    R3Vector distance((*it)->pos - self->pos);
    if (closest_dist > distance.Length()) {
      closest_bubble = *it;
      closest_dist = distance.Length();
    }
  }*/

  // Go toward the bubble.
  R3Vector direction;
  direction = target->pos /*+ 0.3 * target->size * target->v*/ - self->pos;
  direction.Normalize();

  // If we're nearly in the right direction, don't change course.
  // Otherwise, emit a propulsion bubble.
  R3Vector heading = self->v;
  heading.Normalize();
  double theta = acos(heading.Dot(direction));
  if (theta > 0.2) {
    world->EmitAtBubble(self, direction);
  }
}

void EnemyAI::Aggress() {
  // If we're smaller than the player, enter idle mode.
  double fuel = self->Mass() * World::emission_sizefactor
                             / (1.0 - World::emission_sizefactor);
  if (self->Mass() < world->bubbles[0]->Mass() + fuel) {
    state = kIdle;
    delay = 1.0;
    target = NULL;
    return;
  }

  state = kAggress;
  delay = 1.0;

  // Path toward the target and shoot stuff at it.
  R3Vector direction = world->bubbles[0]->pos /*+ 0.3 * world->bubbles[0]->size * world->bubbles[0]->v*/ - self->pos;
  direction.Normalize();

  // If we're nearly in the right direction, don't change course.
  // Otherwise, emit a propulsion bubble.
  R3Vector heading = self->v;
  heading.Normalize();
  double theta = acos(heading.Dot(direction));
  if (theta > 0.2) {
    world->EmitAtBubble(self, direction);
  }
}
