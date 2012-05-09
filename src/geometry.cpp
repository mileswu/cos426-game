#include "geometry.h"

R3Vector::
R3Vector(void)
{
}



R3Vector::
R3Vector(double x, double y, double z)
{
  // Initialize coordinates
  v[0] = x; 
  v[1] = y; 
  v[2] = z;
}



R3Vector::
R3Vector(const R3Vector& vector)
{
  // Initialize coordinates
  v[0] = vector.v[0]; 
  v[1] = vector.v[1]; 
  v[2] = vector.v[2]; 
}



R3Vector::
R3Vector(double pitch, double yaw)
{
  // Initialize coordinates
  double cosine_yaw = cos(yaw);
  v[0] = cosine_yaw * cos(pitch);
  v[1] = cosine_yaw * sin(pitch); 
  v[2] = sin(yaw);
}



R3Vector::
R3Vector(const double array[3])
{
  // Initialize coordinates
  v[0] = array[0]; 
  v[1] = array[1]; 
  v[2] = array[2];
}



double R3Vector::
Dot(const R3Vector& vector) const
{
  // Return dot product of this with vector
  return((v[0]*vector.v[0]) + (v[1]*vector.v[1]) + (v[2]*vector.v[2]));
}



void R3Vector::
Normalize(void)
{
  // Make vector have length equal to one
  double length = Length();
  if (length == 0.0) return;
  v[0] /= length;
  v[1] /= length;
  v[2] /= length;
}



void R3Vector::
Flip (void) 
{
  // Flip vector direction
  v[0] = -v[0];
  v[1] = -v[1];
  v[2] = -v[2];
}



void R3Vector::
Cross(const R3Vector& vector)
{
  // Modify this vector so that it holds the cross product of this with passed vector
  const double x = (v[1]*vector.v[2]) - (v[2]*vector.v[1]);
  const double y = (v[2]*vector.v[0]) - (v[0]*vector.v[2]);
  const double z = (v[0]*vector.v[1]) - (v[1]*vector.v[0]);
  v[2] = z; v[1] = y; v[0] = x; 
}



void R3Vector::
Rotate(const R3Vector& axis, double theta)
{
  // Rotate vector counterclockwise around axis (looking at axis end-on) (rz(xaxis) = yaxis)
  // From Goldstein: v' = v cos t + a (v . a) [1 - cos t] - (v x a) sin t 
  const double cos_theta = cos(theta);
  const double dot = this->Dot(axis);
  R3Vector cross = *this % axis;
  *this *= cos_theta;
  *this += axis * dot * (1.0 - cos_theta);
  *this -= cross * sin(theta); 
}



void R3Vector::
Project(const R3Vector& vector)
{
  // Project onto another vector    
  double dot = this->Dot(vector);
  double length = vector.Length();
  if (length != 0) dot /= (length * length);
  *this = vector * dot;
}


/*
void R3Vector::
Project(const R3Plane& plane) 
{
  // Project onto plane
  *this -= plane.Normal() * this->Dot(plane.Normal());
}



void R3Vector::
Mirror(const R3Plane& plane)
{
  // Mirror vector across plane
  double d = Dot(plane.Normal());
  *this += plane.Normal() * (-2.0 * d);
}



void R3Vector::
Transform(const R3Matrix& matrix)
{
  // Transform vector
  *this = matrix * (*this);
}



void R3Vector::
InverseTransform(const R3Matrix& matrix)
{
  // Transform vector by inverse
  *this = matrix.Inverse() * (*this);
}

*/

R3Vector& R3Vector::
operator=(const R3Vector& vector)
{
  // Copy vector coordinates
  v[0] = vector.v[0];
  v[1] = vector.v[1];
  v[2] = vector.v[2];
  return *this;
}



R3Vector& R3Vector::
operator+=(const R3Vector& vector)
{
  // Add vector to this 
  v[0] += vector.v[0];
  v[1] += vector.v[1];
  v[2] += vector.v[2];
  return *this;
}



R3Vector& R3Vector::
operator-=(const R3Vector& vector)
{
  // Add subtract vector from this 
  v[0] -= vector.v[0];
  v[1] -= vector.v[1];
  v[2] -= vector.v[2];
  return *this;
}



R3Vector& R3Vector::
operator*=(const double a)
{
  // Multiply vector coordinates by a scalar
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
  return *this;
}



R3Vector& R3Vector::
operator*=(const R3Vector& vector)
{
  // Entry by entry multiply (not dot or cross product)
  v[0] *= vector.v[0];
  v[1] *= vector.v[1];
  v[2] *= vector.v[2];
  return *this;
}



