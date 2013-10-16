
#ifndef __FRACTALCANVAS__
#define __FRACTALCANVAS__

#include <wx/wx.h>
#include <wx/glcanvas.h>

#ifdef __DARWIN__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#endif

#include "fractal.h"
 
class FractalCanvas: public wxGLCanvas {
public:
    FractalCanvas(wxFrame* parent);
    
	void SetFractal( Fractal * base);

  Vector_3 GetCameraVector( ); // Gets a vector from <0,0,0> in the direction of the Camera
  void ResetCamera( ); // Gets a vector from <0,0,0> in the direction of the Camera

	void Render();
	int DrawGLScene();// Here's Where We Do All The Drawing
	
	void CreateSphere(Vector_3 c, double r, int n);
	void PrepareSphere();
	
	// events
	void PaintIt(wxPaintEvent& event);
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	
	void keyPressed(wxKeyEvent& event);
	
protected:
	void GLinit();
	bool GLinitalized;

	GLfloat cameraMatrix[16];
	
	void RotateView( GLfloat dPhi, GLfloat dTheta , bool save = false);
	void ZoomView( GLfloat zoom );
	
	Fractal* _frac;
	GLuint sphereList;
	
	//Rotating
	bool dragging;
	float dragPosX, dragPosY;
	
    DECLARE_EVENT_TABLE()
};
 


#endif