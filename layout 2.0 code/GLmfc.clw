; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CmainDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "glmfc.h"
LastPage=0

ClassCount=3
Class1=CinputDlg
Class2=CmainDlg

ResourceCount=3
Resource1=IDD_setNv
Resource2=IDD_mainDlg
Class3=CsetNvDlg
Resource3=IDR_MENU

[CLS:CinputDlg]
Type=0
BaseClass=CDialog
HeaderFile=inputDlg.h
ImplementationFile=inputDlg.cpp
LastObject=ID_minMap

[CLS:CmainDlg]
Type=0
BaseClass=CDialog
HeaderFile=mainDlg.h
ImplementationFile=mainDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=ID_dim2

[DLG:IDD_mainDlg]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1208025089
Control2=IDCANCEL,button,1073807360

[MNU:IDR_MENU]
Type=1
Class=?
Command1=ID_dim2
Command2=ID_setNv
Command3=ID_complete
Command4=ID_rand
Command5=ID_layout_hot
Command6=ID_layout_warm
Command7=ID_layout_cool
Command8=ID_control
Command9=ID_instruction
Command10=ID_about
CommandCount=10

[DLG:IDD_setNv]
Type=1
Class=CsetNvDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_Nv,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CsetNvDlg]
Type=0
HeaderFile=setNvDlg.h
ImplementationFile=setNvDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_layout_hot