R3Vector& R3Vector::
operator/=(const double a)
{
  // Divide vector coordinates by a scalar
  assert(a != 0);
  v[0] /= a;
  v[1] /= a;
  v[2] /= a;
  return *this;
}



R3Vector& R3Vector::
operator/=(const R3Vector& vector)
{
  // Entry by entry divide
  assert(vector.v[0] != 0);
  assert(vector.v[1] != 0);
  assert(vector.v[2] != 0);
  v[0] /= vector.v[0];
  v[1] /= vector.v[1];
  v[2] /= vector.v[2];
  return *this;
}



R3Vector 
operator+(const R3Vector& vector)
{
  // Return vector
  return vector;
}



R3Vector 
operator-(const R3Vector& vector)
{
  // Return opposite of vector
  return R3Vector(-vector.v[0], 
                  -vector.v[1], 
                  -vector.v[2]);
}



R3Vector 
operator+(const R3Vector& vector1, const R3Vector& vector2)
{
  // Return sum of two vectors
  return R3Vector(vector1.v[0] + vector2.v[0], 
                  vector1.v[1] + vector2.v[1], 
                  vector1.v[2] + vector2.v[2]);
}



R3Vector 
operator-(const R3Vector& vector1, const R3Vector& vector2)
{
  // Return difference of two vectors
  return R3Vector(vector1.v[0] - vector2.v[0], 
                  vector1.v[1] - vector2.v[1], 
                  vector1.v[2] - vector2.v[2]);
}



R3Vector
operator*(const double a, const R3Vector& vector)
{
  // Return vector multiplied by a scalar
  return vector * a;
}



R3Vector 
operator*(const R3Vector& vector1, const R3Vector& vector2)
{
  // Return product of two vectors
  // Entry by entry multiply (not dot or cross product)
  return R3Vector(vector1.v[0] * vector2.v[0], 
                  vector1.v[1] * vector2.v[1], 
                  vector1.v[2] * vector2.v[2]);
}



R3Vector 
operator*(const R3Vector& vector, const double a)
{
  // Return product of vector and scalar
  return R3Vector(vector.v[0] * a, 
                  vector.v[1] * a, 
                  vector.v[2] * a);
}



R3Vector 
operator/(const R3Vector& vector1, const R3Vector& vector2)
{
  // Return coordinates of vector1 divided by corresponding coordinates of vector2
  assert(vector2.v[0] != 0);
  assert(vector2.v[1] != 0);
  assert(vector2.v[2] != 0);
  return R3Vector(vector1.v[0]/vector2.v[0], 
                  vector1.v[1]/vector2.v[1], 
                  vector1.v[2]/vector2.v[2]);
}



R3Vector 
operator/(const R3Vector& vector, const double a)
{
  // Return vector divided by a scalar
  assert(a != 0);
  return R3Vector(vector.v[0]/a, 
                  vector.v[1]/a, 
                  vector.v[2]/a);
}



R3Vector 
operator%(const R3Vector& vector1, const R3Vector& vector2)
{
  // Return cross product of two vectors
  R3Vector v = vector1;
  v.Cross(vector2);
  return v;
}



void R3Vector::
Print(FILE *fp) const
{
  // Print vector coordinates
  fprintf(fp, "%g %g %g", v[0], v[1], v[2]);
}

// Public functions 

R3Point::
R3Point(void)
{
}



R3Point::
R3Point(double x, double y, double z)
{
  // Initialize coordinates
  v[0] = x; 
  v[1] = y; 
  v[2] = z;
}



R3Point::
R3Point(const R3Point& point)
{
  // Initialize coordinates
  v[0] = point.v[0]; 
  v[1] = point.v[1]; 
  v[2] = point.v[2]; 
}



R3Point::
R3Point(const double array[3])
{
  // Initialize coordinates
  v[0] = array[0]; 
  v[1] = array[1]; 
  v[2] = array[2];
}



R3Vector R3Point::
Vector(void) const
{
  // Return vector to point from origin
  return R3Vector(v[0], v[1], v[2]);
}



void R3Point::
Translate (const R3Vector& vector) 
{
  // Move point by vector
  *this += vector;
}


