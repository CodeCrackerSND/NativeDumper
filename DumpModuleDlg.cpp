// DumpModuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NativeDumper.h"
#include "DumpModuleDlg.h"
#include "GenericPurposeMethods.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DumpModuleDlg dialog


DumpModuleDlg::DumpModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DumpModuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DumpModuleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DumpModuleDlg& DumpModuleDlg::operator=(DumpModuleDlg& right)
{
// right contains value to be set
// this contains old value
(*this).m_hWnd = right.m_hWnd;
return *this;

}

void DumpModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DumpModuleDlg)
	DDX_Control(pDX, IDC_NEWEntryPoint, m_ep_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DumpModuleDlg, CDialog)
	//{{AFX_MSG_MAP(DumpModuleDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DumpBTN, OnDumpBTN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DumpModuleDlg message handlers


void DumpModuleDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	this->DestroyWindow();  // destroy the window
	CDialog::OnClose();
}

BOOL DumpModuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	CButton *m_ctlCheck = (CButton*) GetDlgItem(IDC_FixSizeOfImage);
	m_ctlCheck->SetCheck(BST_CHECKED);// check FixSizeOfImage

	m_ctlCheck = (CButton*) GetDlgItem(IDC_CHANGEEP);
	m_ctlCheck->SetCheck(BST_CHECKED);// check Change EP



	char window_title[500];
	window_title[0] = 0;
	//this->GetWindowText(window_title,sizeof(window_title));
	strcat(window_title, "Dump module ");
	strcat(window_title, shortmodulename);
	//strcat(window_title, " from process ");
	//strcat(window_title, processname);
	this->SetWindowText(window_title);

	unsigned int entrypoint = DumpModuleDlg::GetEntryPoint();
	char epaddress[20];
	wsprintf(epaddress,"%X",entrypoint);  // convert number to hex string
	
	int len = 8-strlen(epaddress); // get the missing part size
	memmove(epaddress+len,epaddress,strlen(epaddress)); // move the string characters to the end
	for ( int i = 0; i < len; i++ ) // fill the beginning characters with '0'
    epaddress[i] = '0';

	epaddress[8] = 0;  // place the 00 end char at the end of string!

	GetDlgItem(IDC_NEWEntryPoint)->SetWindowText(epaddress);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

unsigned int DumpModuleDlg::GetEntryPoint()
{
HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                               FALSE, processid);
if (hProcess==NULL) return 0;

unsigned char* tempbuf= new unsigned char[04];
unsigned long dwTotalRead;

unsigned int base_address = (unsigned int)hMod+0x03C;
int isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return 0;  // if read failed return 0

int PEOffset = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
if (PEOffset==0) return 0;

base_address = (unsigned int)hMod+PEOffset+0x28;
isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return 0;  // if read failed return 0

unsigned int EntryPointAddress = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);

CloseHandle(hProcess);

return EntryPointAddress;

}

void DumpModuleDlg::OnDumpBTN() 
{
	// TODO: Add your control notification handler code here
	TCHAR* extension = GenericPurposeMethods::GetExtension(shortmodulename);
	// Final string = "All Files (*.*)\0*.*\0\0";
	TCHAR* first_part = GenericPurposeMethods::JoinChars("PE Files (*",extension);
	first_part = GenericPurposeMethods::JoinChars(first_part,")");
	int fp_len = _tcslen(first_part);
	TCHAR* second_part = GenericPurposeMethods::JoinChars("*",extension);
	int sp_len = _tcslen(second_part);
	TCHAR* lpszFilter = new TCHAR[MAX_PATH];
	for (int i=0;i<fp_len;i++)
	lpszFilter[i]=first_part[i];  // copy first part

	lpszFilter[fp_len]=0;  // mark end of string

	for (int j=0;j<sp_len;j++)
	lpszFilter[fp_len+1+j]=second_part[j];  // copy first part

	lpszFilter[fp_len+sp_len+1]=0;  // mark end of string
	lpszFilter[fp_len+sp_len+2]=0;  // mark end of string

	CFileDialog dlg(FALSE, NULL, GenericPurposeMethods::GetDumpFileName(shortmodulename),
	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, this);
	// FALSE = save file dialog
	// CFileDialog( BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL,
	// DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL );

	dlg.m_ofn.lpstrInitialDir = GenericPurposeMethods::GetDirectory(fullmodulename);
    dlg.m_ofn.lpstrTitle="Save module!";

    if (dlg.DoModal() == IDOK)
    {
    CString filename = dlg.GetPathName(); // return full path and filename
	TCHAR* cfilename = new TCHAR[filename.GetLength()+1];
	lstrcpy(cfilename, filename);
	if (!GenericPurposeMethods::ContainsExtension(cfilename))
	{
	cfilename = GenericPurposeMethods::JoinChars(cfilename,extension);
	}
	DumpModule(cfilename);

    }
	

}

