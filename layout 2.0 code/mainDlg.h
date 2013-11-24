#if !defined(AFX_MAINDLG_H__8E5464D3_779E_4E81_A349_996F0B8CBF1F__INCLUDED_)
#define AFX_MAINDLG_H__8E5464D3_779E_4E81_A349_996F0B8CBF1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CmainDlg dialog
class CmainDlg : public CDialog
{
public://ÊÖ¶¯Ìí¼Ó-------------
	int MySetPixelFormat(HDC hdc);
	HDC hdc ;
	HGLRC hglrc;
	void draw();

// Construction
public:
	CmainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CmainDlg)
	enum { IDD = IDD_mainDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmainDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CmainDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void Onabout();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void Oninstruction();
	afx_msg void Onrand();
	afx_msg void Oncomplete();
	afx_msg void Oncontrol();
	afx_msg void Ondim2();
	afx_msg void OnsetNv();
	afx_msg void Onlayouthot();
	afx_msg void Onlayoutwarm();
	afx_msg void Onlayoutcool();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__8E5464D3_779E_4E81_A349_996F0B8CBF1F__INCLUDED_)
