#include <vector>
#include "geometry.h"

class Bubble {
  public:
    Bubble();
    R3Point pos;
    R3Vector v;
    R3Vector a;
    double size;
    void Draw();
    
    double Mass();
    void SetSizeFromMass(double mass);
    
    
  private:
  
};
