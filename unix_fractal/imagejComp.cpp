#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

#include "micrograph.h"

using namespace std;

void parseFile(char* filename, vector<vector<bool> > & img )
{
  ifstream imageJfile( filename );
  string s;
  
  vector<bool> row;
  
  while( getline(imageJfile,s) ) {
  	istringstream ss(s);
	double d;
	
	row.clear();
	while( ss >> d ) {
	  row.push_back(d == 0);
	}
	img.push_back(row);
  }
  
  return;
}

void printAnalysis( Micrograph & micro )
{
  cout << "Area, Perimeter, Lmax, width, 2d D (Mass Ball), R^2, 2d D (Mass Grid), R^2, 2d D (Perim Grid), R^2, 2d D (Sorensen), R^2, 2d Rg" << endl;
  
  pair<double, double> dim = micro.Df();
  pair<double, double> perDf = micro.perdf(); //Sorensen
  pair<double, double> dim_grid = micro.Df_grid();
  pair<double, double> perimDf_grid = micro.perimeterDf_grid();
  
  double length = 0, width = 0 ;
  micro.dimensions( length, width );
  
  //cout << micro.Nproj() << ", ";            // Nproj
  cout << micro.Area() << ", ";			  // Area
  cout << micro.Perimeter() << ", ";
  cout << length << ", ";					  // Lmax
  cout << width << ", ";					  // Width based on length
  cout << dim.first << ", ";                // 2d Df (Box Counting)
  cout << dim.second * dim.second << ", ";  // R-squared (of above)
  
  cout << dim_grid.first << ", ";		      //
  cout << abs(dim_grid.second) << ", ";     // R-squared (of above)
  cout << perimDf_grid.first << ", ";		  //
  cout << abs(perimDf_grid.second) << ", "; // R-squared (of above)
  
  cout << perDf.first << ", ";			  // 2d Perimeter Df Sorrenson
  cout << abs(perDf.second) << ", ";        //R-squared (of above)
  cout << micro.Rg() << ", ";               // 2d Rg
  cout << endl;
}


int main(int argc, char** argv)
{
  if( argc != 2 )
  {
	cout << "Must specify filename as single parameter";
	exit(0);
  }
  vector<vector<bool> > img;
  parseFile( argv[1], img );
  
  cout << "foobar" << endl;
  
  cout << img.size() << endl << img[0].size() << endl;
  
  Micrograph m( img );
  
  
  printAnalysis( m );
  
  
  return 0;
}
