/*
 * 3dstructure.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;

#include "fractal.h"
#include "settings.h"

const int STRUCTURE_ITER = 500;
const int ANG_ITER = 100; // Was 2000
const int STRUCTURE_PRECISION = 1000;

/* Structure factor of the fractal */
void Fractal::structurec(ofstream & output)
{
	double qmin, qmax, qfactor, qval, Angsum;
    double Resum, Imsum;
	double dotprod;
	double Sqtotsum;
	Vector_3 qvec;

	output << "Qcount, Qval, Angsum\n";

	create_box();
	cout << points.size() << endl;

	cout << "Vector_3 coordinates stored." << endl;

	qmin = 2 * PI / num_parts * 0.1;
	qmax = 2 * PI / (2 * _a * PARTS_PER_RADIUS) * 2.0;
	qfactor = pow( (qmax / qmin), (1.0 / ((double)STRUCTURE_ITER - 1.0)) );

	for(int i = 0; i < STRUCTURE_ITER; i++)
	{
		qval = qmin * pow( qfactor, i );
		Angsum = 0.0;

		for(int j = 0; j < ANG_ITER; ++j)
		{	
		    qvec = RandomVec();
			qvec *= qval;
		  
		    Resum = 0.0;
			Imsum = 0.0;

			for(int k = 0; k < STRUCTURE_PRECISION; k++)
			{
				int rand_pt = rand() % (int)points.size();

			    dotprod = points[rand_pt] * qvec;
			  
				Resum += cos(dotprod);
				Imsum += sin(dotprod);
			}
			Resum *= (double) points.size() / STRUCTURE_PRECISION;
			Imsum *= (double) points.size() / STRUCTURE_PRECISION;

			Sqtotsum = ((Resum*Resum + Imsum*Imsum) / points.size()*points.size());
			Angsum  += Sqtotsum / ANG_ITER;
		}

		cout  << i + 1 << ", " << log(qval) << ", " << log(Angsum) << endl;
		output << i + 1 << ", " << log(qval) << ", " << log(Angsum) << endl;
	}
}


void Fractal::create_box()
{
	if(box_created)
		return;
	else
	{		
		Vector_3 temp;

		box_length = 0.0;

		for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
		{
			if(_Rmean.distance(*iter) > box_length) box_length = _Rmean.distance(*iter);
		}

		box_length += _a + 1.0;
		box_length *= 2;

		num_parts = (int)(box_length/_a * PARTS_PER_RADIUS);


		for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
		{
			for(int xcoord = 0; xcoord <= (PARTS_PER_RADIUS * 2 + 1); ++xcoord)
			{
				for(int ycoord = 0; ycoord <= (PARTS_PER_RADIUS * 2 + 1); ++ycoord)
				{
					for(int zcoord = 0; zcoord <= (PARTS_PER_RADIUS * 2 + 1); ++zcoord)
					{
						temp.initialize((GLdouble)xcoord/PARTS_PER_RADIUS - _a,
							(GLdouble)ycoord/PARTS_PER_RADIUS - _a,
							(GLdouble)zcoord/PARTS_PER_RADIUS - _a);
						temp += *iter;
						if(temp.distance(*iter) < _a)
						{	
							points.push_back(temp);
						}
					}
				}
			}
		}
		box_created = true;
	}
}