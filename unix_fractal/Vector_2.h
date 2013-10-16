/*
 *  Vector_2.h
 *  wxFracMap
 *
 *  Created by eluusive on 06/03/2009.
 *  Copyright 2009 University Of Neveda, Reno. All rights reserved.
 *
 */

#ifndef _VECTOR_2_H
#define _VECTOR_2_H

#include "settings.h"
#include "Vector_2.h"

using namespace std;

class Vector_2 {
public:
	Vector_2(double _x, double _y) : x(_x), y(_y) {}
	Vector_2() { x=0; y=0;}
	~Vector_2() {}
	
	/*
	 typedef vector<Vector_2>::iterator::iterator_category iterator_category;
	 typedef vector<Vector_2>::iterator::value_type value_type;
	 typedef vector<Vector_2>::iterator::difference_type difference_type;
	 typedef Vector_2 * pointer;
	 typedef Vector_2 & reference;
	 */
	
	void initialize(double _x, double _y);
	
	double distance(Vector_2 pt2) const;	
	double square_difference(Vector_2 target) const;
	double magnitude() const;
	Vector_2 normalize() const;
	
	Vector_2 operator-(const Vector_2 & rhs) const;
	Vector_2 operator+(const Vector_2 & rhs) const;
	Vector_2 operator/(double den) const;
	Vector_2 operator*(double mult) const;
	
	double operator*(const Vector_2 rhs ) const;
	double X(const Vector_2 rhs ) const;
	
	Vector_2& operator-=(const Vector_2 & rhs);
	Vector_2& operator+=(const Vector_2 & rhs);
	Vector_2& operator/=(const double den);
	Vector_2& operator*=(const double mult);
	
	Vector_2 & operator=(const Vector_2 &o);
	bool operator==(const Vector_2 &b) const;
	
	double x, y;
	
	static Vector_2 center() { return _center; }
private:
	static const Vector_2 _center;
};

#endif
