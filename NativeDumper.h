// NativeDumper.h : main header file for the NATIVEDUMPER application
//

#if !defined(AFX_NATIVEDUMPER_H__582C232C_849A_4C7F_A620_BCB9FD8F36AE__INCLUDED_)
#define AFX_NATIVEDUMPER_H__582C232C_849A_4C7F_A620_BCB9FD8F36AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNativeDumperApp:
// See NativeDumper.cpp for the implementation of this class
//

class CNativeDumperApp : public CWinApp
{
public:
	CNativeDumperApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNativeDumperApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNativeDumperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NATIVEDUMPER_H__582C232C_849A_4C7F_A620_BCB9FD8F36AE__INCLUDED_)