// main code:
void DumpModuleDlg::DumpModule(TCHAR* filename) 
{
GetDlgItem(IDC_STATUS_ST)->SetWindowText("");

HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                               FALSE, processid);
if (hProcess==NULL) return;

CButton *m_ctlCheck = (CButton*) GetDlgItem(IDC_FixSizeOfImage);
bool FixSizeOfImage = false;
int ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
FixSizeOfImage = true;

m_ctlCheck = (CButton*) GetDlgItem(IDC_CHANGEEP);
bool ChangeEP = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
ChangeEP = true;

m_ctlCheck = (CButton*) GetDlgItem(IDC_FIXRAW);
bool FixRaw = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
FixRaw = true;


unsigned int speed = 0x1000;
		
try
{
SYSTEM_INFO* pSI = new SYSTEM_INFO();
GetSystemInfo(pSI);
speed = pSI->dwPageSize;
}
catch(...)  // catch any exception
{
}

unsigned char* tempbuf= new unsigned char[04];
unsigned long dwTotalRead;

unsigned int base_address = (unsigned int)hMod+0x03C;
int isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return;  // if read failed return

int PEOffset = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
if (PEOffset==0) return;

try
{
base_address = (unsigned int)hMod+PEOffset+0x0F8+20;
isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return;  // if read failed return

unsigned int RawOfFirstSection = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
unsigned int offset=0;

unsigned int sizetocopy = RawOfFirstSection;
if (sizetocopy>speed) sizetocopy=(unsigned int)speed;
if (sizetocopy==0) sizetocopy = speed;

offset=offset+RawOfFirstSection;

unsigned char* PeHeader = new byte[sizetocopy];
isok = ReadProcessMemory(hProcess, (LPVOID)hMod, PeHeader, sizetocopy, &dwTotalRead);
if (isok == 0) return;  // if read failed return

unsigned int nrofsection = GenericPurposeMethods::UnsignedArrayToShort(PeHeader, PEOffset+0x06);
unsigned int sectionalignment = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x038);
unsigned int filealignment = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x03C);	    
unsigned int sizeofimage = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x050);
unsigned int calculatedimagesize = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x0F8+0x0C);
// calculatedimagesize = Virtual Address of first section first time!

for (unsigned int i = 0; i < nrofsection; i++)
{
int virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x0F8+0x28*i+8);
int toadd = (virtualsize%sectionalignment);
if (toadd!=0) toadd = sectionalignment-toadd;
calculatedimagesize = calculatedimagesize+virtualsize+toadd;
}

if (calculatedimagesize!=sizeofimage) sizeofimage=calculatedimagesize;
unsigned char* Dump = new unsigned char[sizeofimage];
unsigned char* Partkeep = new byte[sizeofimage];

for (unsigned int j=0;j<sizetocopy;j++)
Dump[j] = PeHeader[j];  // copy PE Header on dump

