
// Suriken.h : main header file for the Suriken application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSurikenApp:
// See Suriken.cpp for the implementation of this class
//

class CSurikenApp : public CWinAppEx
{
public:
	CSurikenApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSurikenApp theApp;
