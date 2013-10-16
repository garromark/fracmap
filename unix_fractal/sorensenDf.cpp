/*
 * perdf.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <math.h>
#include <assert.h>

#include "settings.h"
#include "fractal.h"
#include "micrograph.h"

const unsigned int GRID_STEPS = 25;
const int SLOPE_SEP = 20;
const int MAX_DSTEPS = 1000;

const double GRID_MIN_SIZE = 1;
const double GRID_MAX_SIZE = 40;

/* Sorensen Fractal Df */
pair<double,double>
Micrograph::perdf()
{
	try
	{
		LinRegression<double, double> plot;
		vector<bool> grid_col(pixels.size(), false);
		vector<vector<bool> > grid(pixels.size(), grid_col);
		vector<double> perimeterval, gridval;
		vector<bool> colResizer;

		double gratio = GRID_MAX_SIZE / GRID_MIN_SIZE;	

		for (unsigned int grids = 0; grids < GRID_STEPS; grids++)					//cycle through gridsizes
		{
			double gpower=(double)grids/(double)(GRID_STEPS - 1);
			double grid_size = (double) GRID_MIN_SIZE * pow(gratio, gpower);		//calculate current size of grid
			int grids_on_side=(int)(max(rows, cols) / (double)grid_size);			//We want a square grid for this calculation.
			
			colResizer.resize( grids_on_side+1, false );
		    grid.clear();
			grid.resize(grids_on_side + 1, colResizer);
			
			assert(grid.size() == grid[0].size());

			for(vector<Vector_2>::iterator i = points.begin(); i < points.end(); i++)
			{
				grid[(int)floor((double)i->x/grid_size)]
					[(int)floor((double)i->y/grid_size)] = true;
			}

			int perimeter_pts = 0;

			for(unsigned int i = 0; i < grid.size(); i++)
			{
				for(unsigned int j = 0; j < grid.size(); j++)
				{
					if(i > 0 && i < (grid.size() - 1) &&
						j > 0 && j < (grid.size() - 1))
					{
						if(grid[i - 1][j] &&
							grid[i][j - 1] &&
							grid[i][j + 1] &&
							grid[i + 1][j]) continue;
					}

					if(grid[i][j])
					{
						perimeter_pts++;
					}
				}
			}

			perimeterval.push_back(perimeter_pts);
			gridval.push_back(grid_size);
		}

		for(unsigned int i = 0; i < GRID_STEPS; i++)
		{
			if(i > gridval.size() - 1) cout << "Error 2" << endl;
			if(log((double)gridval[i]) > 1.0)
			{
				//TODO: Figure this out.
				//if ( i < (GRID_STEPS - SLOPE_SEP) ) //Add it to the plot, or add it to the plot?
				//{
					plot.add_x(log((double)gridval[i]));
					plot.add_y(log((double)perimeterval[i]));
				//}
				//else
				//{
				//	plot.add_x(log((double)gridval[i]));
				//	plot.add_y(log((double)perimeterval[i]));
				//}
			}
		}

		pair<double, double> ret( -1 * plot.slope(),plot.Rsquared());

	    //if( ret.first > 2 )
		//{
		//  ret.first = 1 + pow(3 - ret.first, 3.0/2.0);
		//}
	  
		return ret;
	}
	catch(...)
	{
		throw 2;
	}
}
