/*
 *  MicrographFrame.cpp
 *  wxFracMap
 *
 *  Created by eluusive on 14/02/2009.
 *  Copyright 2009 University Of Neveda, Reno. All rights reserved.
 *
 */

#include "MicrographFrame.h"

//DEFINE_EVENT_TYPE( PostTextCommandEvent )

BEGIN_EVENT_TABLE(MicrographFrame, wxMDIChildFrame)
//EVT_BUTTON( ID_CreateFractal, FractalFrame::CreateThread)
//EVT_BUTTON( ID_SaveMicrographs, FractalFrame::SaveMicrographs)
//EVT_POSTTEXT( wxID_ANY, FractalFrame::ReceiveText )
END_EVENT_TABLE()

MicrographFrame::MicrographFrame( wxMDIParentFrame* parent ) 
: wxMDIChildFrame( parent, -1, _T("Micrograph"), wxDefaultPosition, wxSize(640,480) )
{
	m_sizer = new wxGridBagSizer(2,1);
	
	//Make the windows sizer a boxSizer, so that we can get a border.
	//Add one item, the grid sizer and use that.
	wxBoxSizer *windowSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBitmap bitmap;
	bitmap.Create( 50, 50, 1);
	m_MicrographView = new wxStaticBitmap( this, wxID_ANY, bitmap, wxDefaultPosition, wxSize( 10, 10),  wxBORDER_SIMPLE  );

	m_infoText = new wxStaticText( this, -1, _T("Boxcount:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
	m_sizer->SetFlexibleDirection(wxBOTH);
	m_sizer->AddGrowableRow(0);
	m_sizer->AddGrowableCol(0);
	
	m_sizer->Add( m_MicrographView, wxGBPosition(0,0), wxDefaultSpan, wxEXPAND );

	m_sizer->Add( m_infoText, wxGBPosition(1,0), wxDefaultSpan, wxEXPAND);

	m_micrograph = NULL;

	SetSizer(windowSizer);
	windowSizer->Add( m_sizer, 1, wxALL | wxEXPAND, 10);
	windowSizer->Fit(this);
}

void MicrographFrame::SetMicrograph( Micrograph & micrograph )
{
	m_micrograph = & micrograph;
	
	//wxImage Foo( width, width, bitarray, false );
	unsigned int width;
	
	char* bitArray = m_micrograph->getBitArray(width);
	wxBitmap myNewBitmap(bitArray, width, width, 1);
	m_MicrographView->SetBitmap( myNewBitmap );

	SetInfoText();
}

void MicrographFrame::SetInfoText()
{
	wxString output;
	double length = 0, width = 0;
	
	pair<double, double> dim = m_micrograph->Df();
	pair<double, double> perDf = m_micrograph->perdf();  //Sorrenson
  
	pair<double, double> dim_grid = m_micrograph->Df_grid();
	pair<double, double> perimDf_grid = m_micrograph->perimeterDf_grid(); //2d Perimeter

	m_micrograph->dimensions( length, width );
  
	output << m_micrograph->Nproj() << wxT("  ");            // Nproj
	output << m_micrograph->Area() << wxT("\t\t"); // Area
	
	output << length << wxT("  ");       // Lmax
	output << width << wxT("\t\t");            // Width based on length
  
	output << dim.first << wxT("  ");                // 2d Df (Box Counting)
	output << dim.second * dim.second << wxT("\t\t");  // R-squared (of above)
  
	output << dim_grid.first << wxT("  ");                // 2d Df (Box Counting)
	output << fabs(dim_grid.second) << wxT("\t\t");  // R-squared (of above)

	output << perimDf_grid.first << wxT("  ");		    // 2d Perimeter Df (Coastal)
	output << fabs(perimDf_grid.second) << wxT("\t\t");           // R-squared (of above)
  
	output << perDf.first << wxT("  ");				// 2d Perimeter Df Sorrenson
	output << fabs(perDf.second) << wxT("\t\t");             //R-squared (of above)
	
	m_infoText->SetLabel( output );
}

MicrographFrame::~MicrographFrame()
{

}