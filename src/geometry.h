#ifndef GEOMETRY_H
#define GEOMETRY_H

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

    R3Plane right_plane, left_plane, top_plane, bottom_plane, camera_plane;
    
    void CalcPlanes();
    void Load(int width, int height);
};

#endif
