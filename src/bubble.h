#include <vector>
#include "geometry.h"

class Bubble {
  public:
    Bubble();
    
    void Draw();
    bool inView(R3Camera camera);

    double Mass();
    void SetSizeFromMass(double mass);
    int Collides(Bubble *otherbubble);

    R3Point pos;
    R3Vector v;
    R3Vector a;
    double size;
    double density;
    int player_id;
    
  private:
  
};
