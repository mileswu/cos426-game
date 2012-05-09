#include <stdio.h>
#include <math.h>
#include <assert.h>
class R3Point;
class R3Vector;

#define R3_X 0
#define R3_Y 1
#define R3_Z 2

class R3Vector {
 public:
  // Constructors
  R3Vector(void);
  R3Vector(const R3Vector& vector);
  R3Vector(double x, double y, double z);
  R3Vector(const double array[3]);
  R3Vector(double pitch, double yaw);
  
  // Coordinate access
  double X(void) const;
  double Y(void) const;
  double Z(void) const;
  double& operator[](int dim);
  double operator[](int dim) const;

  // Properties
  bool IsZero(void) const;
  bool IsNormalized(void) const;
  double Length(void) const;
  R3Point Point(void) const;
  int MinDimension(void) const;
  int MaxDimension(void) const;

  // Relationships
  bool operator==(const R3Vector& vector) const;
  bool operator!=(const R3Vector& vector) const;
  double Dot(const R3Vector& vector) const;

  // Manipulation functions
  void SetX(double x);
  void SetY(double y);
  void SetZ(double z);
  void SetCoord(int dim, double coord);
  void Flip(void);
  void Normalize(void);
  void Cross(const R3Vector& vector);
  void Rotate(const R3Vector& axis, double theta);
  void Project(const R3Vector& vector);
  /*void Project(const R3Plane& plane);
  void Mirror(const R3Plane& plane);
  void Transform(const R3Matrix& matrix);
  void InverseTransform(const R3Matrix& matrix);*/
  void Reset(double x, double y, double z);

  // Assignment operators
  R3Vector& operator=(const R3Vector& vector);
  R3Vector& operator+=(const R3Vector& vector);
  R3Vector& operator-=(const R3Vector& vector);
  R3Vector& operator*=(const double a);
  R3Vector& operator*=(const R3Vector& vector);
  R3Vector& operator/=(const double a);
  R3Vector& operator/=(const R3Vector& vector);

  // Arithmetic operators
  friend R3Vector operator+(const R3Vector& vector);
  friend R3Vector operator-(const R3Vector& vector);
  friend R3Vector operator+(const R3Vector& vector1, const R3Vector& vector2);
  friend R3Vector operator-(const R3Vector& vector1, const R3Vector& vector2);
  friend R3Vector operator*(const R3Vector& vector1, const R3Vector& vector2);
  friend R3Vector operator*(const R3Vector& vector, const double a);
  friend R3Vector operator*(const double a, const R3Vector& vector);
  friend R3Vector operator/(const R3Vector& vector1, const R3Vector& vector2);
  friend R3Vector operator/(const R3Vector& vector, const double a);
  friend R3Vector operator%(const R3Vector& vector1, const R3Vector& vector2);

  // Output functions
  void Print(FILE *fp = stdout) const;

 private:
  double v[3];
};

class R3Point {
 public:
  // Constructors
  R3Point(void);
  R3Point(const R3Point& point);
  R3Point(double x, double y, double z);
  R3Point(const double array[3]);

  // Coordinate access
  double X(void) const;
  double Y(void) const;
  double Z(void) const;
  double operator[](int dim) const;
  double& operator[](int dim);

  // Properties
  bool IsZero(void) const;
  R3Vector Vector(void) const;
  bool operator==(const R3Point& point) const;
  bool operator!=(const R3Point& point) const;

  // Manipulation functions
  void SetX(double x);
  void SetY(double y);
  void SetZ(double z);
  void SetCoord(int dim, double coord);
  void Translate(const R3Vector& vector);
  /*void Project(const R3Line& line);
  void Project(const R3Plane& plane);
  void Mirror(const R3Plane& plane);*/
  void Rotate(const R3Vector& axis, double theta);
  /*void Rotate(const R3Line& axis, double theta);
  void Transform(const R3Matrix& matrix);
  void InverseTransform(const R3Matrix& matrix);*/
  void Reset(double x, double y, double z);

  // Assignment operators
  R3Point& operator=(const R3Point& point);
  R3Point& operator+=(const R3Point& point);
  R3Point& operator+=(const R3Vector& vector);
  R3Point& operator-=(const R3Vector& vector);
  R3Point& operator*=(const double a);
  R3Point& operator/=(const double a);

  // Arithmetic operators
  friend R3Point operator-(const R3Point& point);
  friend R3Point operator+(const R3Point& point1, const R3Point& point2);
  friend R3Point operator+(const R3Point& point, const R3Vector& vector);
  friend R3Point operator+(const R3Vector& vector, const R3Point& point);
  friend R3Vector operator-(const R3Point& point1, const R3Point& point2);
  friend R3Point operator-(const R3Point& point, const R3Vector& vector);
  friend R3Point operator*(const R3Point& point, const double a);
  friend R3Point operator*(const double a, const R3Point& point);
  friend R3Point operator/(const R3Point& point, const double a);

  // Output functions
  void Print(FILE *fp = stdout) const;

 private:
  double v[3];
};






// Inline functions 

inline double R3Vector::
X (void) const
{
  // Return X coordinate
  return (v[0]);
}



