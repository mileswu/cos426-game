#include <vector>
#include "bubble.h"
#include <sys/time.h>

class World {
  public:
    World();
    void Draw();
    void Simulate();
    void Emit(R3Vector camera_direction);
    
    R3Point PlayerPosition();
    void PrintPlayerStatus();
    
  private:
    std::vector<Bubble *> bubbles;
    struct timeval lasttime_updated;
  
};
