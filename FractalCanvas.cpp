#include "FractalCanvas.h"

BEGIN_EVENT_TABLE(FractalCanvas, wxGLCanvas)
	EVT_MOTION(FractalCanvas::mouseMoved)
	EVT_LEFT_DOWN(FractalCanvas::mouseDown)
	EVT_LEFT_UP(FractalCanvas::mouseReleased)
	EVT_MOUSEWHEEL(FractalCanvas::mouseWheelMoved)
    EVT_PAINT(FractalCanvas::PaintIt)
	EVT_CHAR(FractalCanvas::keyPressed)
END_EVENT_TABLE()

FractalCanvas::FractalCanvas(wxFrame *parent)
: wxGLCanvas(parent, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0, wxT("FracMapCanvas"))
{
	_frac = NULL;
	dragging = 0;
	GLinitalized = false;
}

void FractalCanvas::GLinit()
{
	GLinitalized = true;

	SetCurrent();
	wxPaintDC(this);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(0, 0, (GLint)GetSize().x, (GLint)GetSize().y);
	
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_DEPTH_TEST );	
	glEnable( GL_BLEND );
	
	ResetCamera();
	
	PrepareSphere();

	glFlush();
	SwapBuffers();
}

void FractalCanvas::SetFractal( Fractal * base)
{
	_frac = base;
}
 
void FractalCanvas::PaintIt(wxPaintEvent& WXUNUSED(event))
{
	if( !GLinitalized )
		GLinit();
	
	wxPaintDC( this );
	Render();
}

// some useful events to use
void FractalCanvas::mouseMoved(wxMouseEvent& event)
{
	if( dragging )
	{
		float dTheta = (( event.GetX() - dragPosX ) / GetSize().x) * 180;
		float dPhi = (( event.GetY() - dragPosY ) / GetSize().y) * 180;
		
		RotateView(dPhi, dTheta);
				
		Render();
	}
}

void FractalCanvas::RotateView( float dPhi, float dTheta, bool save )
{
	SetCurrent();
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( cameraMatrix );
	glRotatef( dTheta, cameraMatrix[1], cameraMatrix[5], cameraMatrix[9] );
	glRotatef( dPhi, cameraMatrix[0], cameraMatrix[4], cameraMatrix[8] );

	if(save)
		glGetFloatv(GL_MODELVIEW_MATRIX, cameraMatrix);
}

void FractalCanvas::mouseReleased(wxMouseEvent& WXUNUSED(event)) 
{
	dragging = false;

	SetCurrent();
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraMatrix);
}

void FractalCanvas::mouseDown(wxMouseEvent& event)
{
	dragging = true;
	dragPosX = event.GetX();
	dragPosY = event.GetY();
	SetFocus();
}

void FractalCanvas::mouseWheelMoved(wxMouseEvent& event)
{
	GLfloat mult =  (event.GetWheelRotation() < 0 ? .9 : 1.1);
	
	ZoomView( mult );
	
	Render();
}

void FractalCanvas::keyPressed(wxKeyEvent& event)
{
	switch ( (char)event.GetKeyCode() )
	{
		case 'j':
			RotateView( 0.0, -10.0, true );
			break;
		case 'l':
			RotateView( 0.0, 10.0, true );
			break;
		case 'i':
			RotateView( 10.0, 0.0, true);
			break;
		case 'k':
			RotateView( -10.0, 0.0, true );
			break;
		
		case '=':
		case '+':
			ZoomView( 1.1 );
			break;
			
		case '-':
		case '_':
			ZoomView(.9);
			break;

		default:
			event.Skip();
			return;
	}
	
	Render();
}

void FractalCanvas::ZoomView( GLfloat zoom )
{
	SetCurrent();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glLoadMatrixf( cameraMatrix );
	glScalef( zoom, zoom, zoom );
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraMatrix);	
}

