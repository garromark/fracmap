/*
 * fractal.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <fstream>

#include "settings.h"
#include "fractal.h"
using namespace std;

/* Fractal::create_monomer()
 * -------------------------
 * Corresponds with AddNewMonomer() from the literature.
 * Input: Fractal of size N
 * Output: Fractal of size N+1 such that
 * relevant desired parameters remain static.
 */
bool Fractal::create_monomer()
{
	if(_fractal.empty())
	{
		add_monomer(0.0f, 0.0f, 0.0f);
	}
	else if(_fractal.size() == 1)
	{
	  add_monomer(create_random_attached_monomer());
	}
	else
	{
		return MonteCarlo();
		//There is no good reason to have 100 trials here as WELL as 100 trials inside MonteCarlo.
		//Just obfusicates how many trials we're really doing (IE: 500*100, instead of just 500, or 50000)
	}

	return true;
}

void Fractal::clear()
{
	_fractal.clear();
	points.clear();
	parameter_update();
}

/* Fractal::MonteCarlo()
 * ---------------------
 * In order to add a random monomer that will retain the expected Rg of
 * the subsequent fractal, we first copy the fractal, then we choose a
 * random monomer of the fractal to be the target monomer, onto which
 * the new monomer will be added.  Create monomer at a distance of
 * 2*monomer radius from the center of the target.  Calculate the
 * resulting Rg and ensure no overlap. If the resulting monomer retains
 * Rg within EPSILON of the expected value and the absolute difference
 * of the expected value and the calculated Rg is less than any
 * previously chosen monomers, then we choose the current random monomer
 * to be our new addition.  Iterate for at least NUM_OF_TRIALS trials
 * and iterate this entire selection process RECURSIVE_FACTOR number of
 * times.  Return true if a successful addition could be found, else return false.
 */

//The previous description needs to be totally updated because I've SIGNIFICANTLY changed this function.
bool Fractal::MonteCarlo()
{
	bool success = false;
	double expected_Rg = pow(((_N + 1)/_kf), 1/_Df) * _a;
    
	Vector_3 temp_monomer, best_monomer(1000.0f, 1000.0f, 1000.0f); // Initialized out of range
	double temp_Rg, best_Rg = expected_Rg * 1000;//Initialized out of range	

	for(int j = 0; j < MAX_MONOMER_TRIES ; j++) // Try MAX_MONOMER_TRIES
	{
	  temp_monomer = create_random_attached_monomer();
	  temp_Rg = test_rg(temp_monomer);
	  
		if(abs(temp_Rg - expected_Rg) < abs(best_Rg - expected_Rg))
		{
		  success = true;
		  best_Rg = temp_Rg;
		  best_monomer = temp_monomer;
		}
	}

	//if( success && abs(temp_opt.Rg() - expected_Rg) < EPSILON)
	//{
		//The following should always return true,
		//but return the actual value so call chain can see if any
		//hanky panky is going on.
		return add_monomer(best_monomer);
	//}

	return false;
}

//See how adding `monomer' would affect the radius of gyration.
double Fractal::test_rg(Vector_3 monomer)
{
  Vector_3 temp_Rmean = (_Rsum + monomer)/(_N+1);
  double temp_Rsquared = _Rsquared + monomer * monomer;
  
  return sqrt(temp_Rsquared/(_N+1) - (temp_Rmean * temp_Rmean));
}

//This updates _Rg from the maintained sum of Radii and R^2
void Fractal::parameter_update()
{
  _N = (int)_fractal.size();
  _Rmean = _Rsum / _N;
  _Rg = sqrt(_Rsquared/_N- (_Rmean*_Rmean)); //Formula for standard deviation
}

//This will update the R_g and other parameters of the fractal from scratch
//This is not currently used in the code though, as progressive updates are used instead to speed up the algorithm.
void Fractal::parameter_update_complete()
{
	double Rg_sum = 0;

	_N = (int)_fractal.size();
  
	_Rsum.initialize(0.0, 0.0, 0.0);
	for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
	{
	  _Rsum += *iter;
	  _Rsquared += (*iter) * (*iter);
	}
	_Rmean = _Rsum / _N;

	for(vector<Vector_3>::const_iterator iter = _fractal.begin(); iter != _fractal.end(); ++iter)
	{
		Rg_sum += _Rmean.square_difference(*iter);
	}
	_Rg = sqrt(Rg_sum / _N);

	box_created = false;
}


//Return the last monomer added.
Vector_3 Fractal::return_last()
{
	return _fractal.back();
}

//Obtain a random monomer from the agglomerate
Vector_3 Fractal::random_monomer()
{
	return _fractal[rand() % _fractal.size()];
}

//Create a monomer and attach it randomly to the fractal such that it doesn't overlap beyond the accepted parameter.
Vector_3 Fractal::create_random_attached_monomer()
{
  Vector_3 a_center, new_monomer;
  
  do
  {
	new_monomer = RandomVec(  );  //Get a randomly distributed unit vector for where to attach the next monomer.
	a_center = random_monomer();  //Pick a random monomer to attach to.
	new_monomer *= 2 * _a * _oL;        //Grow the unit vector to twice the monomer radius
	new_monomer += a_center;      //Translate the vector/monomber to the location of the new center.
  } while( overlap(new_monomer) ); 
  //No trial counter as there is ALWAYS somewhere to add a monomer that doesn't overlap
  //Should be relatively easy.
  
  return new_monomer; //Return this random monomer
}

//Add a monomer as below, but through coordinates rather than a Vector_3 class.
bool Fractal::add_monomer(GLdouble x, GLdouble y, GLdouble z)
{
	Vector_3 new_monomer;
	
	new_monomer.x = x;
	new_monomer.y = y;
	new_monomer.z = z;
	
	return add_monomer(new_monomer);
}


//Add a monomer, and the associated value of R's sum.
bool Fractal::add_monomer(Vector_3 new_monomer)
{
  _fractal.push_back(new_monomer);
  _Rsum += new_monomer;
  _Rsquared += new_monomer * new_monomer;
	  
  parameter_update();
  
  return true;
}


//Remove the last monomer, and the associated value of R's sum.
void Fractal::remove_last()
{
  Vector_3 lastMonomer = _fractal.back();
  _fractal.pop_back();	
  
  _Rsum -= lastMonomer;
  _Rsquared -= lastMonomer * lastMonomer;
  
  parameter_update();
}

/* Returns true if monomers are point contact (do not overlap) */
/* Changes by SC on 1/9/09 to check if a PARTICULAR monomer overlaps */
bool Fractal::overlap(const Vector_3 & monomer)
{
  
  //TODO:  This should be updated to use an octtree for collision detection.
	for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
	{
		if(//iter != monomer && //TODO: Fix this!
		    //Until this is fixed, assuming it's checking for overlap on UNADDED monomer
 		   monomer.distance(*iter) < (2 * _a * _oL - 0.01)) //This is rudimentary
		{
			return true; //There was an overlap
		}
	}

	return false;
}
