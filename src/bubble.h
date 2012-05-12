#include <vector>
#include "geometry.h"

typedef enum {
	invin_state,
	small_sink_state,
	sink_state,
	speed_up_state,
	slow_down_state,
} BubbleState;

class Bubble {
  public:
    Bubble();
    
    void Draw();

    double Mass();
    void SetSizeFromMass(double mass);
    int Collides(Bubble *otherbubble);
		int Collides(R3Mesh* mesh);

    R3Point pos;
    R3Vector v;
    R3Vector a;
    double size;
    double density;
    int player_id;
    
		BubbleState state;
		double effect_end_time;
  private:
  
};
