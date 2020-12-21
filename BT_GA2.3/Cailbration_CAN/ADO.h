#pragma once

class CADO
{
public:
	CADO(void);
	~CADO(void);


	// //初始化建立连接
	bool OnInitADOConn(void);
	// // 打开数据集
	_RecordsetPtr& OpenRecordset(CString sql);
	// 退出连接
	void ExitConection(void);

public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;

public:
	// 得到记录
	_RecordsetPtr& GetRecordset(void);
	// 断开数据库
	void ExitConnection(void);
public:
	int OnInitADOConn(CString dbcName);
public:
	_ConnectionPtr GetConnec(void);
};
