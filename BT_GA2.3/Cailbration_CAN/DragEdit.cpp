// DragEdit.cpp : ʵ���ļ�
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



// CDragEdit ��Ϣ�������



void CDragEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
