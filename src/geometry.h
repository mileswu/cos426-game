#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "R3/R3.h"

class R3Camera {
  public:
    R3Point eye;
    R3Vector towards;
    R3Vector right;
    R3Vector up;
    double xfov, yfov;
    double neardist, fardist;
    
    void Load(int width, int height);
};