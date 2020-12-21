#pragma once
#include "afxwin.h"

class CEditHexInput :
	public CEdit
{
public:
	CEditHexInput(void);
	~CEditHexInput(void);	
	int GetInt(void);
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnKillfocus();
	afx_msg void OnKillFocus(CWnd* pNewWnd);

};