/*
void R3Point::
Project(const R3Line& line)
{
  // Move point to closest point on line
  const R3Point *p = &(line.Point());
  const R3Vector *v = &(line.Vector());
  double denom = v->Dot(*v);
  if (denom == 0) return;
  double t = (v->X() * (X() - p->X()) + v->Y() *(Y() - p->Y()) + v->Z() * (Z() - p->Z())) / denom;
  *this = *p + *v * t;
}



void R3Point::
Project(const R3Plane& plane)
{
  // Move point to closest point on plane
  double d = R3SignedDistance(plane, *this);
  *this += plane.Normal() * -d;
}



void R3Point::
Mirror(const R3Plane& plane)
{
  // Mirror point across plane
  double d = R3SignedDistance(plane, *this);
  *this += plane.Normal() * (-2.0 * d);
}

*/

void R3Point::
Rotate(const R3Vector& axis, double theta)
{
  // Rotate point counterclockwise around axis through origin by radians ???
  R3Vector v = Vector();
  v.Rotate(axis, theta);
  *this = v.Point();
}

/*

void R3Point::
Rotate(const R3Line& axis, double theta)
{
  // Translate axis to origin
  R3Vector v = *this - axis.Point();

  // Rotate point counterclockwise around axis through origin by radians ???
  v.Rotate(axis.Vector(), theta);

  // Translate axis back from origin
  *this = axis.Point() + v;
}



void R3Point::
Transform(const R3Matrix& matrix)
{
  // Transform point
  *this = matrix * (*this);
}



void R3Point::
InverseTransform(const R3Matrix& matrix)
{
  // Transform point by inverse
  *this = matrix.Inverse() * (*this);
}*/



R3Point& R3Point::
operator=(const R3Point& point)
{
  // Assign coordinates
  v[0] = point.v[0];
  v[1] = point.v[1];
  v[2] = point.v[2];
  return *this;
}



R3Point& R3Point::
operator+=(const R3Point& point)
{
  // Add coordinates of point 
  v[0] += point[0];
  v[1] += point[1];
  v[2] += point[2];
  return *this;
}



R3Point& R3Point::
operator+=(const R3Vector& vector)
{
  // Add coordinates of vector
  v[0] += vector[0];
  v[1] += vector[1];
  v[2] += vector[2];
  return *this;
}



R3Point& R3Point::
operator-=(const R3Vector& vector)
{
  // Subtract coordinates of vector
  v[0] -= vector[0];
  v[1] -= vector[1];
  v[2] -= vector[2];
  return *this;
}



R3Point& R3Point::
operator*=(const double a)
{
  // Multiply coordinates by scalar 
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
  return *this;
}



R3Point& R3Point::
operator/=(const double a)
{
  // Divide coordinates by scalar 
  //  assert(!zero(a)); 
  v[0] /= a;
  v[1] /= a;
  v[2] /= a;
  return *this;
}



R3Point 
operator-(const R3Point& point)
{
  // Subtract coordinates of point
  return R3Point(-point.v[0], 
                 -point.v[1], 
                 -point.v[2]);
}



R3Point
operator+(const R3Vector& vector, const R3Point& point)
{
  // Add vector and point
  return point + vector;
}



R3Point 
operator+(const R3Point& point1, const R3Point& point2)
{
  // Add two points
  return R3Point(point1.v[0] + point2.v[0], 
                 point1.v[1] + point2.v[1], 
                 point1.v[2] + point2.v[2]);
}



R3Point 
operator+(const R3Point& point, const R3Vector& vector)
{
  // Add point and vector
  return R3Point(point.X() + vector.X(), 
                 point.Y() + vector.Y(), 
                 point.Z() + vector.Z());
}



R3Vector 
operator-(const R3Point& point1, const R3Point& point2)
{
  // Subtract two points
  return R3Vector(point1.v[0] - point2.v[0], 
                  point1.v[1] - point2.v[1], 
                  point1.v[2] - point2.v[2]);
}



R3Point 
operator-(const R3Point& point, const R3Vector& vector)
{
  // Subtract vector from point
  return R3Point(point.X() - vector.X(), 
                 point.Y() - vector.Y(), 
                 point.Z() - vector.Z());
}



R3Point
operator*(const double a, const R3Point& point)
{
  // Multiply point by scalar
  return point * a;
}



R3Point 
operator*(const R3Point& point, const double a)
{
  // Multiply point by scalar
  return R3Point(point.X() * a, 
                 point.Y() * a,
                 point.Z() * a);
}



R3Point 
operator/(const R3Point& point, const double a)
{
  // Divide point by scalar
  assert(a != 0);
  return R3Point(point.X() / a, 
                 point.Y() / a, 
                 point.Z() / a);
}



void R3Point::
Print(FILE *fp) const
{
  // Print point coordinates
  fprintf(fp, "%g %g %g", v[0], v[1], v[2]);
}
