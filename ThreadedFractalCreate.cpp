/*
 *  ThreadedFractalCreate.cpp
 *  wxFractals
 *
 *  Created by eluusive on 07/11/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */

#include "ThreadedFractalCreate.h"

ThreadedFractalCreate::ThreadedFractalCreate( wxFrame* parent , Fractal *frac, long size ) 
: wxThread(wxTHREAD_JOINABLE)
{
	fractal_size = size;
	base = frac;
	m_parent = parent;
}

wxThread::ExitCode ThreadedFractalCreate::Entry( )
{	
	//Run fractal class and post events.	
	wxString output;
	
	int removals = 0;
	
	do
	{
	TRY_ADD_MONOMER:
		try
		{	
			if( removals > fractal_size ) //This does NOT mean there are negative monomers in the chain
											   //Just using size as an adjustable parameter for the number of failures seems reasonable.
			{
				PostText( _T("Too many failures -- Ending execution!") );
				return 0;
			} else if( ! base->create_monomer() ) //No reason to try a bunch of times here as this is taken care of in MonteCarlo.
			{
				PostText( _T("Removing Last Monomer") , REMMONOMER );
				base->remove_last(); //Modification by me to not clear the whole fractal.
				removals++;
				goto TRY_ADD_MONOMER;
			}
			
			output.Clear();
			output << _T("Fractal Size: ") << (int)base->size() << _T(" of ") << fractal_size ;
			PostText( output, ADDMONOMER);
		}
		catch(...)
		{
			PostText( _T( "Unknown Error in Fractal Creation." ) );
			return (ExitCode)-1;
		}
		
		
	} while(base->size() < fractal_size);

	PostText( _T("Fractal complete!"), FRACTAL_DONE);
	return 0;
}

void ThreadedFractalCreate::PostText( const wxString & text, long evtType )
{	
	PostTextEvent event(evtType, text);
	wxPostEvent( m_parent,  event);
}