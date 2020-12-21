#include "StdAfx.h"
#include "EditHexInput.h"

#define EN_ESCUP WM_USER+0x0903
#define EN_ENTERUP WM_USER+0x904
CEditHexInput::CEditHexInput(void)
{
}

CEditHexInput::~CEditHexInput(void)
{
}
BEGIN_MESSAGE_MAP(CEditHexInput, CEdit)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CEditHexInput::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CEdit::PreSubclassWindow();
	SetLimitText(8);
}
void CEditHexInput::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if(nChar>='a'&&nChar<='f')
	//{
	//	nChar=nChar+'A'-'a';
	//	SendMessage(WM_CHAR,nChar,nFlags);
	//	return;
	//}
	if(nChar>='a'&&nChar<='f')
	{
		nChar=nChar+'A'-'a';
	}
	if(('0'<=nChar&&nChar<='9')||nChar==VK_BACK||('A'<=nChar&&nChar<='F'))
	{
		CEdit::OnChar(nChar,nRepCnt,nFlags);
	}
	

}

void CEditHexInput::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if(nChar==0x0d)
	//	GetParent()->PostMessage (WM_COMMAND,(WPARAM)(EN_ENTERUP<<16|GetDlgCtrlID()),(LPARAM)m_hWnd);

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditHexInput::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CEdit::OnLButtonDown(nFlags, point);
}

void CEditHexInput::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	ShowCaret();
}


void CEditHexInput::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

}

int CEditHexInput::GetInt(void)
{
    CString str;
	GetWindowText(str);
	int dem=0;

    for(int i=0;i<str.GetLength();i++)
    {
        dem=dem*16;
        if((str[i]<='9')&&(str[i]>='0'))        //0~9之间的字符
            dem+=str[i]-'0';
        else if((str[i]<='F')&&(str[i]>='A'))   //A~F之间的字符
            dem+=str[i]-'A'+10;
        else if((str[i]<='f')&&(str[i]>='a'))   //a~f之间的字符
            dem+=str[i]-'a'+10;
        else
            return -1;                          //出错时返回-1
    }
    return dem; 
}
