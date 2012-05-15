#ifndef WORLD_H
#define WORLD_H

#include "bubble.h"
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
  R3Mesh *mesh;
};

struct Particle {
  GLfloat color[4];
  R3Vector velocity;
  R3Point position;
  double lifetime;
  bool is_point;
  int point_size;
};

class World {
  public:
    World();

    void DrawMinimap();
    void Draw(R3Camera camera);
    void DrawTrails(R3Camera camera);
    void DrawWorld();
    void DrawOverlay();
    void DrawPowerups(R3Camera camera);
	  void CreatePowerUp(PowerUpType type);
		void RemovePowerUp(int index);

    void GenerateLevel();
    void Simulate();
    void EmitAtBubble(Bubble *b, R3Vector direction);
    void Emit(R3Vector camera_direction);
    bool InView(R3Camera camera, R3Point pos, double radius);
    void PlayMusic(SoundType type);
    
    R3Point PlayerPosition();
    double PlayerSize();
    std::string PlayerStatus();
    
    std::vector<Bubble *> bubbles;
    std::vector<PowerUpShape> power_ups;
    std::vector<Particle *> particles;
    struct timeval lasttime_updated;
    int world_status;
};

#endif
