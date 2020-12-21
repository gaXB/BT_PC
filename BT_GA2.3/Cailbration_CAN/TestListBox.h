#if !defined(AFX_TESTLISTBOX_H__14AF8E00_C055_4880_8E2D_EF702C355078__INCLUDED_)
#define AFX_TESTLISTBOX_H__14AF8E00_C055_4880_8E2D_EF702C355078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestListBox window

class CTestListBox : public CListBox
{
// Construction
public:
	CTestListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestListBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuRefresh();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLISTBOX_H__14AF8E00_C055_4880_8E2D_EF702C355078__INCLUDED_)