inline double R3Vector::
Y (void) const
{
  // Return Y coordinate
  return (v[1]);
}



inline double R3Vector::
Z (void) const
{
  // Return Z coordinate
  return (v[2]);
}



inline double R3Vector::
operator[](int dim) const
{
  // Return coordinate in given dimension
  assert((dim>=R3_X) && (dim<=R3_Z));
  return(v[dim]);
}



inline double& R3Vector::
operator[] (int dim) 
{
  // Return reference to coordinate in given dimension
  assert((dim>=R3_X) && (dim<=R3_Z));
  return(v[dim]);
}



inline bool R3Vector::
IsZero (void) const
{
  // Return whether vector is zero
  return ((v[0] == 0.0) && (v[1] == 0.0) && (v[2] == 0.0));
}



inline bool R3Vector::
IsNormalized (void) const
{
  // Return whether vector is normalized
  double length = Length();
  if (length < 1.0 - 1E-6) return false;
  if (length > 1.0 + 1E-6) return false;
  return true;
}



inline double R3Vector::
Length(void) const
{
  // Return length of vector
  return (sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2])));
}



inline R3Point R3Vector::
Point(void) const
{
  // Return point at (0,0,0) plus vector
  return R3Point(v[0], v[1], v[2]);
}



inline int R3Vector::
MinDimension(void) const
{
  // Return principal dimension of vector
  if (fabs(v[0]) <= fabs(v[1])) {
    if (fabs(v[0]) <= fabs(v[2])) return R3_X;
    else return R3_Z;
  }
  else {
    if (fabs(v[1]) <= fabs(v[2])) return R3_Y;
    else return R3_Z;
  }
}



inline int R3Vector::
MaxDimension(void) const
{
  // Return principal dimension of vector
  if (fabs(v[0]) >= fabs(v[1])) {
    if (fabs(v[0]) >= fabs(v[2])) return R3_X;
    else return R3_Z;
  }
  else {
    if (fabs(v[1]) >= fabs(v[2])) return R3_Y;
    else return R3_Z;
  }
}



inline bool R3Vector::
operator==(const R3Vector& vector) const
{
  // Return whether vector is equal
  return ((v[0] == vector.v[0]) && (v[1] == vector.v[1]) && (v[2] == vector.v[2]));
}



inline bool R3Vector::
operator!=(const R3Vector& vector) const
{
  // Return whether vector is not equal
  return ((v[0] != vector.v[0]) || (v[1] != vector.v[1]) || (v[2] != vector.v[2]));
}



inline void R3Vector::
SetX (double x) 
{
  // Set X coord
  v[0] = x;
}



inline void R3Vector::
SetY (double y) 
{
  // Set Y coord
  v[1] = y;
}



inline void R3Vector::
SetZ (double z) 
{
  // Set Z coord
  v[2] = z;
}



inline void R3Vector::
SetCoord (int dim, double coord) 
{
  // Set coord in given dimension
  v[dim] = coord;
}



inline void R3Vector::
Reset(double x, double y, double z) 
{
  // Set all coords
  v[0] = x;
  v[1] = y;
  v[2] = z;
}

inline double R3Point::
X (void) const
{
  // Return X coordinate
  return(v[0]);
}



inline double R3Point::
Y (void) const
{
  // Return y coordinate
  return(v[1]);
}



inline double R3Point::
Z (void) const
{
  // Return z coordinate
  return(v[2]);
}



inline double R3Point::
operator[](int dim) const
{
  // Return coordinate in given dimension (0=X, 1=Y, 2=Z)
  assert((dim>=R3_X) && (dim<=R3_Z));
  return(v[dim]);
}



inline double& R3Point::
operator[] (int dim) 
{
  // Return reference to coordinate in given dimension (0=X, 1=Y, 2=Z)
  assert((dim>=R3_X) && (dim<=R3_Z));
  return(v[dim]);
}



inline bool R3Point::
IsZero(void) const
{
  // Return whether point is zero
  return ((v[0] == 0.0) && (v[1] == 0.0) && (v[2] == 0.0));
}



inline void R3Point::
SetX (double x) 
{
  // Set X coord
  v[0] = x;
}



inline void R3Point::
SetY (double y) 
{
  // Set Y coord
  v[1] = y;
}



inline void R3Point::
SetZ (double z) 
{
  // Set Z coord
  v[2] = z;
}



inline void R3Point::
SetCoord (int dim, double coord) 
{
  // Set coord in given dimension
  v[dim] = coord;
}



inline void R3Point::
Reset(double x, double y, double z) 
{
  // Set all coords
  v[0] = x;
  v[1] = y;
  v[2] = z;
}



inline bool R3Point::
operator==(const R3Point& point) const
{
  // Return whether point is equal
  return ((v[0] == point.v[0]) && (v[1] == point.v[1]) && (v[2] == point.v[2]));
}



inline bool R3Point::
operator!=(const R3Point& point) const
{
  // Return whether point is not equal
  return ((v[0] != point.v[0]) || (v[1] != point.v[1]) || (v[2] != point.v[2]));
}

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