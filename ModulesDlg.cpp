// ModulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NativeDumper.h"
#include "ModulesDlg.h"
#include "GenericPurposeMethods.h"

#include "C:\Program Files (x86)\Microsoft SDK\include\Psapi.h"
#pragma comment (lib, "Psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModulesDlg dialog


ModulesDlg::ModulesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ModulesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ModulesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

ModulesDlg& ModulesDlg::operator=(ModulesDlg& right)
{
// right contains value to be set
// this contains old value
(*this).m_hWnd = right.m_hWnd;
return *this;

}

void ModulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModulesDlg)
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ModulesDlg, CDialog)
	//{{AFX_MSG_MAP(ModulesDlg)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModulesDlg message handlers

void ModulesDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	this->DestroyWindow();  // destroy the window
	CDialog::OnClose();
}
TCHAR fullmodulenames[1024][MAX_PATH];
TCHAR shortmodulenames[1024][MAX_PATH];
HMODULE modules_address[1024];
int modulecount;

void ModulesDlg::RefreshModulesList()
{
	m_cListCtrl.DeleteAllItems();  // clean old items!
	
	modulecount = 0;
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processid);

	if (hProcess==NULL) return;

	HMODULE hMods[1024];
	DWORD cbNeeded;


	// Get a list of all the modules in this process.
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {

		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            TCHAR szModName[MAX_PATH];
            // Get the full path to the module's file.
            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                      sizeof(szModName) / sizeof(TCHAR)))
            {

				for (int j=0;j<(sizeof(szModName) / sizeof(TCHAR));j++)
				fullmodulenames[modulecount][j] = szModName[j];

				modules_address[modulecount] = hMods[i];

				TCHAR* psname = GenericPurposeMethods::GetShortModuleName(fullmodulenames[modulecount]);
				for (int k=0;k<(sizeof(szModName) / sizeof(TCHAR));k++)
				shortmodulenames[modulecount][k] = psname[k];

				modulecount++;

            }
		}

	}

	CloseHandle(hProcess);

	
	// Add the process name and identifier.
	for (int i=0;i<modulecount;i++)
	{
	LVITEM lvi;
	CString strItem;

	// Insert the first item
	lvi.mask =  LVIF_TEXT;
	lvi.iItem = m_cListCtrl.GetItemCount(); // this starts with 0!
	
	// insert subitem 0
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(shortmodulenames[i]);
	m_cListCtrl.InsertItem(&lvi);

	char baseaddress[20];
	wsprintf(baseaddress,"%X",hMods[i]);  // convert number to hex string
	
	int len = 8-strlen(baseaddress); // get the missing part size
	memmove(baseaddress+len,baseaddress,strlen(baseaddress)); // move the string characters to the end
	for ( int i = 0; i < len; i++ ) // fill the beginning characters with '0'
    baseaddress[i] = '0';

	baseaddress[8] = 0;  // place the 00 end char at the end of string!

	// insert subitem 1
	lvi.iSubItem = 1;
	lvi.pszText = (char*)LPCTSTR(baseaddress);
	m_cListCtrl.SetItem(&lvi);

	}

}


BOOL ModulesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwStyle = m_cListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;

	// Setup the list control
	m_cListCtrl.SetExtendedStyle(dwStyle);

	// Create the columns
	CRect rect;
	m_cListCtrl.GetClientRect(&rect);
	int size = rect.Width()/3-16;
	m_cListCtrl.InsertColumn(0, _T("Module Name"), LVCFMT_LEFT, size*2);
	m_cListCtrl.InsertColumn(1, _T("Base Address"), LVCFMT_LEFT, size);

	RefreshModulesList();

	char window_title[500];
	this->GetWindowText(window_title,sizeof(window_title));
	strcat(window_title, processname);
	this->SetWindowText(window_title);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ModulesDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
CMenu menu;
menu.LoadMenu(IDR_MODULEMENU); // our context menu
CMenu* pPopup = menu.GetSubMenu(0);

RECT rect;
GetWindowRect(&rect);
CPoint mousepos;
GetCursorPos(&mousepos);
pPopup->TrackPopupMenu(NULL,mousepos.x,mousepos.y, this); 

// The menu is a temporary MFC object, no need to delete it.
	*pResult = 0;
}

BOOL ModulesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (HIWORD(wParam) == BN_CLICKED)  // if button clicked
    {
		switch(LOWORD(wParam))  // Retrieves the low-order word from the specified value.
        {
        case ID_MODULE_DUMP:
		{
		if (!IsWindow(dumpmoduledlg.m_hWnd)||!dumpmoduledlg.IsWindowVisible())
		{
		
		DumpModuleDlg m_pdmoduledialog = new DumpModuleDlg(this);
		dumpmoduledlg = m_pdmoduledialog;
		POSITION pos = m_cListCtrl.GetFirstSelectedItemPosition();
		int position = m_cListCtrl.GetNextSelectedItem(pos);
		
		dumpmoduledlg.processname = processname;
		dumpmoduledlg.processid = processid;

		for (int i=0;i<MAX_PATH;i++)  // copy full/short module name:
		{
		dumpmoduledlg.fullmodulename[i] = fullmodulenames[position][i];
		dumpmoduledlg.shortmodulename[i] = shortmodulenames[position][i];
		}

		dumpmoduledlg.hMod = modules_address[position];

		//testdlg.DoModal();  // not modal one!
		BOOL ret = dumpmoduledlg.Create(IDD_DUMPMODULEDLG_DIALOG,this);
		if (ret)   // If create not failed.
		dumpmoduledlg.ShowWindow(SW_SHOWNORMAL);
		}
		
        break;
		}

		case ID_MODULE_REFRESH:
		RefreshModulesList();
        break;

        }
		
    }

	return CDialog::OnCommand(wParam, lParam);
}
