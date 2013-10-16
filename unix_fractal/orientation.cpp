/*
 * orientation.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>

#include "settings.h"
#include "fractal.h"
#include "micrograph.h"

using namespace std;


void Fractal::orient_random(ofstream & output, unsigned int num_trials, bool stablize)
{
  unsigned int success = 0;
  Vector_3 seed,newOrientation;
	
  Vector_3 backupCM;
  vector<Vector_3> backupMonos;

  backupCM = cm();
  backupMonos = _fractal;

  output << "Nproj, Area, Perimeter, N, Lmax, width, 2d D (Mass Ball), R^2, 2d D (Mass Grid), R^2, 2d D (Perim Grid), R^2, 2d D (Sorensen), R^2, 3d D, 2d Rg, 3d Rg" << endl;
	
  while(success < num_trials)
    {
      try
	  {			
	  seed = newOrientation = RandomVec();
	  if( !stablize || find_stable_vector( seed, newOrientation ) ) //Second function is not called if stablize is false, thus we need to set newOrientation Correctly first.
	    {
	      rotate_newZ(newOrientation);

	      Micrograph micro(*this);

	      pair<double, double> dim = micro.Df();
	      pair<double, double> perDf = micro.perdf(); //Sorensen
		  pair<double, double> dim_grid = micro.Df_grid();
		  pair<double, double> perimDf_grid = micro.perimeterDf_grid();
		  
		  double length = 0, width =0 ;
		  micro.dimensions( length, width );

	      output << micro.Nproj() << ", ";            // Nproj
	      output << micro.Area() << ", ";			  // Area
		  output << micro.Perimeter() << ", ";
	      output << _fractal.size() << ", ";          // N
	      output << length << ", ";					  // Lmax
	      output << width << ", ";					  // Width based on length
	      output << dim.first << ", ";                // 2d Df (Box Counting; Ball or circle)
	      output << dim.second * dim.second << ", ";  // R-squared (of above)
		  
		  output << dim_grid.first << ", ";		      // 2d Df (Box Count; Grid; not from sorrenson's code)
	      output << abs(dim_grid.second) << ", ";     // R-squared (of above)
		  output << perimDf_grid.first << ", ";		  // Hostraff 2Df of perimeter by definition; Take Boundary of original image; Do box counting on this same boundary resized
	      output << abs(perimDf_grid.second) << ", "; // R-squared (of above)
		  
	      output << perDf.first << ", ";			  // 2d Perimeter Df Sorrenson; resize image, retake boundary
	      output << abs(perDf.second) << ", ";        //R-squared (of above)
	      output << _Df << ", ";                      // 3d Df
	      output << micro.Rg() << ", ";               // 2d Rg
	      output << _Rg ;							  // 3d Rg
	      output << endl;
				
	      success++;
	      cout << "successess = " << success << " of " << num_trials << ": " << _Df << " " << _fractal.size() << endl;
	    } else
	    {
	      cout << "failure" << endl;
	    }
	}
      catch(int err)
	{
	  cout << "Error: " << err << endl;
	}
      catch(...)
	{
	  cout << "Unknown Error" << endl;
	}


      //Restore the fractal to the original points after a rotation
      //In order to avoid compounding floating point errors due to repeated rotations
      _Rmean = backupCM;
      _fractal = backupMonos; 
    }
  
  _facets.clear(); //We don't want these showing up on any viewports as they no longer make sense.
}


//This will rotate vector newZ to the new Z vector.
//This is used after finding a stable orientation.
void Fractal::rotate_newZ( Vector_3 newZ )
{
  //Assume newZ is a unit vector
	
  Vector_3 Temp;
	
  double RotMatrix[9];
	
  VectorToMatrix( newZ, RotMatrix );
	
  for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
    {
      Temp = *iter;
		
      //Do not rotate around the center of mass!
		
      iter->x = RotMatrix[0] * Temp.x     + RotMatrix[1] * Temp.y     + RotMatrix[2] * Temp.z;
      iter->y = RotMatrix[0 + 3] * Temp.x + RotMatrix[1 + 3] * Temp.y + RotMatrix[2 + 3] * Temp.z;
      iter->z = RotMatrix[0 + 6] * Temp.x + RotMatrix[1 + 6] * Temp.y + RotMatrix[2 + 6] * Temp.z;
    }
	
  //Rotate the center.
  Temp = _Rmean;
	
  _Rmean.x = RotMatrix[0] * Temp.x     + RotMatrix[1] * Temp.y     + RotMatrix[2] * Temp.z;
  _Rmean.y = RotMatrix[0 + 3] * Temp.x + RotMatrix[1 + 3] * Temp.y + RotMatrix[2 + 3] * Temp.z;
  _Rmean.z = RotMatrix[0 + 6] * Temp.x + RotMatrix[1 + 6] * Temp.y + RotMatrix[2 + 6] * Temp.z;
  
	
  for(vector<Vector_3>::iterator iter = _facets.begin(); iter != _facets.end(); iter++)
    {
      Temp = *iter;
		
      iter->x = RotMatrix[0] * Temp.x     + RotMatrix[1] * Temp.y     + RotMatrix[2] * Temp.z;
      iter->y = RotMatrix[0 + 3] * Temp.x + RotMatrix[1 + 3] * Temp.y + RotMatrix[2 + 3] * Temp.z;
      iter->z = RotMatrix[0 + 6] * Temp.x + RotMatrix[1 + 6] * Temp.y + RotMatrix[2 + 6] * Temp.z;
    }
}


/////////////////////////////
//EXPERIMENTAL CODE BEGIN
/////////////////////////////

//Find the maximum point in a given direction for the purpose of finding the first facet.
Vector_3& Fractal::max_point(Vector_3 direction)
{
  Vector_3* maxPoint = NULL; 
  double maxVal = 0, tempVal = 0;

  for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
    {
      tempVal = direction * (*iter);
      if( maxPoint == NULL || tempVal > maxVal )
	{
	  maxVal = tempVal;
	  maxPoint = &(*iter);
	}
    }
	
  return *maxPoint;
}

Vector_3& Fractal::min_angle_point(Vector_3 head, Vector_3 tail, Vector_3 dir)
{
  Vector_3* minAnglePoint = NULL;
  Vector_3 tempPoint, edgeVec = (head - tail).normalize();
	
  double maxCosine = 0, tempCosine;
	
  for(vector<Vector_3>::iterator iter = _fractal.begin(); iter != _fractal.end(); iter++)
    {
      if( *iter == head || *iter == tail ) continue; //We don't want the existing vertices to be considered.
		
      tempPoint = (*iter - tail); //We want the angle between dir and *iter with tail at the origin.
      tempPoint -= edgeVec * (tempPoint * edgeVec); //Bring it into the plane of interest
		
      tempCosine = dir * tempPoint; //cosine(theta) * | dir | * | tempPoint |
		
      //TODO:
      //    Currently we assume that this edge is ON the direction LAYS _on_ the facet
      //This allows us to ALSO assume that the cosine is not of a negative angle.
		
      tempCosine /= tempPoint.magnitude();  //Normalize it 
		
      if( minAnglePoint == NULL || tempCosine >= maxCosine ) //Maximum cosine value has smallest angle.
	{
	  maxCosine = tempCosine;
	  minAnglePoint = &(*iter);
	}
    }
	
  return *minAnglePoint;
}


/////////////////////////
//WARNING:
//There are assumptions in certain parts of this convex code about how OTHER parts of this convex hull code works.
//BE CAREFUL MODIFYING
/////////////////////////
bool Fractal::find_stable_vector( Vector_3 seed, Vector_3 & stableOut )
{
  unsigned int count = 1, maxCount = _fractal.size();
	
  _facets.clear();
		
  find_first_facet( _facets, seed );
  count ++;

  //Rarely does this become and infinite loop
  //Exit and fail if that happens
  while( ! cm_proj_in_facet( _facets ) && count < maxCount  )
    {
      find_next_facet( _facets );
      count++;
    }
	
  stableOut = last_facet_normal( _facets );
	
  return count < maxCount;
}


//Debug code.   Checks to see if the facet is on the convex hull.
bool Fractal::check_last_facet( vector<Vector_3>& _facets )
{
  //Check facet.  First Facet seems to be good.
  Vector_3 dir = last_facet_normal( _facets ); //Remember this is inward
  dir *= -1;
  Vector_3 maxPoint = max_point( dir );
  bool facetGood = true;
  for(vector<Vector_3>::iterator iter = _facets.end() - 3; iter < _facets.end(); iter++)
    {
      if( maxPoint*dir >= (*iter)*dir + FILTER_DEPTH )
	{
	  facetGood = false;
	  cout << maxPoint.x << " " << maxPoint.y << " " << maxPoint.z << endl;
	  cout << iter->x << " " << iter->y << " " << iter->z << endl;
	}
    }
	
  return facetGood;
}

//This function seems to work.
void Fractal::find_first_facet( vector<Vector_3>& facets, Vector_3 seed ) 
{
  //NOTE:
  //The way this is done is important so that find_next_facet
  //can properly assume that the edge point1->point2 is away from the center of mass.
  //So this code pics a point and finds the first convex facet that is TOWARD the center of mass
  //from the found point.
	
  Vector_3 point1, point2, point3, fakePoint, proj_cm;
  Vector_3 direction, new_norm;
  point1 = max_point( seed );

  proj_cm = cm() + seed * ( point1 * seed - cm() * seed); //project the CM into the plane with the point.	
  direction = (proj_cm - point1).normalize();  //Find the direction from the point to the production of the center of mass.
  fakePoint = point1 + seed.X(direction); //Create a fake edge
	
  point2 = min_angle_point( point1, fakePoint, direction ); //Find the point with the minimum angle around this edge
	
  Vector_3 initialEdge = (point1 - point2).normalize();
  //AX(B-A) = AXB - AXA = AXB  Thus no need to subtract pivotMono
  direction = proj_cm - point2;						   //Need to find direction toward CM projection
  direction -= initialEdge * (initialEdge * direction);  //Need to orthogonalize it to the edge.
	
  point3  = min_angle_point( point1, point2, direction); 
	
  //a - b : from b to a
  facets.push_back( point1 );
  facets.push_back( point2 );
  facets.push_back( point3 );
}

void Fractal::find_next_facet( vector<Vector_3>& facets )
{
  unsigned int N = facets.size();
  Vector_3 a = facets[N-3], b = facets[N-2], c = facets[N-1];
  Vector_3 avg = a + b + c; 	avg /= 3;

  //We don't need to look out the edge from a->b because there's already a triangle on the other side
  //These subtractions matter, so that the vectors are "counter clockwise" when added together.
  Vector_3 edgeBC = (c - b).normalize(),  edgeCA = (a - c).normalize();
  //We need this for finding which of the above edges the CM exits.
  Vector_3 edgeAB = (b - a).normalize();
	
  //Not using last_facet_norm, because we don't know what direction the edges make on the surface.
  //This should be good for use with the cross products to detect where the point is outside the facet.
  Vector_3 norm = edgeBC.X(edgeCA).normalize(); 
	
  Vector_3 proj_cm = cm() + norm * ( avg * norm - cm() * norm);
  Vector_3 proj_cm_dir = (proj_cm - avg).normalize(); //Give us a direction vector for the edge.	
	
  Vector_3 BC_dir = (proj_cm - b);
  BC_dir = proj_cm_dir - edgeBC * ( edgeBC * BC_dir ); //Perpendicular direction
	
  Vector_3 CA_dir = (proj_cm - a);
  CA_dir = CA_dir - edgeCA * ( edgeCA * CA_dir ); //Perpendicular direction
	
  //If these are positive or negative,
  //it tells us which edge of the triangle the thing is coming out of.
  double outNormBC = BC_dir.X(edgeBC) * norm;
  double outNormCA = CA_dir.X(edgeCA) * norm; 
	
  Vector_3 newVert, direction;
  //Outward normals have a positive cross product.
  if( (outNormBC >= 0 && outNormCA <= 0) || //Strictly to out the BC side
      (outNormBC >= 0 && outNormCA >= 0 && outNormBC >= outNormCA ) || // In the range where it is out through either edge,
      // But edgeBC has more torque.
      (outNormBC <= 0 && outNormBC <= 0 && outNormBC >= outNormCA ) //This is floating point error.
      ) 
    {	
      newVert = min_angle_point( c, b, BC_dir ); 
		
      facets.push_back( b );
      facets.push_back( c );
      facets.push_back( newVert ); // This should NOT be a
    } else {	
    newVert = min_angle_point( c, a, CA_dir ); 
		
    facets.push_back( a );
    facets.push_back( c );
    facets.push_back( newVert );  //This should NOT be b
  }
}

//Works
Vector_3 Fractal::last_facet_normal( vector<Vector_3> facets )
{
  int N = facets.size();
	
  Vector_3 avg = facets[N-1] + facets[N-2] + facets[N-3];
  avg /= 3;
	
  Vector_3 a = facets[N - 1] - facets[N - 3];
  Vector_3 b = facets[N - 2] - facets[N - 3];
	
  Vector_3 normal = a.X(b).normalize();

  if( avg * normal > 0 ) normal *= -1; //Reverse it if it's not an inward normal.
		
  return normal;
}


//This function works.
bool Fractal::cm_proj_in_facet( vector<Vector_3>& facets )
{
  unsigned int N = facets.size();
  Vector_3 norm = last_facet_normal( facets );
  Vector_3 a = facets[N-1], b = facets[N-2], c = facets[N-3];
	
  Vector_3 proj_cm = cm() + norm * ( norm * a - norm * cm()  );
	
  if(abs(tri_area(proj_cm, b, c) + 
	 tri_area(a, proj_cm, c) + 
	 tri_area(a, b, proj_cm) -
	 tri_area(a, b, c)) < 0.05)
    return true;
  else
    return false;
}

/////////////////////////////
//EXPERIMENTAL CODE END
/////////////////////////////
