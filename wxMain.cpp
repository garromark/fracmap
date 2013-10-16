/*
 *  wxMain.cpp
 *  wxFractals
 *
 *  Created by eluusive on 30/10/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */

#include "wxMain.h"

class MyApp: public wxApp
	{
		virtual bool OnInit();
	};

bool MyApp::OnInit()
{
	wxMainWindow *frame = new wxMainWindow(  );
	
#ifdef __WXMSW__
	frame->Show(TRUE);
#endif
	SetTopWindow(frame);
	
	return TRUE;
}


IMPLEMENT_APP(MyApp)
