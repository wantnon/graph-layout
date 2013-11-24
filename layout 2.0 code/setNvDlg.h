#if !defined(AFX_SETNVDLG_H__69C0DEAE_7F36_45FF_BF39_04715539B35E__INCLUDED_)
#define AFX_SETNVDLG_H__69C0DEAE_7F36_45FF_BF39_04715539B35E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// setNvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CsetNvDlg dialog

class CsetNvDlg : public CDialog
{
// Construction
public:
	CsetNvDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CsetNvDlg)
	enum { IDD = IDD_setNv };
	int		m_Nv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CsetNvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CsetNvDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETNVDLG_H__69C0DEAE_7F36_45FF_BF39_04715539B35E__INCLUDED_)
