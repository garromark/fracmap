/*
 * micrograph.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "settings.h"
#include "fractal.h"
#include "micrograph.h"
using namespace std;

Vector_2 Micrograph::cm() const
{
	return _cm;
}

double Micrograph::Rg() const
{
  return _Rg;// / _monomer_radius;
}

double Micrograph::Area() const
{
  return points.size();// / (_monomer_radius * _monomer_radius);
}

double Micrograph::Perimeter() const
{
  return boundary.size();// / _monomer_radius;
}

double Micrograph::Nproj() const
{
  return points.size() / (PI *(_monomer_radius*_monomer_radius));
}

//SC - 2009-03-19
//Deleted old length and width algorithms.
//Keeping n^2 length algorithm for now.
//TODO: Max_width is being calculated incorrectly, and can be done in linear time instead of n^2 time.
//Changing this algo to run on the ALREADY generated boundary.  Only boundary points can give maximums anyways.
void Micrograph::dimensions(double &length, double& width) const
{
  double max = 0.0;
  Vector_2 ref1(0,0), ref2(0,0);
  
  //If we perform this algorithm on the BOUNDARY we will have to look at much fewer points.
  for(unsigned int i = 0; i < boundary.size(); i++)
  {
	  for(unsigned int j = i + 1; j < boundary.size(); j++)
	  {
		  double dist = boundary[i].distance(boundary[j]);
		
		  if(dist > max || (i == 0 && j == 1))
		  {
			max = dist;

			ref1 = boundary[i];
			ref2 = boundary[j];
			//cout << "Dist: " << dist << " " << (ref1 - ref2).magnitude() << endl;
		  }
	  }
  }
  
  //Vector_2 ortho = Vector_2( 1, - (ref1.x - ref2.x)/ (ref1.y - ref2.y) ).normalize(); //Orthogonal slope.
  Vector_2 maxLengthVec = (ref1-ref2).normalize();
  double maxw_max = 0, maxw_min = 0;
  //NOTE:
  //with min and max being ZERO it might SEEM as if nothing would hit that minimum or maximum. 
  //But our legnth reference points above will have a zero dot product with this vector.
  for(unsigned int i = 0; i < boundary.size(); i++)
  {
	//double projection = -1 * ( boundary[i] * ortho ); //Orthogonal projection.
	double projection = boundary[i].X(maxLengthVec);
		
	if( projection < maxw_min || i == 0 ) maxw_min = projection;
	if( projection > maxw_max || i == 0) maxw_max = projection;
  }
  
	//SC: Deleted _Fractal.radius().   This algo is not running over monomers, but pixels of the projections.  Length is proper without this.
//  length = max + 2*_fractal.radius()/_monomer_radius;
//  width = max_w * 2*_fractal.radius()/_monomer_radius;
  
  length = (max);// /_monomer_radius;
  width = (maxw_max - maxw_min);// /_monomer_radius;
}

void Micrograph::calc_info()
{
	_cm = Vector_2(0.0, 0.0);
	
	for(vector<Vector_2>::iterator iter = points.begin(); iter != points.end(); iter++)
	{
		_cm += *iter;
	}
	_cm = _cm / points.size();	
	
	double Rg_sum = 0.0;
	
	for(vector<Vector_2>::iterator iter = points.begin(); iter != points.end(); iter++)
	{
		Rg_sum += _cm.square_difference(*iter);
	}
	
	_Rg = sqrt(Rg_sum / points.size());
}

void Micrograph::make_pixels(Fractal & _fractal)
{
	double min_x = 100000000.0, max_x = -100000000.0;
	double min_y = 100000000.0, max_y = -100000000.0;
	
	double ri = _fractal.radius();
	
	for(vector<Vector_3>::iterator itr = _fractal.monomers().begin(); itr < _fractal.monomers().end(); itr++)
	{
		//Include the boundary of the monomers in the min and max values
		if(itr->x - ri < min_x) min_x = itr->x - ri;
		if(itr->y - ri< min_y) min_y = itr->y - ri;
		if(itr->x + ri> max_x) max_x = itr->x + ri;
		if(itr->y + ri > max_y) max_y = itr->y + ri;
	}
		
	double xSize = max_x - min_x, ySize = max_y - min_y;
	unsigned int m_len = (int)ceil(max( xSize, ySize) + 2 * ri ) * PARTS_PER_RADIUS;
	
	m_len = m_len + 8 - m_len % 8;
	
	vector<bool> col;
	col.resize( m_len, false); //Boxes per unit 
	
	pixels.clear();
	pixels.resize(m_len, col);
  
	rows = cols = m_len; // Very important initializer
	
	for(vector<Vector_3>::iterator itr = _fractal.monomers().begin(); itr < _fractal.monomers().end(); itr++)
	{	
		Vector_3 curr_pt = *itr;
		
		for(double j = -ri; j < ri; j += 0.05)
		{
			for(double k = -ri; k < ri; k += 0.05)
			{
				if( ((j)*(j) + (k)*(k)) < ri * ri)
				{
					pixels[(int)floor((curr_pt.x - min_x + j) * PARTS_PER_RADIUS)]
						  [(int)floor((curr_pt.y - min_y + k) * PARTS_PER_RADIUS)]
							= true;
				}
			}
		}
	}
}

//SC - 2009/03/16
//Changing this to depend on make_pixels() instead of the _fractal's points
//This way we can simplify this to an N*N algo from an N*N*M
//Makepixels is an M complexity algo. This is will become O(M + N*N) instead.
void Micrograph::make_points() 
{
	points.clear();
	Vector_2 center(cols/2, rows/2);
	
	for(int x = 0; x < rows ; x++)
	{
		for(int y = 0; y < cols; y++)
		{
			Vector_2 temp((double)x, (double)y);
					
			if(pixels[x][y])
			{
				points.push_back(temp);
			}
		}
	}
}

//Makes a boundary points array
void Micrograph::make_boundary() 
{
	bool foundAdj = false;
	
	//Leave the outside of the box alone, for the purpose of finding the perimeter.
	//We need to examine the surrounding points to see if it is on the perimeter
	for(int x = 0; x < rows; x++)
	{
		for(int y = 0; y < cols-1; y++)
		{
			Vector_2 temp((double)x, (double)y);
			
			if(pixels[x][y])
			{
				foundAdj = false;
			  
				if( x == 0 || y == 0 || x == cols-1 || y == rows -1)
				  foundAdj = true;
				else
				  //Loop through all 8 boundary points.
				  for( int i = -1; i <= 1; i += 1 ) // -1, 0, 1
					for( int j = -1; j <= 1; j += 1 ) // -1, 0, 1
					  if( ! pixels[x + i][y + j]) foundAdj = true; // Will be true if any of these are true.
			  
			  if( foundAdj ) {
				boundary.push_back(temp) ;
			  }
			}
		}
	}
}


//Returns a square bit array.
//Make sure to deallocate this yourself!
char* Micrograph::makeBitarray(vector<vector<bool> > & pixelArr, unsigned int & sideSize)
{
	vector<vector<bool> >::iterator rowItr;
	vector<bool>::iterator colItr;
		
	unsigned int pixel = 0;
	sideSize = pixelArr.size() ;
	
	const unsigned int bitsPerInd = 8;
	
	unsigned int arrSize = ((sideSize * sideSize) / bitsPerInd) + 1;
	char * bitarray = ( char*)malloc( arrSize );
	
	for( unsigned int i = 0; i < arrSize; i++)
		bitarray[i] = 0;
	
	//pixels is a rectangular array;
	
	for ( rowItr = pixelArr.begin(); rowItr != (pixelArr).end(); rowItr++ )
	{
		//if( (*rowItr).size() != width )
		//{
		//	throw AnException;
		//}
		
		for ( colItr = (*rowItr).begin(); colItr != (*rowItr).end(); colItr++ )
		{
			int ind = pixel / bitsPerInd;
			int bit = pixel % bitsPerInd;
			
			bitarray[ ind ] |= (*colItr << bit);
			
			pixel++;
		}
	}
	
	return bitarray;
}

char* Micrograph::getBitArray(unsigned int & sideSize)
{
  return makeBitarray(pixels, sideSize);
}

char* Micrograph::getBoundaryBitArray(unsigned int & sideSize)
{
    
  vector<vector<bool> > boundaryPixels;
  make_boundary_pixels( boundaryPixels );  //We generate a boundary, but not a bitmake for the boundary
  return makeBitarray(boundaryPixels, sideSize);
}


//This is for debug visualization.
void Micrograph::make_boundary_pixels(vector<vector<bool> > & boundaryPixels)
{
	double min_x = 100000000.0, max_x = -100000000.0;
	double min_y = 100000000.0, max_y = -100000000.0;
	
  double ri = 1.0;
	
	for(vector<Vector_2>::iterator itr = boundary.begin(); itr < boundary.end(); itr++)
	{
		//Include the boundary of the monomers in the min and max values
		if(itr->x - ri < min_x) min_x = itr->x - ri;
		if(itr->y - ri< min_y) min_y = itr->y - ri;
		if(itr->x + ri> max_x) max_x = itr->x + ri;
		if(itr->y + ri > max_y) max_y = itr->y + ri;
	}
	
	double xSize = max_x - min_x, ySize = max_y - min_y;
	unsigned int m_len = (int)ceil(max( xSize, ySize) + 2 * ri );
	
	m_len = m_len + 8 - m_len % 8;
	
	vector<bool> col;
	boundaryPixels.clear();
	col.resize( m_len, false);
	
	boundaryPixels.resize(m_len, col);
	
	for(vector<Vector_2>::iterator itr = boundary.begin(); itr < boundary.end(); itr++)
	{	
		//Vector_3 curr_pt = *itr;
		Vector_2 curr_pt = *itr;
		
		if( curr_pt.x - min_x < m_len && curr_pt.y - min_y < m_len &&
			curr_pt.x - min_x >= 0 && curr_pt.y - min_y >= 0)
			boundaryPixels[int((curr_pt.x - min_x))]
				[int((curr_pt.y - min_y))]
						= true;
	}
}
