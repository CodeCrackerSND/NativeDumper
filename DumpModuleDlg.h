#if !defined(AFX_DUMPMODULEDLG_H__6E6FC8CC_265F_43CC_BBC7_FD4D86EF606C__INCLUDED_)
#define AFX_DUMPMODULEDLG_H__6E6FC8CC_265F_43CC_BBC7_FD4D86EF606C__INCLUDED_

#include "NewEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DumpModuleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DumpModuleDlg dialog

class DumpModuleDlg : public CDialog
{
// Construction
public:
	DumpModuleDlg(CWnd* pParent = NULL);   // standard constructor
	DumpModuleDlg& operator=(DumpModuleDlg& right); // Overload Assignment Operator
	void DumpModule(TCHAR filename[]);
	DWORD GetMainThreadId(DWORD dwPid);
	unsigned int GetModuleSize(HMODULE hMod);
	unsigned int GetEIPRegister();
	unsigned int GetEntryPoint();
	void LastErrorDisplay();

	int processid;
	CString processname;
	TCHAR fullmodulename[MAX_PATH];
	TCHAR shortmodulename[MAX_PATH];
	HMODULE hMod;

// Dialog Data
	//{{AFX_DATA(DumpModuleDlg)
	enum { IDD = IDD_DUMPMODULEDLG_DIALOG };
	NewEdit	m_ep_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DumpModuleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DumpModuleDlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDumpBTN();
	afx_msg void OnButCurrentEip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMPMODULEDLG_H__6E6FC8CC_265F_43CC_BBC7_FD4D86EF606C__INCLUDED_)
