/*
 *  Vector_2.cpp
 *  wxFracMap
 *
 *  Created by eluusive on 06/03/2009.
 *  Copyright 2009 University Of Neveda, Reno. All rights reserved.
 *
 */

#include "Vector_2.h"

const Vector_2 Vector_2::_center = Vector_2(0,0);

void Vector_2::initialize(double _x, double _y)
{
	x = _x;
	y = _y;
}

double tri_area( Vector_2 pt1, Vector_2 pt2, Vector_2 pt3 )
{
  Vector_2 a = pt2 - pt1;
  Vector_2 b = pt3 - pt1;
  
  return 0.5 * a.X(b);
}

double Vector_2::operator*(const Vector_2 rhs) const
{
  return x*rhs.x + y*rhs.y ;
}

double Vector_2::X(const Vector_2 rhs) const
{
  return x*rhs.y - y*rhs.x;
}

double Vector_2::magnitude() const
{
	return sqrt( x*x + y*y  ) ;
}

Vector_2 Vector_2::normalize() const
{
	double norm = magnitude();
	return Vector_2(x/norm,y/norm);
}

double Vector_2::distance(Vector_2 pt2) const
{
	return sqrt(square_difference(pt2));
}

double Vector_2::square_difference(Vector_2 pt2) const
{
	double x1 = x - pt2.x,
	y1 = y - pt2.y;
	
	return (x1*x1 + y1*y1);
}


Vector_2 Vector_2::operator+(const Vector_2 & rhs) const
{
	Vector_2 temp(this->x + rhs.x,
				  this->y + rhs.y);
	
	return temp;
}

Vector_2 Vector_2::operator-(const Vector_2 & rhs) const
{
	Vector_2 temp(this->x - rhs.x,
				  this->y - rhs.y);
	
	return temp;
}

Vector_2 Vector_2::operator/(double den) const
{
	if (den == 0) return *this; //Why is this here?
	
	Vector_2 temp(this->x / den,
				  this->y / den);
	
	return temp;
}

Vector_2 Vector_2::operator*(const double scalar) const
{
	
	return Vector_2(x*scalar, y*scalar);
}


Vector_2& Vector_2::operator+=(const Vector_2 & rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	
	return *this;
}

Vector_2& Vector_2::operator-=(const Vector_2 & rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	
	return *this;
}

Vector_2 & Vector_2::operator/=(const double scalar)
{
	x /= scalar;
	y /= scalar;
	
	return *this;
}

Vector_2 & Vector_2::operator*=(const double scalar)
{
	x *= scalar;
	y *= scalar;
	
	return *this;
}

Vector_2 & Vector_2::operator=(const Vector_2 & rhs)
{
	x = rhs.x;
	y = rhs.y;
	
	return *this;
}


bool Vector_2::operator==( const Vector_2 & b ) const
{
	return (x == b.x && y == b.y);
}

