#include "bubble.h"
#include "ai.h"


void AI::Target(Bubble *target_)
{
  target = target_;
  ActFromState();
}


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
  // Silence is golden.
}


void EnemyAI::Seek()
{
  // Silence is golden.
}


void EnemyAI::Aggress()
{
  // Silence is golden.
}


void EnemyAI::Avoid()
{
  // Silence is golden.
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

