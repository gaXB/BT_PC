#pragma once


// CDragEdit

class CDragEdit : public CEdit
{
	DECLARE_DYNAMIC(CDragEdit)

public:
	CDragEdit();
	virtual ~CDragEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


