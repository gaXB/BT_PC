// DragEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "DragEdit.h"


// CDragEdit

IMPLEMENT_DYNAMIC(CDragEdit, CEdit)

CDragEdit::CDragEdit()
{

}

CDragEdit::~CDragEdit()
{
}


BEGIN_MESSAGE_MAP(CDragEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CDragEdit 消息处理程序



void CDragEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    WORD wNumFileDropped = DragQueryFile(hDropInfo, -1, NULL, 0);  

	WCHAR  szFileName[261];
    CString firstFile = L"";  

    for (WORD x = 0; x < wNumFileDropped; x++){  
        WORD wPathnameSize = DragQueryFile(hDropInfo, x, (LPWSTR)szFileName, MAX_PATH);  
		//szFileName[250] = 0x21;
    }  


    DragFinish(hDropInfo);  
    SetWindowText(szFileName);  
    UpdateData(FALSE);  
	CEdit::OnDropFiles(hDropInfo);
}
