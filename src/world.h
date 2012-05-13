#include <vector>
#include "bubble.h"
#include <sys/time.h>
#include <string>

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

class World {
  public:
    World();
    void DrawMinimap();
    void Draw(R3Camera camera);
	  void CreatePowerUp(PowerUpType type);
		void RemovePowerUp(int index);
    void Simulate();
    void Emit(R3Vector camera_direction);
    bool inView(R3Camera camera, R3Point pos, double radius);
    void PlayMusic(SoundType type);
    
    R3Point PlayerPosition();
    std::string PlayerStatus();
    
  private:
    std::vector<Bubble *> bubbles;
		std::vector<PowerUpShape> power_ups;
    struct timeval lasttime_updated;
  
};
