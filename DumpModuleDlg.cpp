// DumpModuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NativeDumper.h"
#include "DumpModuleDlg.h"
#include "GenericPurposeMethods.h"
#include "windows.h"
#include "winbase.h"
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <winerror.h>

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
	ON_BN_CLICKED(IDC_BUT_CURRENT_EIP, OnButCurrentEip)
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

	m_ctlCheck = (CButton*) GetDlgItem(IDC_SECTIONS_FROM_MEMORY);
	m_ctlCheck->SetCheck(BST_CHECKED);// check section from memeory radio

	m_ctlCheck = (CButton*) GetDlgItem(IDC_ORIGINAL_RAW);
	m_ctlCheck->SetCheck(BST_CHECKED);// check original raw

	m_ctlCheck = (CButton*) GetDlgItem(IDC_FIX_PACKERS);
	m_ctlCheck->SetCheck(BST_CHECKED);// check fix packers

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

/* CAUTION: ONLY x86 TESTED
 * get the thread id of the main thread of a target process
 *
 * params:
 *     DWORD dwPid  process id of the target process
 *
 * return:
 *     Success      thread id
 *     Error        NULL
 */
DWORD DumpModuleDlg::GetMainThreadId(DWORD dwPid)
{
    LPVOID lpTid;

    _asm
    {
        mov eax, fs:[18h]
        add eax, 36
        mov [lpTid], eax
    }

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, dwPid);
    if(hProcess == NULL)
        return NULL;

    DWORD dwTid;
    if(ReadProcessMemory(hProcess, lpTid, &dwTid, sizeof(dwTid), NULL) == FALSE)
    {
        CloseHandle(hProcess);
        return NULL;
    }

    CloseHandle(hProcess);

    return dwTid;
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

unsigned int DumpModuleDlg::GetEIPRegister()
{
DWORD thread_id = GetMainThreadId(processid);
unsigned int entrypoint = thread_id;
char epaddress[20];
wsprintf(epaddress,"%X",entrypoint);  // convert number to hex string

typedef HANDLE (WINAPI *OPENTHREAD)(DWORD ,BOOL,DWORD);
typedef BOOL (WINAPI *PINITIALIZECONTEXT)(PVOID Buffer, DWORD ContextFlags, PCONTEXT* Context, PDWORD ContextLength);

HMODULE kernel32 = LoadLibrary(_T("kernel32.dll"));
OPENTHREAD m_OpenThread =(OPENTHREAD)GetProcAddress(kernel32, "OpenThread"); 
HANDLE hThread = m_OpenThread(THREAD_ALL_ACCESS,TRUE,thread_id);

PCONTEXT Context;
DWORD ContextSize = 0;
PINITIALIZECONTEXT pfnInitializeContext = (PINITIALIZECONTEXT)GetProcAddress(kernel32, "InitializeContext");
ContextSize = 0;
int Success = pfnInitializeContext(NULL, CONTEXT_FULL, NULL, &ContextSize);

PVOID Buffer = malloc(ContextSize);
Success = pfnInitializeContext(Buffer,
CONTEXT_FULL,
&Context,
&ContextSize);

int contextOk = GetThreadContext(hThread, Context);

unsigned int eip = Context->Eip;

return eip;
}

