/*
 *  FractalFrame.cpp
 *  wxFractals
 *
 *  Created by Shammah Chancellor on 30/10/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */
#include "FractalFrame.h"

#include <iostream>

DEFINE_EVENT_TYPE( PostTextCommandEvent )
BEGIN_EVENT_TABLE(FractalFrame, wxMDIChildFrame)
EVT_BUTTON( ID_CreateFractal, FractalFrame::CreateThread)
EVT_BUTTON( ID_SaveMicrographs, FractalFrame::SaveMicrographs)
EVT_BUTTON( ID_CreateMicrograph,  FractalFrame::OnMicrograph)
EVT_POSTTEXT( wxID_ANY, FractalFrame::ReceiveText )
END_EVENT_TABLE()

FractalFrame::FractalFrame( wxMDIParentFrame* parent ) 
	: wxMDIChildFrame( parent, -1, _T("New Pseudo-Fractal"), wxDefaultPosition, wxSize(640,480) )
{

	thread = NULL; //Initialize the Thread variable to NULL
	frac = NULL; //Initialize the frac variable
	
	mySizer = new wxGridBagSizer(10,10);
	
	//Make the windows sizer a boxSizer, so that we can get a border.
	//Add one item, the grid sizer and use that.
	wxBoxSizer *windowSizer = new wxBoxSizer( wxHORIZONTAL );
	windowSizer->Add( mySizer, 1, wxALL | wxEXPAND, 10);
	SetSizer(windowSizer);

	//Create the window controls
	fracDimTxt = new wxTextCtrl( this, -1, _T("1.82"), wxDefaultPosition, wxSize(50,-1), 0, wxTextValidator(wxFILTER_NUMERIC) );
	fracPrefacTxt = new wxTextCtrl( this, -1, _T("1.19"), wxDefaultPosition, wxSize(50,-1), 0, wxTextValidator(wxFILTER_NUMERIC) );
	fracMonomersTxt = new wxTextCtrl( this, -1, _T("100"), wxDefaultPosition, wxSize(50,-1), 0, wxTextValidator(wxFILTER_NUMERIC) );
	threadOutput = new wxStaticText( this, -1, _T(""), wxDefaultPosition, wxDefaultSize);
	progressBar = new wxGauge( this, -1, 0, wxDefaultPosition, wxSize(100,-1), wxGA_HORIZONTAL );
	createFractal = new wxButton( this, ID_CreateFractal, _T("Create Fractal"));
	saveMicrographs = new wxButton( this, ID_SaveMicrographs, _T("Ensemble Analysis"));
	createMicrograph = new wxButton( this, ID_CreateMicrograph, _T("Create Micrograph"));

	createMicrograph->Disable();
	saveMicrographs->Disable();
	fracCanvas =  new FractalCanvas(this);
	
	//Position controls on my gridBagSizer in their proper columns and rows.
	mySizer->SetFlexibleDirection(wxBOTH);
	mySizer->AddGrowableCol(2,1);
	mySizer->AddGrowableRow(5,1);

	mySizer->Add( createFractal, wxGBPosition(3,0), wxGBSpan(1,2), wxEXPAND );
	mySizer->Add( saveMicrographs, wxGBPosition(4,0), wxGBSpan(1, 2), wxEXPAND );
	mySizer->Add( createMicrograph, wxGBPosition(5,0), wxGBSpan(1,2), wxEXPAND );
	
	mySizer->Add( fracCanvas, wxGBPosition(0,2), wxGBSpan(6,1), wxEXPAND );
	mySizer->Add( threadOutput, wxGBPosition(6,2), wxDefaultSpan, wxEXPAND );
	mySizer->Add( progressBar, wxGBPosition(7,2), wxDefaultSpan, wxEXPAND );
	
	
	mySizer->Add( new wxStaticText( this, -1, _T("Dimension:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT),
					wxGBPosition(0,0), wxDefaultSpan, wxEXPAND);
	
	mySizer->Add( fracDimTxt, wxGBPosition(0,1) );
	
	mySizer->Add( new wxStaticText( this, -1, _T("Prefactor:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT),
					wxGBPosition(1,0), wxDefaultSpan, wxEXPAND);
	
	mySizer->Add( fracPrefacTxt, wxGBPosition(1,1) );
	
	mySizer->Add( new wxStaticText( this, -1, _T("Monomers:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT),
				 wxGBPosition(2,0), wxDefaultSpan, wxEXPAND);
	
	mySizer->Add( fracMonomersTxt, wxGBPosition(2,1) );

	windowSizer->Fit(this);
}

FractalFrame::~FractalFrame( )
{
	if(thread != NULL)
	{
		thread->Kill();
		delete thread;
	}
	
	delete fracDimTxt;
	delete fracPrefacTxt;
	delete createFractal;
	delete threadOutput;
	delete fracCanvas;
	delete saveMicrographs;
	delete createMicrograph;
}

void FractalFrame::CreateThread( wxCommandEvent & WXUNUSED(event) )
{
	frac = new Fractal();
	wxString output;
	
	threadOutput->SetLabel( _T("Converting\n") );
	double Df, kf;
	long size;
	
	fracDimTxt->GetLineText(0).ToDouble(&Df);
	fracPrefacTxt->GetLineText(0).ToDouble(&kf);
	fracMonomersTxt->GetLineText(0).ToLong(&size);
	
	if(Df >= 1.0 && Df <= 3)
	{
		frac->set_Df(Df);
	}
	else
	{
		output << _T("Invalid Fractal DImension. Range: [1.0,3.0]: ") << Df;
		threadOutput->SetLabel( output );
		return;
	}
	
	if(kf >= 1.0)
	{
		frac->set_kf(kf);
	}
	else
	{
		output << _T("Invalid Prefactor.  Range: [1.0, inf): ") << kf;
		threadOutput->SetLabel( output );
		return;
	}
	
	if(size <= 0)
	{
		(output) << _T("Invalid Size. Range: Integer > 0: ") << size << _T("\n") ;
		threadOutput->SetLabel( output );

		return;
	}
		
	progressBar->SetRange(size);
	thread = new ThreadedFractalCreate(this, frac, size);
	fracCanvas->SetFractal( frac );

	thread->Create();
	thread->Run();
	
	createFractal->Disable();
}

void FractalFrame::SaveMicrographs( wxCommandEvent & WXUNUSED(event) )
{ 
	wxFileDialog *fileSelector = 
		new wxFileDialog(this, _T("Select the file to save to:"), _T(""),_T(""), 
						 _T("Tab Separated File (*.tsv)|*.tsv"), wxFD_SAVE );
  
  //TODO: Thread this.
  if(fileSelector->ShowModal() == wxID_OK)
	{
	threadOutput->SetLabel( _T("Saving please wait...\n") );
	progressBar->Pulse();

	wxString path = fileSelector->GetPath();
	
	ofstream outFile;
	outFile.open( path.mb_str(wxConvUTF8), ios_base::out | ios_base::trunc );
	
	frac->orient_random(outFile, 50);
	outFile.flush();
	outFile.close();

	threadOutput->SetLabel( _T("Done saving...\n") );
	progressBar->Pulse();
  
	fracCanvas->Render();
	}
}

void FractalFrame::ReceiveText( PostTextEvent & event )
{
	switch( event.GetId() )
	{
		case ADDMONOMER:
			progressBar->SetValue( progressBar->GetValue() + 1 );
			break;
		case REMMONOMER:
			progressBar->SetValue( progressBar->GetValue() - 1 );
			break;
		case FRACTAL_DONE:
			thread->Wait();
			delete thread;
			thread = NULL;
			saveMicrographs->Enable();
			createMicrograph->Enable();
			break;
		default:
			break;
	}
	threadOutput->SetLabel(event.GetText());
	
	fracCanvas->Render();
}


void FractalFrame::OnMicrograph( wxCommandEvent & WXUNUSED( event ) )
{
  Vector_3 seed = fracCanvas->GetCameraVector();
  Vector_3 newOrientation;
  
  if( frac->find_stable_vector( seed, newOrientation ) )
  {
	  frac->rotate_newZ(newOrientation);

	  MicrographFrame *micrographWindow = new MicrographFrame((wxMDIParentFrame*)(this->GetParent()));


	  Micrograph *newMicro = new Micrograph( *frac );
	  
	  micrographWindow->SetMicrograph( *newMicro );

	  micrographWindow->Show(TRUE);

	  fracCanvas->ResetCamera();
	  fracCanvas->Render();

  }
}