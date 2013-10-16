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

//Mass ball counting.
pair<double, double> Micrograph::Df()
{
	return boxcount_ball( points );
}

//Boundary ball Counting.
//pair<double,double> Micrograph::perimeterDf()
//{		
//	return boxcount_ball( boundary );
//}

//Mass box counting.
pair<double, double> Micrograph::Df_grid()
{
  return boxcount_grid( points );
}

//Boundary Box Counting.
// Take Boundary of original image; Do box counting on this same boundary resized
pair<double,double> Micrograph::perimeterDf_grid()
{		
  return boxcount_grid( boundary );
}

/* Open Ball Box Counting */
pair<double, double> Micrograph::boxcount_ball( const vector<Vector_2>& space ) const
{
  //TODO:  These should potentially be the cm() and Rg() of the space parameter, not of the micrograph.
  Vector_2 center = cm();
	vector<vector<Vector_2> > freqArr;
	LinRegression<double,double> plot;
	
	vector<Vector_2> col; col.resize(0);
	freqArr.resize( ceil(Rg()), col ); //Number of slots	
	
	//Sort them by radius
	for(vector<Vector_2>::const_iterator iter = space.begin(); iter != space.end(); iter++)
	{
	  //TODO: Check this.
	  unsigned int i = floor( center.distance( *iter ) );// / _monomer_radius ); // *_monomer_radius
		
		if( i >= freqArr.size() ) continue; //Cutoff at the Radius of Gyration
	  
		//Put them in the R boxes
		freqArr[i].push_back( (*iter) );
	}
	
	//We're now doing the log-log data plot.
    int count = 0;
	double Rg_i_sum = 0;
  
	for( unsigned int i = 0; i < freqArr.size(); i++ )
	{
		count += freqArr[i].size();
		
		//Log goes to -inf quickly for small values.
		//This will give NANs in the linear regression.
		//We must filter them out.
		//Also throw out the first couple data points as they throw off the counts.
		for(vector<Vector_2>::iterator iter = freqArr[i].begin(); iter != freqArr[i].end(); iter++)
		{
			//Don't reset this each iteration so that it stays N complexity, instead of N^2
			Rg_i_sum += center.square_difference(*iter);
		}
		  
		double Rg_i = sqrt(Rg_i_sum / count);
		double Nproj_i =  count;
		
		if( log(Rg_i) > 1 && log(Nproj_i) > 1 ) //Small errors blow up before this value.
		{
		  plot.add_x(log(Rg_i)); // Log - Log Scale
		  plot.add_y(log(Nproj_i) );
		}
	}
  
	
	pair<double, double> _Df;
	
	_Df.first = plot.slope();
	_Df.second = plot.Rsquared();
  
	return _Df;
}


/* Box Counting */
pair<double, double> Micrograph::boxcount_grid( const vector<Vector_2>& space ) const
{
  const unsigned int GRID_STEPS = 25;
  //const int SLOPE_SEP = 20;
  //const int MAX_DSTEPS = 1000;
  
  const double GRID_MIN_SIZE = 1;
  const double GRID_MAX_SIZE = 40;
  
  LinRegression<double, double> plot;
  vector<vector<bool> > grid;
  vector<double> volval, gridval;
  vector<bool> colResizer;
  
  double gratio = GRID_MAX_SIZE / GRID_MIN_SIZE;	
  
  for (unsigned int grids = 0; grids < GRID_STEPS; grids++)					//cycle through gridsizes
  {
	double gpower=(double)grids/(double)(GRID_STEPS - 1);
	double grid_size = (double) GRID_MIN_SIZE * pow(gratio, gpower);		//calculate current size of grid
	int grids_on_side=(int)(max(rows, cols) / (double)grid_size);			//Square grid for this calculation.
	
	colResizer.resize( grids_on_side+1, false );
	grid.clear();
	grid.resize(grids_on_side + 1, colResizer);
	
	assert(grid.size() == grid[0].size());
	
	for(vector<Vector_2>::const_iterator i = space.begin(); i < space.end(); i++)
	{
	  grid[(int)floor((double)i->x/grid_size)]
	      [(int)floor((double)i->y/grid_size)] = true;
	}
	
	int volume = 0;
	
	for(unsigned int i = 0; i < grid.size(); i++)
	{
	  for(unsigned int j = 0; j < grid.size(); j++)
	  {
		if(grid[i][j])
		{
		  volume++;
		}
	  }
	}
	
	volval.push_back(volume);
	gridval.push_back(grid_size);
  }
  
  if(GRID_STEPS > gridval.size() ) cout << "Error 2" << endl;
  
  for(unsigned int i = 0; i < GRID_STEPS; i++)
  {
	if(log((double)gridval[i]) > 1.0)
	{
	  plot.add_x(log((double)gridval[i]));
	  plot.add_y(log((double)volval[i]));
	}
  }
  
  pair<double, double> df( -1 * plot.slope(), -1 * plot.Rsquared());
  
  return df;  
}