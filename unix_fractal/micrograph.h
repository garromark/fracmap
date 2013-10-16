/*
 * micrograph.h
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#ifndef _MICROGRAPH_H
#define _MICROGRAPH_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

#include "fractal.h"
#include "settings.h"
#include "Vector_2.h"

using namespace std;

class Micrograph {
public:
	Micrograph(Fractal & fractal)
	{
	  
	  //_monomer_radius = PARTS_PER_RADIUS;
	  
		make_pixels(fractal); // This needs to be done first
		make_points(); //This next
		make_boundary();

		calc_info();   //Alright we can calc now.
	}
	
	//This code will be needed to open threshed micrographs
	Micrograph(vector<vector<bool> > & img)
	{
		//_monomer_radius = monomer_radius;
		cols = img[0].size(); rows = img.size();
		
		//pixelsFromBitarr( bitArray, width );
		pixels = img;
		make_points();
	    make_boundary();
		
	    calc_info();
	}
	
	~Micrograph() {}
		
	double Rg() const;
	Vector_2 cm() const;
	double Nproj() const;
	double Area() const;
	double Perimeter() const;

	pair<double, double> Df();
	//pair<double,double> perimeterDf();
	pair<double, double> Df_grid();
	pair<double,double> perimeterDf_grid();
  
	pair<double,double> perdf();
	
	//void read_micrograph(ifstream & input);
	
	void dimensions(double &length, double&width) const;
	//void dimensions_new(double &length, double&width) const;
  
	char* getBitArray(unsigned int &width);
	char* getBoundaryBitArray(unsigned int &width);
  
	//Debug
private:
	//Setup the basic data for running the algorithms
	void make_pixels(Fractal & _fractal);
	void make_points();
	void make_boundary();
	void make_boundary_pixels(vector<vector<bool> > & boundaryPixels);  
	char* makeBitarray(vector<vector<bool> > & pixelArr, unsigned int & sideSize);
  
	void calc_info();	
	
	pair<double, double> boxcount_ball( const vector<Vector_2>& space ) const;
    pair<double, double> boxcount_grid( const vector<Vector_2>& space ) const;

	long cols, rows;
	vector<vector<bool> > pixels;
	vector<Vector_2> points;
	vector<Vector_2> boundary;
  
	Vector_2 _cm;
	double _Rg;
	static const int _monomer_radius = 10; // This is only for make_pixels to construct a fake micrograph from a Fractal class
};
#endif
