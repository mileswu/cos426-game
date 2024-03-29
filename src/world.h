#ifndef WORLD_H
#define WORLD_H

#include "bubble.h"
#include "shader.h"
#include <sys/time.h>
#include <string>
#include <vector>

typedef enum {
  DEATH_SOUND,
  ABSORBING_SOUND,
  ALERT_SOUND,
  INVINCIBLE_SOUND,
  SINK_SOUND,
  SPEED_UP_SOUND,
} SoundType;

typedef enum {
  invincible_type,
  small_sink_type,
  sink_type,
  speed_up_type,
  slow_down_type,
} PowerUpType;

struct PowerUpShape {
  PowerUpType type;
  double die_time;
  R3Mesh *mesh[2];
  R3Point Center;
};

struct Particle {
  Bubble *parent;
  GLfloat color[4];
  R3Vector velocity;
  R3Point position;
  double lifetime;
  bool is_point;
  int point_size;
};

struct World {
  World();

  void DrawMinimap();
  void Draw(R3Camera camera, Shader *bump_shader, int occlusion);
  void DrawTrails(R3Camera camera);
  void DrawWorld(R3Camera camera);
  void DrawOverlay();
  void DrawPowerups(R3Camera camera);

  void CreatePowerUp(PowerUpType type);
  void RemovePowerUp(int index);

  Bubble *CreateBubble();
  Bubble *CreatePlayerBubble();
  Bubble *CreateEnemyBubble();
  void GenerateEmptyLevel();
  void GenerateRandomLevel();
  void LoadMeshes();

  void Timestep();
  void SimulateParticles();
  void SimulateAI();
  void SimulatePowerups();
  void SimulateMotion(bool player_only);
  void SimulateCollisions();
  void Simulate();
  void EmitAtBubble(Bubble *b, R3Vector direction);
  void Emit(R3Vector camera_direction);
  bool InView(R3Camera camera, R3Point pos, double radius);
  void PlayMusic(SoundType type);

  void SetPlayerVelocity(R3Vector v);
  void SetPlayerPosition(R3Point p);
  R3Point PlayerPosition();
  R3Vector PlayerDirection();
  double PlayerSize();
  std::string PlayerStatus();
  
  std::vector<Bubble *> bubbles;
  std::vector<PowerUpShape> power_ups;
  std::vector<Particle *> particles;
  struct timeval lasttime_updated;
  int world_status;

  double curr_time;
  double last_time;
  double timestep;

  static double emission_speed;
  static double emission_sizefactor;
  static const double world_size;
  
  int level_of_detail;
  int trails_enabled;
  int powerups_enabled;
  int num_enemies;
  int num_bubbles;
  
  R3Mesh *invincible_mesh[2];
  R3Mesh *smallsink_mesh[2];
  R3Mesh *sink_mesh[2];
  R3Mesh *speedup_mesh[2];
  R3Mesh *slowdown_mesh[2];
};

#endif
