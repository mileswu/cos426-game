#include <vector>
#include "bubble.h"
#include <sys/time.h>
#include <string>

class World {
  public:
    World();
    void Draw(R3Camera camera);
    void Simulate();
    void Emit(R3Vector camera_direction);
    
    R3Point PlayerPosition();
    std::string PlayerStatus();
    
  private:
    std::vector<Bubble *> bubbles;
    struct timeval lasttime_updated;
  
};
