/*
 *  ThreadedFractalCreate.h
 *  wxFractals
 *
 *  Created by Shammah Chancellor on 07/11/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */

#ifndef __THREADEDFRACTALCREATE_H__
#define __THREADEDFRACTALCREATE_H__

#include <wx/wx.h>
#include <wx/thread.h>
#include "PostTextEvent.h"
#include "fractal.h"

enum
{
	ADDMONOMER = 100,
	REMMONOMER ,
	FRACTAL_DONE,
};

class ThreadedFractalCreate : public wxThread
{
	Fractal *base;
	long fractal_size;
	
public:
	ThreadedFractalCreate( wxFrame* parent, Fractal *frac, long size );
	
	ExitCode Entry( );
	
	void PostText( const wxString& text, long EventType = wxID_ANY);
	
protected:
	wxFrame* m_parent;
};


#endif