unsigned int address=0;
bool ShouldFixrawsize=false;


			int rawsize,rawaddress,virtualsize,virtualAddress;
			int calcrawsize=0;

			if (FixRaw)
			{
            for (unsigned int l = 0; l < nrofsection; l++)
            {
            rawsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*l+0x010);
            virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*l+8);
            virtualAddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*l+0xC);
            
            // RawSize = Virtual Size rounded on FileAlligment - only for .NET
            calcrawsize=0;
            calcrawsize = virtualsize%filealignment;
            if (calcrawsize!=0) calcrawsize = filealignment-calcrawsize;
            calcrawsize = virtualsize+calcrawsize;

            if (calcrawsize!=0&&rawsize!=calcrawsize&&rawsize!=virtualsize)
            {
            ShouldFixrawsize=true;
            break;
            }
            }
            

			}  // if FixRaw end

            for (i = 0; i < nrofsection; i++)
            {
            
            rawsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0x010);
			rawaddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0x014);
            virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+8);
            // RawSize = Virtual Size rounded on FileAlligment
            virtualAddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0xC);
            

            if (ShouldFixrawsize)
            {
            rawsize = virtualsize; // rawsize SHOULD BE FIXED!!!
			unsigned int position = PEOffset+0x0F8+0x28*i+0x010;
			unsigned char* vsize_array = GenericPurposeMethods::UIntToUnsignedArray(virtualsize);
			for (int j=0;j<04;j++)
			Dump[position+j] = vsize_array[j];  // fix raw size

			position = PEOffset+0x0F8+0x28*i+0x014;
			unsigned char* vaddress_array = GenericPurposeMethods::UIntToUnsignedArray(virtualAddress);
			for (j=0;j<04;j++)
			Dump[position+j] = vaddress_array[j]; // fix raw address

            //BinaryWriter writer = new BinaryWriter(new MemoryStream(Dump));
            //writer.BaseStream.Position=PEOffset+0x0F8+0x28*i+16;
            //writer.Write(virtualsize);
            //writer.BaseStream.Position=PEOffset+0x0F8+0x28*i+20;
            //writer.Write(virtualAddress);
            //writer.Close();

            }
            
    
                        
            address = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0xC);

			isok = ReadProcessMemory(hProcess, (LPVOID)((unsigned int)hMod+address), Partkeep, rawsize, &dwTotalRead);
			//ReadProcessMemory(hProcess,(uint)(ImageBase+address),Partkeep,(uint)rawsize, ref BytesRead);
            if (!isok)
            {
            unsigned char* onepage = new unsigned char[512];
            for (int c = 0; c < virtualsize; c=c+512)
            {
			isok = ReadProcessMemory(hProcess,(LPVOID)((unsigned int)hMod+virtualAddress+c),onepage,512, &dwTotalRead);
			for (int i=0;i<512;i++)
			Partkeep[c+i] = onepage[i];
			// Array.Copy(onepage, 0, Partkeep, c, 512);
            }
            }
         
            
            if (ShouldFixrawsize)
            {
            //Array.Copy(Partkeep, 0, Dump, virtualAddress, rawsize);
			for (int k=0;k<rawsize;k++)
			Dump[virtualAddress+k] = Partkeep[k];
            //offset = virtualAddress+rawsize;
            }
            else
            {
            //Array.Copy(Partkeep, 0, Dump, offset, rawsize);
			for (int z=0;z<rawsize;z++)
			Dump[rawaddress+z] = Partkeep[z];
            //offset = offset+rawsize;
            }
            }

			if (ShouldFixrawsize)
            {
			//unsigned int virtualDumpSize = 0;
			for (i = nrofsection-1; i >= 0; --i)
            {
			virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+8);
            virtualAddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0xC);
			if (virtualAddress!=0)
			{
			offset = virtualsize+virtualAddress;
			break;
			}
			}

			}
			else
			{
			//unsigned int rawDumpSize = 0;
			for (i = nrofsection-1; i >= 0; --i)
            {
            
            rawsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0x010);
			rawaddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+0x0F8+0x28*i+0x014);

			if (rawaddress!=0)
			{
			offset = rawsize+rawaddress;
			break;
			}
			}

			}

			if (FixSizeOfImage)
			{
			unsigned int position = PEOffset+0x050;
			unsigned char* isize_array = GenericPurposeMethods::UIntToUnsignedArray(calculatedimagesize);
			for (int j=0;j<04;j++)
			Dump[position+j] = isize_array[j];  // fix image size
			}

			if (ChangeEP)
			{
			unsigned int position = PEOffset+0x28;
			char addressconv[10];
			GetDlgItem(IDC_NEWEntryPoint)->GetWindowText(addressconv, sizeof(addressconv));
			unsigned int conv_address = GenericPurposeMethods::ConvertHexStringToInt(addressconv);
			unsigned char* ep_array = GenericPurposeMethods::UIntToUnsignedArray(conv_address);
			for (int j=0;j<04;j++)
			Dump[position+j] = ep_array[j];  // fix entry point
			}

			if (Dump!=NULL&&sizeofimage>0&&sizeofimage>=offset)
            {

            FILE * pFile = fopen ( filename , "w+b" );
            fwrite (Dump , 1 , offset , pFile );  // 
            fclose (pFile);

			GetDlgItem(IDC_STATUS_ST)->SetWindowText("Module dumped!");
            }




}
catch(...)  // catch any exception
{
}






CloseHandle(hProcess);

}
