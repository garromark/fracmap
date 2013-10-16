

		Fractal Creation, Orientation, and Analysis Package
		---------------------------------------------------


COMPILING:

	WINDOWS INSTALLATION:

	Microsoft Visual Studio (MSV) 2005 workspace and project files have been included in
	the source code distribution.

	To build executable:
	First, open the Microsoft Visual Studio Solution file Unix_Fractal.sln from within MSV.
	Second, select Build->Build Unix_Fractal_final.
	The build process will copy:
	Unix_Fractal.exe to the directory debug

	
	LINUX INSTALLATION:

	Included with the source code should be the makefile "Makefile".
	Change directory to Unix_Fractal where the makefile and base code should be found.
	Execute on the command line:
	% make -f Makefile
	The build should create the executable file "fractal" in the same directory.


EXECUTION:

	USAGE:
	
	METHOD1 -

	From the command line, execution without parameters (accomplished by double-clicking
	the executable in Windows):

		% $(EXE)

	This method will prompt the user for expected parameters for creation from within the
	program.  The program will proceed as follows:

		Fractal Dimension?
		% 'fractal_dimension
		Prefactor?
		% 'prefactor
		Fractal Size?
		% 'size

	METHOD2 -

	Command line execution given specified parameters:

		% $(EXE) 'fractal_dimension 'prefactor 'size
	

	With both methods, a fractal will be generated with Df equal to "fractal_dimension",
	kf equal to "prefactor", and of size equal to "size".

	Following successful fractal generation, the program will request user input for any
	proceeding analysis to be done.

		Fractal Paramters: N = 'size
		Choose an option to continue.
		Fractal menu:
			0: Exit
			1: Clear current fractal and create a new fractal
			2: Calculate Structure Fractor of Current Fractal
			3: 2D Micrograph Analysis
			4: Help
		% 'option

	If option 0 is selected, the program will exit.  If option 1 is selected, the program
	will clear its current fractal information and will proceed to prompt the user for new
	parameters in order to construct a new fractal.  If option 2 is selected, the user will
	be prompted for the target filename for an output file where the lognormal values of
	Qval and Angsum will be placed in tab delimited columns so that further analysis can be
	done using spreadsheet management software such as Microsoft Office's Excel.  The user
	must take a linear regression, but must make a decision for preliminary cutoff of output
	points such that the regression is, for the most part, a linear one.  If option 3 is
	selected, the user will be prompted for the target filename for an output file that will
	be the results of a comprohensive analyis of 25 projected, pixelated, 2-D images of
	unique stable orientations of the given 3-D fractal.  The output for the file will have
	output the following parameters per trial:

		N-projected - Apparent number of monomers in the 2-D image
		Area of the 2-D image
		N - Actual number of monomers in the 2-D image
		Lmax - Maximum length (in units, monomer radius is one unit) of the 2-D image
		width - Maximum perpendicular width to the given Lmax
		2-D Fractal Dimension (Box Counting Method)
		R^2 value for the linear regression to find the above value
		2-D Hybrid Perimeter Fractal Dimension
		R^2 value for the linear regression to find the above value
		Sorrenson's 2-D Perimeter Fractal Dimension Method
		R^2 value for the linear regression to find the above value
		3-D Fractal Dimension
		2-D Radius of Gyration
		3-D Radius of Gyration