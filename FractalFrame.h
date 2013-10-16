/*
 *  FractalFrame.h
 *  wxFractals
 *
 *  Created by Shammah Chancellor on 30/10/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */

#ifndef __FRACTALFRAME__
#define __FRACTALFRAME__

#include <fstream>

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/gbsizer.h>

#include "PostTextEvent.h"
#include "ThreadedFractalCreate.h"
#include "FractalCanvas.h"

#include "MicrographFrame.h"

enum FractalFrameEvents
{
	ID_CreateFractal = 1,
	ID_SaveMicrographs,
	ID_CreateMicrograph
};

class FractalFrame :  public wxMDIChildFrame
{
public:
	FractalFrame( wxMDIParentFrame * parent );
	~FractalFrame();
	
protected:
	wxGridBagSizer *mySizer;
	wxButton *createFractal;
	wxButton *saveMicrographs;
	wxButton *createMicrograph;

	wxTextCtrl *fracDimTxt;
	wxTextCtrl *fracPrefacTxt;
	wxTextCtrl *fracMonomersTxt;
	wxTextCtrl *fracMonomerDiameterTxt;	
	
	wxGauge *progressBar;
	wxStaticText *threadOutput;
	
	ThreadedFractalCreate *thread;
	FractalCanvas *fracCanvas;
	Fractal *frac;
	
	void CreateThread(wxCommandEvent& event);
	void SaveMicrographs(wxCommandEvent& event);
	void OnMicrograph(wxCommandEvent& event);
	

	void ReceiveText(PostTextEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif