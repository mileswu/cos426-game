#include <vector>
#include "bubble.h"
#include <sys/time.h>
#include <string>

typedef enum {
	invincible_type,
	small_sink_type,
	sink_type,
	speed_up_type,
	slow_down_type,
} PowerUpType;

struct PowerUpShape {
	PowerUpType type;
	R3Mesh *mesh;
};

class World {
  public:
    World();
	  void CreatePowerUp(PowerUpType type);
    void Draw();
    void Simulate();
    void Emit(R3Vector camera_direction);
    
    R3Point PlayerPosition();
    std::string PlayerStatus();
    
  private:
    std::vector<Bubble *> bubbles;
		std::vector<PowerUpShape> power_ups;
    struct timeval lasttime_updated;
  
};
