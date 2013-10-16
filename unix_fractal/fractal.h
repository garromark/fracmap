/*
 * fractal.h
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#ifndef _FRACTAL_H
#define _FRACTAL_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <sstream>

#include "settings.h"

using namespace std;

class Fractal {
public:
	Fractal() { initialize(); }

	Fractal(Vector_3 center)
	{
		_fractal.push_back(center);
		initialize();
	}

	Fractal(double df, double prefactor, double _overlap = 1.0)
	{
		initialize();
		_Df = df;
		_kf = prefactor;
		_oL = _overlap;
	}

	void initialize()
	{	
		srand((unsigned int)time(NULL));

		_a = MEAN_RADIUS;
		_N = 0;
		_Df = FRACTAL_DIMENSION;
		_kf = PREFACTOR;
		_Rg = 0;
		_oL = 1.0;
		
		_Rmean.initialize(0.0f, 0.0f, 0.0f);
		_Rsum.initialize(0.0f, 0.0f, 0.0f);
		_Rsquared = 0;
	}

	~Fractal(){}

	//
	// METHODS
	//

	bool add_monomer(GLdouble x, GLdouble y, GLdouble z);
	bool add_monomer(Vector_3 new_monomer);
	
	Vector_3 create_random_attached_monomer();
  
	bool create_monomer();
	void clear();
	
	bool MonteCarlo();
	double N();
	bool overlap(const Vector_3 & monomer);
	void parameter_update(); //Update based on maintained SUM(R^2) and Rmean 
	void parameter_update_complete(); //Update and calculate new sums.
  
	double test_rg(Vector_3 monomer);
  
	Vector_3 random_monomer();
	void remove_last();
	Vector_3 return_last();		

	unsigned int size()			  { return _N; }
	void set_Df(double Df)		  { _Df = Df; }
	double get_Df()				  { return _Df; }
	void set_kf(double kf)		  { _kf = kf; }
	double get_kf()				  { return _kf; }
	double radius()				  {	return _a; }
  
  
	Vector_3 rmean()			  { return _Rmean; }
	Vector_3 cm()				  { return _Rmean; }
	double Rg()					  { return _Rg; }
  
  
  int begin()					  { return 0; }
  int end()						  { return _fractal.size(); }
  Vector_3 & grab(int i)		  {	return _fractal[i]; }
  Vector_3 operator[](int i)	  { return _fractal[i]; }

	vector<Vector_3> & monomers() {return _fractal;}
	
	//Structure Constant stuff
	void create_box();
	void structurec(ofstream & output);
	
	void orient_random(ofstream & output, unsigned int num_trials = 25, bool stablize = true);
	void rotate_newZ( Vector_3 newZ );
  
	//Begin ConvexHullCodes
	Vector_3& max_point(Vector_3 direction);
	Vector_3& min_angle_point(Vector_3 head, Vector_3 tail, Vector_3 dir);
	bool find_stable_vector( Vector_3 seed, Vector_3 & stableOut );
	void find_first_facet( vector<Vector_3>& facets, Vector_3 seed );
	void find_next_facet( vector<Vector_3>& facets );
	Vector_3 last_facet_normal( vector<Vector_3> facets );
	bool cm_proj_in_facet( vector<Vector_3>& facets );
	
	bool check_last_facet( vector<Vector_3>& _facets );
	///End ConvexHull

	static bool last_trial;

	vector<Vector_3> points;
	bool box_created;

	vector<Vector_3> _facets; //Temporary debug for surface detection
	
private:
	vector<Vector_3> _fractal; // A vector of the centers of the monomers
	
  
	//Overlap Param
	double _oL;  // in percent
	//Fixed parameters
	double _Df, _kf, _a;
  
	//For linear time calculation of Rg
	Vector_3 _Rsum;  
	double _Rsquared;
	
	//Parameters which are updated from the above.
	Vector_3 _Rmean;
	double _Rg;
	int _N;
	
	double box_length;
	int num_parts;
};

double psdrand(int);

#endif