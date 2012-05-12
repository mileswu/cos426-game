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

void R3Camera::CalcPlanes() {

  R3Point upright_point, upleft_point, bottomleft_point, bottomright_point;
  //R3Vector upright_v, upleft_v, bottomleft_v, bottomright_v;

  upright_point = (towards + tan(xfov)*right + tan(yfov)*up) + eye;
  bottomleft_point = (towards - tan(xfov)*right - tan(yfov)*up) + eye;
  upleft_point = (towards - tan(xfov)*right + tan(yfov)*up) + eye;
  bottomright_point = (towards + tan(xfov)*right - tan(yfov)*up) + eye;

 /* upright_point = camera.eye + upright_v;
  bottomleft_point = camera.eye + bottomleft_v;
  upleft_point = camera.eye + upleft_v;
  bottomright_point = camera.eye + bottomright_v;
*/
  right_plane = R3Plane(eye, upright_point, bottomright_point);
  left_plane = R3Plane(eye, upleft_point, bottomleft_point);
  top_plane = R3Plane(eye, upright_point, upleft_point);
  bottom_plane = R3Plane(eye, bottomright_point, bottomleft_point);
  camera_plane = R3Plane(upright_point, upleft_point, bottomright_point);

  R3Vector rn = right_plane.Normal();
  R3Vector ln = left_plane.Normal();
  R3Vector tn = top_plane.Normal();
  R3Vector bn = bottom_plane.Normal();
  R3Vector cn = camera_plane.Normal();

  if (rn.Dot(right) < 0) right_plane.Flip();
  if (ln.Dot(right) > 0) left_plane.Flip();
  if (tn.Dot(up) < 0) top_plane.Flip();
  if (bn.Dot(up) > 0) bottom_plane.Flip();
  if (cn.Dot(towards) < 0) camera_plane.Flip();


}
