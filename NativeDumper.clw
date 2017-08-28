; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=DumpModuleDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NativeDumper.h"

ClassCount=5
Class1=CNativeDumperApp
Class2=CNativeDumperDlg

ResourceCount=6
Resource1=IDR_MAINFRAME
Resource2=IDR_PROCESSMENU
Class3=DumpModuleDlg
Class4=NewEdit
Resource3=IDD_MODULES
Class5=ModulesDlg
Resource4=IDD_DUMPMODULEDLG_DIALOG
Resource5=IDD_NATIVEDUMPER_DIALOG
Resource6=IDR_MODULEMENU

[CLS:CNativeDumperApp]
Type=0
HeaderFile=NativeDumper.h
ImplementationFile=NativeDumper.cpp
Filter=N

[CLS:CNativeDumperDlg]
Type=0
HeaderFile=NativeDumperDlg.h
ImplementationFile=NativeDumperDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CNativeDumperDlg



[DLG:IDD_NATIVEDUMPER_DIALOG]
Type=1
Class=CNativeDumperDlg
ControlCount=1
Control1=IDC_LIST1,SysListView32,1350631425

[DLG:IDD_DUMPMODULEDLG_DIALOG]
Type=1
Class=DumpModuleDlg
ControlCount=16
Control1=IDC_CHANGEEP,button,1342242819
Control2=IDC_FixSizeOfImage,button,1342242819
Control3=IDC_STATIC,static,1342308352
Control4=IDC_NEWEntryPoint,edit,1350631552
Control5=IDC_DumpBTN,button,1342242816
Control6=IDC_STATUS_ST,static,1342308352
Control7=IDC_ROUND_RAW_SIZE,button,1342242819
Control8=IDC_ORIGINAL_RAW,button,1342308361
Control9=IDC_RAW_EQ_VA,button,1342177289
Control10=IDC_CALCULATE_RAW,button,1342177289
Control11=IDC_SECTIONS_FROM_MEMORY,button,1342308361
Control12=IDC_STATIC,button,1342177287
Control13=IDC_SECTIONS_FROM_FILE,button,1342177289
Control14=IDC_BUT_CURRENT_EIP,button,1342242816
Control15=IDC_STATIC,button,1342177287
Control16=IDC_FIX_PACKERS,button,1342242819

[CLS:DumpModuleDlg]
Type=0
HeaderFile=DumpModuleDlg.h
ImplementationFile=DumpModuleDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=DumpModuleDlg
VirtualFilter=dWC

[CLS:NewEdit]
Type=0
HeaderFile=NewEdit.h
ImplementationFile=NewEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=NewEdit

[CLS:ModulesDlg]
Type=0
HeaderFile=ModulesDlg.h
ImplementationFile=ModulesDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ModulesDlg

[MNU:IDR_PROCESSMENU]
Type=1
Class=?
Command1=ID_MODULES
Command2=ID_DUMPMAINMODULE
Command3=ID_REFRESH
CommandCount=3

[DLG:IDD_MODULES]
Type=1
Class=ModulesDlg
ControlCount=1
Control1=IDC_LIST1,SysListView32,1350631425

[MNU:IDR_MODULEMENU]
Type=1
Class=?
Command1=ID_MODULE_DUMP
Command2=ID_MODULE_REFRESH
CommandCount=2