void DumpModuleDlg::LastErrorDisplay() 
{ 
// Retrieve the system error message for the last-error code
DWORD dw = GetLastError();
char message[40];
wsprintf(message,"Error %d", dw);

MessageBox(message, TEXT("Error"), MB_OK); 

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

m_ctlCheck = (CButton*) GetDlgItem(IDC_ROUND_RAW_SIZE);
bool RoundRawSize = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
RoundRawSize = false;

m_ctlCheck = (CButton*) GetDlgItem(IDC_ORIGINAL_RAW);
bool OriginalRaw = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
OriginalRaw = true;

m_ctlCheck = (CButton*) GetDlgItem(IDC_RAW_EQ_VA);
bool RawEqVA = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
RawEqVA = true;

m_ctlCheck = (CButton*) GetDlgItem(IDC_CALCULATE_RAW);
bool CalculateRaw = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
CalculateRaw = true;

m_ctlCheck = (CButton*) GetDlgItem(IDC_SECTIONS_FROM_FILE);
bool SectionInfoFromFile = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
SectionInfoFromFile = true;

m_ctlCheck = (CButton*) GetDlgItem(IDC_FIX_PACKERS);
bool FixPackers = false;
ChkBox = m_ctlCheck->GetCheck();
if (ChkBox == BST_CHECKED)
FixPackers = true;

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

unsigned int e_lfnew = 0;
unsigned int base_address = 0;
int isok = 0;
int PEOffset = 0;
unsigned char* PeHeader = 0;
int SectionsPos =0;
unsigned int sizetocopy = 0;
unsigned int total_size=0;

unsigned int SignatureLen = 4 ; // 4 since is a dword, from Nt Headers
unsigned int FileHeaderLen = 2*2+3*4+2*2;
unsigned int SizeOfOptionHeaderRelPos = FileHeaderLen-2*2;

try
{

if (SectionInfoFromFile)   // read sections from disk
{
FILE* pFile = fopen (fullmodulename , "rb" );  // open file
fseek(pFile, 0x03C, SEEK_SET);
fread(tempbuf,04,1, pFile);  // buffer, size = item size in bytes, count = maximum number of items to be read.
PEOffset = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
if (PEOffset==0) return;

unsigned int SizeOfOptionHeader_pos = PEOffset+SignatureLen+SizeOfOptionHeaderRelPos;
fseek(pFile, SizeOfOptionHeader_pos, SEEK_SET);
fread(tempbuf,02,1, pFile);  // read SizeOfOptionHeader
unsigned int SizeOfOptionHeader = GenericPurposeMethods::UnsignedArrayToShort(tempbuf, 0);

SectionsPos = SizeOfOptionHeader+FileHeaderLen+SignatureLen;
fseek(pFile, PEOffset+SectionsPos+20, SEEK_SET);
fread(tempbuf,04,1, pFile);  // read RawOfFirstSection
unsigned int RawOfFirstSection = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);

sizetocopy = RawOfFirstSection;
if (sizetocopy>speed) sizetocopy=(unsigned int)speed;
if (sizetocopy==0) sizetocopy = speed;

total_size=total_size+RawOfFirstSection;

PeHeader = new byte[sizetocopy];
for (unsigned int i=0;i<sizetocopy;i++)
PeHeader[i] = 00;  // fill with 0

fseek(pFile, 0, SEEK_SET);
fread(PeHeader,sizetocopy,1, pFile);  // read PeHeader

fclose (pFile);  // close file
}
else   // else read sections from memory
{
e_lfnew = (unsigned int)hMod+0x03C;  // PE Offset from DOS HEADER
base_address = e_lfnew;
isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return;  // if read failed return

PEOffset = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
if (PEOffset==0) return;

unsigned int SizeOfOptionHeader_pos = (unsigned int)hMod+PEOffset+SignatureLen+SizeOfOptionHeaderRelPos;
int isok = ReadProcessMemory(hProcess, (LPVOID)SizeOfOptionHeader_pos, tempbuf, 02, &dwTotalRead);
if (isok == 0) return;  // if read failed return

unsigned int SizeOfOptionHeader = GenericPurposeMethods::UnsignedArrayToShort(tempbuf, 0);

//isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
//if (isok == 0) return;  // if read failed return

SectionsPos = SizeOfOptionHeader+FileHeaderLen+SignatureLen;

base_address = (unsigned int)hMod+PEOffset+SectionsPos+20;
isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return;  // if read failed return

unsigned int RawOfFirstSection = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
sizetocopy = RawOfFirstSection;
if (sizetocopy>speed) sizetocopy=(unsigned int)speed;
if (sizetocopy==0) sizetocopy = speed;

total_size=total_size+RawOfFirstSection;

PeHeader = new byte[sizetocopy];
for (unsigned int i=0;i<sizetocopy;i++)
PeHeader[i] = 00;  // fill with 0

isok = ReadProcessMemory(hProcess, (LPVOID)hMod, PeHeader, sizetocopy, &dwTotalRead);
if (isok == 0) return;  // if read failed return

}


unsigned int nrofsection = GenericPurposeMethods::UnsignedArrayToShort(PeHeader, PEOffset+0x06);
unsigned int sectionalignment = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x038);
unsigned int filealignment = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x03C);	    
unsigned int sizeofimage = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x050);
unsigned int calculatedimagesize = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+0x0C);
// calculatedimagesize = Virtual Address of first section first time!

if (FixPackers)
{
if (nrofsection>=2)
{
if (PeHeader[PEOffset+SectionsPos]=='U'&&PeHeader[PEOffset+SectionsPos+1]=='P'&&PeHeader[PEOffset+SectionsPos+2]=='X'
&&PeHeader[PEOffset+SectionsPos+0x28*1]=='U'&&PeHeader[PEOffset+SectionsPos+0x28*1+1]=='P'&&PeHeader[PEOffset+SectionsPos+0x28*1+2]=='X')
{  // If UPX

unsigned char* characteristics_array = GenericPurposeMethods::UIntToUnsignedArray(0x060000020);
//for (int j=0;j<04;j++)
//PeHeader[PEOffset+SectionsPos+0x024+j] = characteristics_array[j];  // fix characteristics of first section


}

}

}

