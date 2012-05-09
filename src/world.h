#include <vector>
#include "bubble.h"
#include <sys/time.h>

class World {
  public:
    World();
    void Draw();
    void Simulate();
    void Emit(R3Vector camera_direction);
    
  private:
    std::vector<Bubble *> bubbles;
    struct timeval lasttime_updated;
  
};
