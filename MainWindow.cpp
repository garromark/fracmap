/*
 *  wxMainWindow.cpp
 *  wxFractals
 *
 *  Created by eluusive on 30/10/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */

#include "MainWindow.h"

BEGIN_EVENT_TABLE(wxMainWindow, wxFrame)
EVT_MENU(ID_Quit, wxMainWindow::OnQuit)
EVT_MENU(wxID_ABOUT, wxMainWindow::OnAbout) //Special cross platform About Menu item ID.
EVT_MENU(ID_NewFractal, wxMainWindow::OnNewFractal)
EVT_MENU(ID_NewMicrograph, wxMainWindow::OnNewMicrograph)

END_EVENT_TABLE()

wxMainWindow::wxMainWindow( )
: wxMDIParentFrame( (wxFrame *)NULL, wxID_ANY, _T("SPT FracMap"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE )
{
#ifdef __WXMAC__
	//wxApp::s_macAboutMenuItemID = ID_About;
#endif

	CenterOnScreen(); //Center the Window

	wxMenu *menuFile = new wxMenu;
	
	wxMenu *menuNew = new wxMenu;
	menuNew->Append( ID_NewFractal, _T("&Fractal") );
	menuNew->Append( ID_NewMicrograph, _T("&Micrograph") );
	
	menuFile->AppendSubMenu( menuNew, _T("New"));
	menuFile->Append( wxID_ABOUT, _T("&About...") );
	menuFile->AppendSeparator();
	
	menuFile->Append( ID_Quit, _T("E&xit") );
	
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, _T("&File") );
	
	SetMenuBar( menuBar );

	SetSizer( new wxBoxSizer(wxVERTICAL) );
	GetSizer()->Add( GetClientWindow(), 1, wxALL | wxEXPAND, 0);
	
	Fit();
}

void wxMainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void wxMainWindow::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
	wxMessageBox(_T("Written by Mark Garro and Shammah Chancellor"), _T("About SPT FracMap..."),  wxOK | wxICON_INFORMATION, this);
}

void wxMainWindow::OnNewFractal( wxCommandEvent& WXUNUSED(event) )
{
	FractalFrame* newFractal = new FractalFrame( this );

	newFractal->Show(TRUE);
}

void wxMainWindow::OnNewMicrograph( wxCommandEvent& WXUNUSED(event) )
{
	//Insert stuff here to open bitmaps

	MicrographFrame* newMicrograph = new MicrographFrame( this );
	
	newMicrograph->Show(TRUE);
}