for (unsigned int i = 0; i < nrofsection; i++)
{
int virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+0x28*i+8);
int toadd = (virtualsize%sectionalignment);
if (toadd!=0) toadd = sectionalignment-toadd;
calculatedimagesize = calculatedimagesize+virtualsize+toadd;
}

if (calculatedimagesize!=sizeofimage) sizeofimage=calculatedimagesize;
unsigned char* Dump = new unsigned char[sizeofimage];
for (unsigned int i2=0;i2<sizeofimage;i2++)
Dump[i2] = 00;  // fill with 0
unsigned char* Partkeep = new byte[sizeofimage];
for (unsigned i3=0;i2<sizeofimage;i3++)
Partkeep[i3] = 00;  // fill with 0

for (unsigned int j=0;j<sizetocopy;j++)
Dump[j] = PeHeader[j];  // copy PE Header on dump

unsigned int address=0;
bool ShouldFixrawsize=false;


			int rawsize,rawaddress,virtualsize,virtualAddress;
			int calcrawsize=0;

            for (i = 0; i < nrofsection; i++)
            {
            
            rawsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*i+0x010);
			rawaddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*i+0x014);
            virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*i+8);
            virtualAddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*i+0xC);

			
			if (OriginalRaw)
			{
			// information from memory
			if (RoundRawSize)  // if chosen to raw the file size!
			{
            
            int to_add_size = rawsize%filealignment;
			if (to_add_size!=0) to_add_size = filealignment-to_add_size;
			rawsize = rawsize+to_add_size;

			}
			
			}
            // RawSize = Virtual Size rounded on FileAlligment
            else if (RawEqVA)
            {
			rawaddress = virtualAddress; // so place bytes at the virtual address - big file size
            rawsize = virtualsize; // rawsize SHOULD BE FIXED!!!
			// rawsize shouldn't be rounded here - it should be equal with virtualsize

			unsigned int position = PEOffset+SectionsPos+0x28*i+0x010;
			unsigned char* vsize_array = GenericPurposeMethods::UIntToUnsignedArray(rawsize);
			for (int j=0;j<04;j++)
			Dump[position+j] = vsize_array[j];  // fix raw size

			position = PEOffset+SectionsPos+0x28*i+0x014;
			unsigned char* vaddress_array = GenericPurposeMethods::UIntToUnsignedArray(rawaddress);
			for (j=0;j<04;j++)
			Dump[position+j] = vaddress_array[j]; // fix raw address

            //BinaryWriter writer = new BinaryWriter(new MemoryStream(Dump));
            //writer.BaseStream.Position=PEOffset+0x0F8+0x28*i+16;
            //writer.Write(virtualsize);
            //writer.BaseStream.Position=PEOffset+0x0F8+0x28*i+20;
            //writer.Write(virtualAddress);
            //writer.Close();

            }
            else if (CalculateRaw)
            {
if (i==0)  // if first section
{
unsigned int rawaddress_of_first = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+0x014);
if (rawaddress_of_first==0||rawaddress_of_first>0x02000)
{

int SizeOfHeaders = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+0x054);
unsigned int new_rawaddress_of_first = SizeOfHeaders;
unsigned int position = PEOffset+SectionsPos+0x014;

rawaddress = new_rawaddress_of_first;
unsigned char* raddress_array = GenericPurposeMethods::UIntToUnsignedArray(new_rawaddress_of_first);
for (j=0;j<04;j++)
Dump[position+j] = raddress_array[j]; // fix raw address

}

unsigned int rawsize_of_first = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+0x010);

if (rawsize_of_first==0||rawaddress_of_first>0xF0000000)
{
unsigned int virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+8);
rawsize = virtualsize;

unsigned char* rsize_array = GenericPurposeMethods::UIntToUnsignedArray(rawsize);
for (j=0;j<04;j++)
Dump[PEOffset+SectionsPos+0x10+j] = rsize_array[j]; // fix raw size

}


}
else  // for all section except first section:
{
unsigned int PrevSectionRawAddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*(i-1)+0x014);
unsigned int PrevSectionRawSize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*(i-1)+0x010);

int to_add_size = PrevSectionRawSize%filealignment;
if (to_add_size!=0) to_add_size = filealignment-to_add_size;

unsigned int NewRawAddress = PrevSectionRawAddress+PrevSectionRawSize+to_add_size;

unsigned int raw_address_position = PEOffset+SectionsPos+0x28*i+0x014;
unsigned char* raddress_array = GenericPurposeMethods::UIntToUnsignedArray(NewRawAddress);
for (j=0;j<04;j++)
Dump[raw_address_position+j] = raddress_array[j]; // fix raw address

