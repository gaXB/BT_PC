#pragma once

class CADO
{
public:
	CADO(void);
	~CADO(void);


	// //��ʼ����������
	bool OnInitADOConn(void);
	// // �����ݼ�
	_RecordsetPtr& OpenRecordset(CString sql);
	// �˳�����
	void ExitConection(void);

public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;

public:
	// �õ���¼
	_RecordsetPtr& GetRecordset(void);
	// �Ͽ����ݿ�
	void ExitConnection(void);
public:
	int OnInitADOConn(CString dbcName);
public:
	_ConnectionPtr GetConnec(void);
};
