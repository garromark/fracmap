/*
 *  Vector_3.h
 *  FracMap
 *
 *  Created by Shammah Chancellor on 13/02/2009.
 *  Copyright 2009 University Of Neveda, Reno. All rights reserved.
 *
 */
#ifndef _VECTOR_3_H
#define _VECTOR_3_H

using namespace std;

class Vector_3 {
public:
	Vector_3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	Vector_3() { x=0; y=0; z = 0;}
	~Vector_3() {}
	
	void initialize(double _x, double _y, double _z);
	
	double distance(Vector_3 pt2) const;	
	double square_difference(Vector_3 target) const;
	double magnitude() const;
	Vector_3 normalize() const;
	
	Vector_3 operator-(const Vector_3 & rhs) const;
	Vector_3 operator+(const Vector_3 & rhs) const;
	Vector_3 operator/(double den) const;
	Vector_3 operator*(double mult) const;
	
	double operator*(const Vector_3 rhs ) const;
	Vector_3 X(const Vector_3 rhs ) const;
	
	Vector_3& operator-=(const Vector_3 & rhs);
	Vector_3& operator+=(const Vector_3 & rhs);
	Vector_3& operator/=(const double den);
	Vector_3& operator*=(const double mult);
	
	Vector_3 & operator=(const Vector_3 &o);
	bool operator==(const Vector_3 &b) const;
	
	double x, y, z;
	
	static Vector_3 center() { return _center; }
private:
	static const Vector_3 _center;
};


void RandomQuat( double Quat[] ); //Size = 4
void QuatToMatrix( const double Quat[], double Matrix[] );  // 4 and 9 respectively
void VectorToMatrix( const Vector_3 newZ, double RotMatrix[] );
Vector_3 RandomVec( ); //3



template<class T1, class T2, class T3>
double tri_area(T1 pt1, T2 pt2, T3 pt3)
{
	double
	a = pt1.x - pt3.x,
	b = pt1.y - pt3.y,
	c = pt2.x - pt3.x,
	d = pt2.y - pt3.y;
	return (0.5 * fabs((a * d) - (b * c)));
}

#endif