rawaddress = NewRawAddress;
}

}
            
			isok = ReadProcessMemory(hProcess, (LPVOID)((unsigned int)hMod+virtualAddress), Partkeep, virtualsize, &dwTotalRead);
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
			
			// Copy section bytes:
            //Array.Copy(Partkeep, 0, Dump, offset, rawsize);
			for (int z=0;z<rawsize;z++)
			Dump[rawaddress+z] = Partkeep[z];
            //offset = offset+rawsize;

			}  // for each section end!

			 // calculate file total_size:
			for (i = nrofsection-1; i >= 0; --i)
            {
            
            rawsize = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*i+0x010);
			rawaddress = GenericPurposeMethods::UnsignedArrayToUInt(Dump, PEOffset+SectionsPos+0x28*i+0x014);

			if (rawaddress!=0)  // last valid raw address
			{
			total_size = rawsize+rawaddress;
			break;
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

			if (Dump!=NULL&&sizeofimage>0&&sizeofimage>=total_size)
            {  // if file total_size <= sizeofimage

            FILE * pFile = fopen ( filename , "w+b" );  // create file
            fwrite (Dump , 1 , total_size , pFile );  // save bytes file
            fclose (pFile);  // close file

			GetDlgItem(IDC_STATUS_ST)->SetWindowText("Module dumped!");
            }
	


}
catch(...)  // catch any exception
{
}


CloseHandle(hProcess);

}

unsigned int DumpModuleDlg::GetModuleSize(HMODULE hMod)
{

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

unsigned int SignatureLen = 4 ; // 4 since is a dword, from Nt Headers
unsigned int FileHeaderLen = 2*2+3*4+2*2;
unsigned int SizeOfOptionHeaderRelPos = FileHeaderLen-2*2;
unsigned int SizeOfOptionHeader_pos = (unsigned int)hMod+PEOffset+SignatureLen+SizeOfOptionHeaderRelPos;
isok = ReadProcessMemory(hProcess, (LPVOID)SizeOfOptionHeader_pos, tempbuf, 02, &dwTotalRead);
if (isok == 0) return 0;  // if read failed return 0

unsigned int SizeOfOptionHeader = GenericPurposeMethods::UnsignedArrayToShort(tempbuf, 0);

base_address = (unsigned int)hMod+PEOffset+SizeOfOptionHeader+20;
isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return 0;  // if read failed return

unsigned int RawOfFirstSection = GenericPurposeMethods::UnsignedArrayToUInt(tempbuf, 0);
unsigned int offset=0;

unsigned int sizetocopy = RawOfFirstSection;
if (sizetocopy>speed) sizetocopy=(unsigned int)speed;
if (sizetocopy==0) sizetocopy = speed;

offset=offset+RawOfFirstSection;

unsigned char* PeHeader = new byte[sizetocopy];
isok = ReadProcessMemory(hProcess, (LPVOID)hMod, PeHeader, sizetocopy, &dwTotalRead);
if (isok == 0) return 0;  // if read failed return

unsigned int nrofsection = GenericPurposeMethods::UnsignedArrayToShort(PeHeader, PEOffset+0x06);

base_address = (unsigned int)hMod+PEOffset+0x28;
isok = ReadProcessMemory(hProcess, (LPVOID)base_address, tempbuf, 04, &dwTotalRead);
if (isok == 0) return 0;  // if read failed return 0

unsigned int modulesize = 0;

int SectionsPos = SizeOfOptionHeader+FileHeaderLen+SignatureLen;

			for (int i = nrofsection-1; i >= 0; --i)
            {
			unsigned int virtualsize = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+0x28*i+8);
            unsigned int virtualAddress = GenericPurposeMethods::UnsignedArrayToUInt(PeHeader, PEOffset+SectionsPos+0x28*i+0xC);
			if (virtualAddress!=0)
			{
			modulesize = virtualsize+virtualAddress;
			break;
			}
			}

CloseHandle(hProcess);

return modulesize;

}

void DumpModuleDlg::OnButCurrentEip() 
{
	// TODO: Add your control notification handler code here
	unsigned int module_size = GetModuleSize(hMod);
	unsigned int current_EIP = GetEIPRegister();

	if (current_EIP<(unsigned int)hMod)
	GetDlgItem(IDC_STATUS_ST)->SetWindowText("EIP before module base!");
	else if(current_EIP>((unsigned int)hMod+module_size))
	GetDlgItem(IDC_STATUS_ST)->SetWindowText("EIP after (module_base+module_size)!");
	else  // convert EIP to RVA
	current_EIP = current_EIP-(unsigned int)hMod;
	
	char str_EIP[20];
	wsprintf(str_EIP,"%X",current_EIP);  // convert number to hex string
	GetDlgItem(IDC_NEWEntryPoint)->SetWindowText(str_EIP);
}
