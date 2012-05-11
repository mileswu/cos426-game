#include "geometry.h"
#include "gl.h"

void R3Camera::Load(int width, int height) {
  // Set projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(2*180.0*yfov/M_PI, (GLdouble) width /(GLdouble) height, 0.01, 10000);

  // Set camera transformation
  R3Vector t = -(towards);
  R3Vector& u = up;
  R3Vector& r = right;
  GLdouble camera_matrix[16] = { r[0], u[0], t[0], 0, r[1], u[1], t[1], 0, r[2], u[2], t[2], 0, 0, 0, 0, 1 };
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixd(camera_matrix);
  glTranslated(-(eye[0]), -(eye[1]), -(eye[2]));
}
