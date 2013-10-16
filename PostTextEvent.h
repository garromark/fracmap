/*
 *  PostTextEvent.h
 *  wxFractals
 *
 *  Created by Shammah Chancellor on 07/11/2008.
 *  Copyright 2008 University Of Neveda, Reno. All rights reserved.
 *
 */
#ifndef __POSTTEXTEVENT__
#define __POSTTEXTEVENT__

#include <wx/wx.h>

DECLARE_EVENT_TYPE( PostTextCommandEvent, -1 )

class PostTextEvent : public wxCommandEvent
	{
	protected:
		wxString m_text;
		
	public:
		PostTextEvent( int id = 0, const wxString & text = _T("") ) 
		: wxCommandEvent(PostTextCommandEvent, id) { SetText(text); }
		
		PostTextEvent( const PostTextEvent & event )
		:  wxCommandEvent(event) { this->SetText( event.GetText() ); }
		
		// Required for sending with wxPostEvent()
		wxEvent* Clone() const { return new PostTextEvent(*this); }
		
		wxString GetText() const { return m_text; }
		void SetText( const wxString &text ) { m_text = text; }
	};


typedef void (wxEvtHandler::*PostTextEventFunction)(PostTextEvent &);

// This #define simplifies the one below, and makes the syntax less
// ugly if you want to use Connect() instead of an event table.
#define PostTextEventHandler(func)                                         \
(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)\
wxStaticCastEvent(PostTextEventFunction, &func)                    

// Define the event table entry. Yes, it really *does* end in a comma.
#define EVT_POSTTEXT(id, fn)                                            \
DECLARE_EVENT_TABLE_ENTRY( PostTextCommandEvent, id, wxID_ANY,  \
(wxObjectEventFunction)(wxEventFunction)                     \
(wxCommandEventFunction) wxStaticCastEvent(                  \
PostTextEventFunction, &fn ), (wxObject*) NULL ),

#endif