void FractalCanvas::ResetCamera()
{
  //Setup our camera matrix that we will use to move around with the mouse
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  gluLookAt( 0, 0, -50,
			0.0, 0.0, 0.0,
			0.0, 1.0, 1000.0 ); 
  glGetFloatv(GL_MODELVIEW_MATRIX , cameraMatrix );
}

Vector_3 FractalCanvas::GetCameraVector( )
{
  Vector_3 viewVec;
  
  glGetFloatv(GL_MODELVIEW_MATRIX, cameraMatrix); 
  viewVec.x = cameraMatrix[8];
  viewVec.y = cameraMatrix[9];
  viewVec.z = cameraMatrix[10];
  
  return viewVec.normalize();
}
 
void FractalCanvas::Render()
{
	GLfloat curCamera[16];
	GLfloat lightDiffusion[] = { 1.0, 1.0, 1.0 };
	GLfloat lightPosition[] = {0.0,0.0,1.0,0.0}; //Light from infinity in the Z direction.
	
    SetCurrent( );
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT );
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// Clear Screen And Depth Buffer
	
	glMatrixMode( GL_PROJECTION );
	glViewport( 0, 0, (GLint)GetSize().x, (GLint)GetSize().y );
	glLoadIdentity( );
	gluPerspective( 80.0, (GLfloat)GetSize().x / (GLfloat)GetSize().y, 1.0, 1000.0 ); 
	
	glMatrixMode( GL_MODELVIEW );
	//This is not needed because we don't change our modelview matrix in a way that affects the camera.
	//glLoadIdentity( );
	glGetFloatv(GL_MODELVIEW_MATRIX, curCamera);
	lightPosition[0] = curCamera[1];  //Set the light vector to be that of the camera's up axis.
	lightPosition[1] = curCamera[5];
	lightPosition[2] = curCamera[9];
	
	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
	//glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffusion );
	//If the light is at infiinity, attenuation is not used by OpenGL
    //glLightf(  GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5 );	
	
	DrawGLScene(  );
	
    glFlush();
    SwapBuffers();
}

int FractalCanvas::DrawGLScene() // Here's Where We Do All The Drawing
{	
	for(int i = _frac->begin(); _frac && i < _frac->end(); i++)
	{
		Vector_3 & monomer = _frac->grab(i);
		
		//This colors and moves the call list for the sphere we made prior.
		glPushMatrix();
		glTranslatef( monomer.x, monomer.y, monomer.z );

		GLfloat r = .4, g = .4, b = .4;
		
		for( vector<Vector_3>::const_iterator itr = _frac->_facets.begin();
			itr != _frac->_facets.end(); itr++)
		{
			if( *itr == monomer )
			{r = 1.0; g = 1; b = 1;}
		}
		
		glColor3f( r,g,b );
		//glScalef is not really necessary as radius is a fixed value of 1 currently.
		glScalef( _frac->radius(), _frac->radius(), _frac->radius() );
		glCallList( sphereList );
		glPopMatrix();
	}
	
	
	if( _frac && _frac->_facets.size() >= 3 )
	{
		glBegin(GL_TRIANGLES);
		
		double n = _frac->_facets.size();
		int i = 1;
		for( vector<Vector_3>::const_iterator itr = _frac->_facets.begin();
			itr != _frac->_facets.end();
			itr++,i++)
		{
			if( i % 3 == 0)
			{
			  double color = (double)i/n;
			  if( i + 1 == n )
				glColor3f(0 , 0 , 0 );
			  else
				glColor3f(color,  color, color);
			}
			glVertex3f( itr->x, itr->y, itr->z );
		}
		glEnd();
	}
	
	return TRUE;
}

void FractalCanvas::PrepareSphere( )
{
	//Make a sphere display list.
	GLUquadric *sphere = gluNewQuadric();
	sphereList = 10;
	
	glNewList( sphereList, GL_COMPILE );
	gluQuadricDrawStyle( sphere, GLU_FILL );
	gluQuadricNormals( sphere, GLU_SMOOTH );
	gluSphere( sphere, 1, 100, 100 );
	glEndList();
	
	gluDeleteQuadric( sphere );
}
