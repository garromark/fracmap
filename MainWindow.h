/*
 *  wxMainWindow.h
 *  wxFractals
 *
 *  Created by Shammah Chancellor on 30/10/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */
#ifndef __wxMAINWINDOW__
#define __wxMAINWINDOW__
#include <wx/wx.h>

#include "FractalFrame.h"
#include "MicrographFrame.h"


enum wxMDIParentFrameIDs{
	ID_New = 1,
	ID_Quit,
	ID_About,
	
	ID_NewFractal,
	ID_NewMicrograph
}; 

class wxMainWindow : public wxMDIParentFrame
	{
	public: 
		wxMainWindow( );
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		
		void OnNewFractal( wxCommandEvent& event );
		void OnNewMicrograph( wxCommandEvent& event );
		
		DECLARE_EVENT_TABLE()
	};

#endif