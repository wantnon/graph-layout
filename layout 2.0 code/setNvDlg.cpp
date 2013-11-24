// setNvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glmfc.h"
#include "setNvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CsetNvDlg dialog


CsetNvDlg::CsetNvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsetNvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CsetNvDlg)
	m_Nv = 0;
	//}}AFX_DATA_INIT
}


void CsetNvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CsetNvDlg)
	DDX_Text(pDX, IDC_Nv, m_Nv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CsetNvDlg, CDialog)
	//{{AFX_MSG_MAP(CsetNvDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CsetNvDlg message handlers

void CsetNvDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	UpdateData(false);
	CDialog::OnOK();
}
