/*
 * main.cpp
 * --------
 * By Mark Garro
 * Date: September 06, 2007
 *
 * The main function can be run cross-platform and provides access to
 * written utilities for fractal generation and analysis.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include "fractal.h"
#include "settings.h"
#include "micrograph.h"

#include <unistd.h>
#include <sys/wait.h>

using namespace std;

enum {
	ESCAPE,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3
};

bool generate_fractal(Fractal *base, unsigned int fractal_size, bool output)
{
	unsigned int removals = 0;
	
	do
	{
	TRY_ADD_MONOMER:
		try
		{	
			if( removals > fractal_size ) //This does NOT mean there are negative monomers in the chain
				//Just using size as an adjustable parameter for the number of failures seems reasonable.
			{
				!output || cout << "Too many failures -- Ending execution!"<<endl;
				return false;
			} else if( ! base->create_monomer() ) //No reason to try a bunch of times here as this is taken care of in MonteCarlo.
			{
				!output || cout << "Removing Last Monomer"<<endl;
				base->remove_last(); //Modification by me to not clear the whole fractal.
				removals++;
				goto TRY_ADD_MONOMER;
			}
			
			!output || cout << "Fractal Size: " << (int)base->size() << " of " << fractal_size << endl;
		}
		catch(...)
		{
			!output || cout << "Unknown Error in Fractal Creation." <<endl ;
			return true;
		}
	} while(base->size() < fractal_size);
	
	return true;
}

void pre_programmed_thread(int cpuNum, int cpus)
{
  const int Min_Size = 10, Max_Size = 500;
  const double minK = .9, maxK = 1.1, Kstep = 0.05;
  const double minD = 1.0, maxD = 3.0, Dstep = .01, Dtotal = (maxD - minD)/Dstep;
  const unsigned int Num_Trials = 100;

  ofstream outputStable, outputUnstable;
  Fractal *base;
  
  double k = 1.19;
  for(double D = minD; D < maxD; D+=Dstep)
  {
	for(unsigned int i = cpuNum; i < Num_Trials; i+=cpus)
	{
	  unsigned int N = (double)rand() / RAND_MAX * (Max_Size - Min_Size) + Min_Size;
	  //Start from nothing
	  base = new Fractal();
	  
	  base->set_Df(D);
	  base->set_kf(k);
	  
	  if( generate_fractal(base, N, false) ) //Create a fractal for the given parameters. 
	  {
		
		char fileStable[255];
		char fileUnstable[255];
		
		sprintf( fileStable, "%s_%g_%g_%03u_%03u.txt", "stable", k, D, N, i );
		outputStable.open(fileStable, ios::trunc);
		
		sprintf( fileUnstable, "%s_%g_%g_%03u_%03u.txt", "unstable", k, D, N, i );
		outputUnstable.open(fileUnstable, ios::trunc);
		
		base->orient_random(outputStable, 25, true); //Conduct Analysis
		
		base->orient_random(outputUnstable, 25, false); //Conduct Analysis
		
		outputStable.close();		
		outputUnstable.close();
		
		delete base;
	  }
	}
  }
}


void pre_programmed()
{
	  const int cpus = 4;
	  int* pids = new int[cpus];
  
	  //This is really rudimentary multithreading using fork.
      //Pthreads should likely be used in the future, but since the threads don't need to communicate, fork works well.
  
	  for( int i = 0; i < cpus; i++ )
	  {
		if( ( pids[i] = fork() ) == 0 ) 
		{
		  pre_programmed_thread( i, cpus );
		  exit(3); 
		}
		sleep ( 1 );  //So they have different random seeds.
	  }
		   
	  for( int i = 0; i < cpus; i++ )
	  {
		int status = 0;
		do
		{
		  waitpid(pids[i], &status, 0);
		} while( !WIFEXITED( status) );
	  }
}


void print_menu(Fractal & base)
{
	cout << "Fractal Parameters: ";
	cout << "N = " << base.size() << ",\t";
	
	cout << endl;
	cout << "Choose an option to continue." << endl;
	cout << "Fractal menu:" << endl;
	cout << "\t" << ESCAPE  << ": Exit" << endl;
	cout << "\t" << KEY_0  << ": Clear current fractal and create a new fractal" << endl;
	cout << "\t" << KEY_1  << ": Calculate Structure Factor of Current Fractal" << endl;
	cout << "\t" << KEY_2  << ": 2D Micrograph Analysis" << endl;
	cout << "\t" << KEY_3  << ": Help" << endl;
}

int get_user_input(Fractal & base)
{
	int key;
	print_menu(base);
	cin >> key;
	return key;
}

void print_input_error(const char *program_name, const char *error_msg)
{
	cerr << "Error: " << error_msg << endl;
	cerr << "Usage:" << endl;
	cerr << "\t" << program_name << " Df kf N" << endl;
	cerr << "or, using default values," << endl;
	cerr << "\t" << program_name << " N" << endl;
	cerr << endl;
	cerr << "Default Values:" << endl;
	cerr << "\tFractal Dimension (Df):\t" << FRACTAL_DIMENSION << endl;
	cerr << "\tPrefactor (kf):\t" << PREFACTOR << endl;
}

int parse_input(int argc, char **argv, Fractal & f)
{
	if(argc == 1 || argc < 4)
		return -1;
	else
	{
		cout << "Converting" << endl;
		double Df = atof(argv[1]), kf = atof(argv[2]);
		int size = atoi(argv[3]);
		if(Df >= 1.0 && Df <= 2)
		{
			f.set_Df(Df);
		}
		else
		{
			print_input_error(argv[0],"Invalid Fractal Dimension.  Range: [1.0, 2.0]");
			return -1;
		}
		
		if(kf >= 1.0)
		{
			f.set_kf(kf);
		}
		else
		{
			print_input_error(argv[0],"Invalid Prefactor.  Range: [1.0, inf)");
			return -1;
		}
		
		if(size <= 0)
		{
			print_input_error(argv[0],"Invalid Size. Range: Integer > 0");
			return -1;
		}
		else
		{
			cout << "Successfully Returning Size." << endl;
			return size;
		}
	}
}

void fractal_creation(Fractal & base, int argc, const char** argv)
{
	int fractal_size;
	int n_argc = argc;
	char **n_argv;
	
	n_argv = (char **)malloc(sizeof(char *) * 4);
	for(int i = 0; i < 4; i++)
	{
		n_argv[i] = (char *) malloc(sizeof(char) * 128);
	}
	
	for(int i = 0; i < argc; i++)
		strcpy(n_argv[i], argv[i]);
	
	base.clear();
	
	while(!((fractal_size = parse_input(n_argc, n_argv, base)) > 0))
	{
		char answer[128];
		do
		{
			cout << "Proceed with prearranged program?(yes/no)" << endl;
			cin >> answer;
			if(strncmp(answer, "yes", 3) == 0)
			{
				pre_programmed();
				exit(0);
			}
			else if(strncmp(answer, "no", 2) == 0)
			{
				cout << "Fractal Dimension?" << endl;
				cin >> n_argv[1];
				cout << "Prefactor?" << endl;
				cin >> n_argv[2];
				cout << "Fractal Size?" << endl;
				cin >> n_argv[3];
				n_argc = 4;
				break;
			}
			else
			{
				cout << "Please answer \"yes\" or \"no\"." << endl;
			}
		}while(true);
	}
		
	generate_fractal(&base, fractal_size, true);
}

int main(int argc, const char **argv)
{
	int key;		
	ifstream test;
	ofstream output;
	Fractal base;
	char filename[128];
	
	fractal_creation(base, argc, argv);
	
	do
	{
		key = get_user_input(base);	
		
		try
		{
			switch (key)
			{
				case ESCAPE:
					break;
				case KEY_0:
					fractal_creation(base, 1, argv);
				case KEY_1:
					while(!output.is_open())
					{
						cout << "Output File Name? (e.g. output.txt)" << endl;
						cin >> filename;
						test.open(filename);
						if (test.is_open())
						{
							cerr << "File already exists." << endl;
							test.close();
							continue;
						}
						else
						{
							output.open(filename);
						}
					}
					base.structurec(output);
					output.close();
					break;
				case KEY_2:
					while(!output.is_open())
					{
						cout << "Output File Name? (e.g. output.txt)" << endl;
						cin >> filename;
						test.open(filename);
						if (test.is_open())
						{
							cerr << "File already exists." << endl;
							test.close();
							continue;
						}
						else
						{
							output.open(filename);
						}
					}
					base.orient_random(output, 25, true);
					output.close();
					break;
				case KEY_3:
					cout << "Help:" << endl;
					break;
				default:
					cout << "Invalid menu option." << endl;
					break;					
			}
		}
		catch(...)
		{
			cout << "Generic Error: Probable failed creation of monomer." << endl;
		}
	} while(key != ESCAPE);
	
	return 0;
}
