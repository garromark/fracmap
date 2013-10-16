/*
 * Vector_3.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#include "micrograph.h"
#include "fractal.h"
#include "settings.h"
#include "Vector_3.h"

const Vector_3 Vector_3::_center = Vector_3(0,0,0);

double tri_area( Vector_3 pt1, Vector_3 pt2, Vector_3 pt3 )
{
  Vector_3 a = pt2 - pt1;
  Vector_3 b = pt3 - pt1;
	
  return 0.5 * a.X(b).magnitude();
}

double Vector_3::operator*(const Vector_3 rhs) const
{
  return x*rhs.x + y*rhs.y + z*rhs.z;
}

Vector_3 Vector_3::X(const Vector_3 rhs) const
{
  return Vector_3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

void RandomQuat( double Quat[] )
{
  double u1 = (double)rand() / RAND_MAX;
  double u2 = (double)rand() / RAND_MAX;
  double u3 = (double)rand() / RAND_MAX;
	
  Quat[0] = sqrt( 1 - u1 ) * sin( 2 * PI * u2);
  Quat[1] = sqrt( 1 - u1 ) * cos( 2 * PI * u2);
  Quat[2] = sqrt( u1 ) * sin( 2 * PI * u3);
  Quat[3] = sqrt( u1 ) * cos( 2 * PI * u3);
}

Vector_3 RandomVec( )
{
  double u1 = (double)rand() / RAND_MAX;
  double u2 = (double)rand() / RAND_MAX;
	
  //1 - ( 2*u2 - 1 )^2  ==  4*u2 - 4*u2*u2
  return Vector_3( sqrt( 4*u2 - 4 * u2 * u2 ) * sin(2*PI*u1),
		   sqrt( 4*u2 - 4 * u2 * u2 ) * cos(2*PI*u1),
		   ( 2 * u2 - 1) );
}

void QuatToMatrix( const double Quat[], double Matrix[] ) //4 and 9 respectively
{
  //w i j k
  double a = Quat[0], b = Quat[1], c = Quat[2], d = Quat[3];
	
  Matrix[ 0 ] = a*a + b*b - c*c - d*d;
  Matrix[ 1 ] = 2*b* c- 2*a*d;
  Matrix[ 2 ] = 2*a*c + 2*b*d;
	
  Matrix[ 0 + 3 ] = 2*a*d + 2*b*c;
  Matrix[ 1 + 3 ] = a*a - b*b + c*c - d*d;
  Matrix[ 2 + 3 ] = 2*c*d - 2*a*b;
	
  Matrix[ 0 + 6 ] = 2*b*d - 2*a*c;
  Matrix[ 1 + 6 ] = 2*a*b + 2*c*d;
  Matrix[ 2 + 6 ] = a*a - b*b - c*c + d*d;
}

void VectorToMatrix( const Vector_3 newZ, double RotMatrix[] )
{
  //TODO: Convert the newZ to a rotation matrix.
  //Details at: http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
  
  Vector_3 zAxis = Vector_3(0,0,1);
  Vector_3 axis = newZ.X(zAxis).normalize();
  double sina = newZ.X(zAxis).magnitude(); //Cross produce = |a||b|sin alpha
  double cosa = newZ * zAxis; // Dot producct = |a||b|cos alpha
  double a = atan2( sina, cosa );
  
  //w i j k
  double RotQuat[4] = { cos(a/2) , (axis.x * sin(a/2)), (axis.y * sin(a/2)), (axis.z * sin(a/2))};
  
  double mag = RotQuat[0] * RotQuat[0] + RotQuat[1] * RotQuat[1]  + RotQuat[2] * RotQuat[2] + RotQuat[3] * RotQuat[3];
  
  //cout << "Angle a: " << a << endl;
  //cout << "Quat magnitude: " << mag << endl;
  
  if( fabs( mag - 1 ) < .05 ) //The Quaternion was well defined
  {
	//Cheating and reusing code.
	//It's easy to express the rotation we need with Quaternions, and we can then convert it to a matrix.
	
	QuatToMatrix( RotQuat, RotMatrix );
  } else { //The rotation was too parallel to the Z axis already. 
	//Return +/- identity matrix.
	for( int i = 0; i< 9; i++ )
	  RotMatrix[i] = 0;
	
	RotMatrix[0] = RotMatrix[4] = RotMatrix[8] = cosa < 0 ? -1 : 1; //flip depending on projection of newZ on oldZ
  }
}


void Vector_3::initialize(double _x, double _y, double _z)
{
  x = _x;
  y = _y;
  z = _z;
}


double Vector_3::magnitude() const
{
  return sqrt( x*x + y*y + z*z ) ;
}

Vector_3 Vector_3::normalize() const
{
  double norm = magnitude();
  return Vector_3(x/norm,y/norm,z/norm);
}

double Vector_3::distance(Vector_3 pt2) const
{
  return sqrt(square_difference(pt2));
}

double Vector_3::square_difference(Vector_3 pt2) const
{
  double x1 = x - pt2.x,
    y1 = y - pt2.y,
    z1 = z - pt2.z;
	
  return (x1*x1 + y1*y1 + z1*z1);
}


Vector_3 Vector_3::operator+(const Vector_3 & rhs) const
{
  Vector_3 temp(this->x + rhs.x,
		this->y + rhs.y,
		this->z + rhs.z);
	
  return temp;
}

Vector_3 Vector_3::operator-(const Vector_3 & rhs) const
{
  Vector_3 temp(this->x - rhs.x,
		this->y - rhs.y,
		this->z - rhs.z);
	
  return temp;
}

Vector_3 Vector_3::operator/(double den) const
{
  if (den == 0) return *this; //Why is this here?

  Vector_3 temp(this->x / den,
		this->y / den,
		this->z / den);

  return temp;
}

Vector_3 Vector_3::operator*(const double scalar) const
{
	
  return Vector_3(x*scalar, y*scalar, z*scalar);
}


Vector_3& Vector_3::operator+=(const Vector_3 & rhs)
{
  this->x += rhs.x;
  this->y += rhs.y;
  this->z += rhs.z;
	
  return *this;
}

Vector_3& Vector_3::operator-=(const Vector_3 & rhs)
{
  this->x -= rhs.x;
  this->y -= rhs.y;
  this->z -= rhs.z;
	
  return *this;
}

Vector_3 & Vector_3::operator/=(const double scalar)
{
  x /= scalar;
  y /= scalar;
  z /= scalar;
	
  return *this;
}

Vector_3 & Vector_3::operator*=(const double scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
	
  return *this;
}

Vector_3 & Vector_3::operator=(const Vector_3 & rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
	
  return *this;
}


bool Vector_3::operator==( const Vector_3 & b ) const
{
  return (x == b.x && y == b.y && z == b.z);
}
