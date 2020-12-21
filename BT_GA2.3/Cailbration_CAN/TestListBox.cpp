// TestListBox.cpp : implementation file
//

#include "stdafx.h"
#include "TestListBox.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestListBox

CTestListBox::CTestListBox()
{
}

CTestListBox::~CTestListBox()
{
}


BEGIN_MESSAGE_MAP(CTestListBox, CListBox)
	//{{AFX_MSG_MAP(CTestListBox)
	ON_WM_RBUTTONDOWN()
	//ON_COMMAND(ID_MENU_REFRESH, OnMenuRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestListBox message handlers

void CTestListBox::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	
	CMenu *submenu;
	submenu=menu.GetSubMenu(0);
	
	CPoint tmppoint=point;
	ClientToScreen(&tmppoint);
	submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,tmppoint.x,tmppoint.y,
		this);
	
	CListBox::OnRButtonDown(nFlags, point);
}

void CTestListBox::OnMenuRefresh() 
{
	// TODO: Add your command handler code here
	DWORD len=GetCount();
	for(DWORD i=0;i<len;i++)
		DeleteString(0);
	
}
