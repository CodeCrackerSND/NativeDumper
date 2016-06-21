// NativeDumperDlg.h : header file
//
#include "ModulesDlg.h"
#include "DumpModuleDlg.h"

#if !defined(AFX_NATIVEDUMPERDLG_H__646483B9_F49A_4496_92F9_A35784B1060A__INCLUDED_)
#define AFX_NATIVEDUMPERDLG_H__646483B9_F49A_4496_92F9_A35784B1060A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNativeDumperDlg dialog

class CNativeDumperDlg : public CDialog
{
// Construction
public:
	CNativeDumperDlg(CWnd* pParent = NULL);	// standard constructor
	static BOOL EnableDebugPrivileges();
	
	ModulesDlg modulesdlg;
	DumpModuleDlg dmoduledlg;
// Dialog Data
	//{{AFX_DATA(CNativeDumperDlg)
	enum { IDD = IDD_NATIVEDUMPER_DIALOG };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNativeDumperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshProcessList();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNativeDumperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NATIVEDUMPERDLG_H__646483B9_F49A_4496_92F9_A35784B1060A__INCLUDED_)
