/*
 *  MicrographFrame.h
 *  wxFracMap
 *
 *  Created by Shammah Chancellor on 14/02/2009.
 *  Copyright 2009 University Of Neveda, Reno. All rights reserved.
 *
 */

#ifndef __MICROGRAPHFRAME__
#define __MICROGRAPHFRAME__

#include <fstream>

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/gbsizer.h>

#include "PostTextEvent.h"
#include "ThreadedFractalCreate.h"
#include "FractalCanvas.h"

#include "micrograph.h"

enum MicrographFrameEvents
{
	ID_TEMPORARY = 1
};

class MicrographFrame :  public wxMDIChildFrame
	{
	public:
		MicrographFrame( wxMDIParentFrame * parent );
		~MicrographFrame();
		
		void SetMicrograph( Micrograph & micrograph );
		
	protected:
		void SetInfoText();
		
		wxGridBagSizer *m_sizer;
		wxStaticBitmap *m_MicrographView;
		wxStaticText *m_infoText;
		
		Micrograph *m_micrograph;
		
		DECLARE_EVENT_TABLE()
	};

#endif