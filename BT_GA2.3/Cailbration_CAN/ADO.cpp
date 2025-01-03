#include "StdAfx.h"
#include "ADO.h"

CADO::CADO(void)
{
}

CADO::~CADO(void)
{
}

// //初始化建立连接
bool CADO::OnInitADOConn(void)
{
	HRESULT hr;  //创建对象
	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");    //创建Connection对象
		if (SUCCEEDED(hr))         //创建成功
		{
			hr = m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=E:\\database\\test.mdb","","",adModeUnknown);///Á¬½ÓÊý¾Ý¿â
		//	AfxMessageBox(L"连接数据库成功！");
		}
	}
	catch(_com_error e)
	{
		CString errorMsg;
		errorMsg.Format(L"连接数据库失败！\r\n错误信息:%s",e.ErrorMessage());
		AfxMessageBox(errorMsg);         //显示错误信息
	}
	return TRUE;

}

// // 打开数据集
_RecordsetPtr& CADO::OpenRecordset(CString sql)
{
	//TODO: insert return statement here
		//TODO: insert return statement here
	ASSERT(!sql.IsEmpty());
	try
	{
		m_pRecordset.CreateInstance("ADODB.Recordset");

		if (m_pRecordset == NULL)
		{
			AfxMessageBox(L"RecordSet 对象创建失败! 请确认是否初始化了COM环境.");
		}

		//设置游标属性 不知道为什么要设置这个，但是不设置这个游标属性为3或者adUseClient的话返回的数据集数目为-1，获取不到记录
		m_pRecordset->CursorLocation = adUseClient;  
		//打开记录集
		m_pRecordset->Open(_variant_t(sql),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockOptimistic,adCmdText);

	}
	catch(_com_error e)
	{
		CString errorMsg;
		errorMsg.Format(L"连接数据库失败！\r\n错误信息:%s",e.ErrorMessage());
		AfxMessageBox(errorMsg);         //显示错误信息
	}
	return m_pRecordset;

}

// 退出连接
void CADO::ExitConection(void)
{
	if(m_pRecordset->GetState() == adStateOpen)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}
	if (m_pConnection->State)
	{
		m_pConnection->Close();
		m_pConnection = NULL;
	}

}

// 得到记录
_RecordsetPtr& CADO::GetRecordset(void)
{
	//TODO: insert return statement here
	return m_pRecordset;
}

// 断开数据库
void CADO::ExitConnection(void)
{
	if(m_pRecordset->GetState() == adStateOpen)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}
	if (m_pConnection->State)
	{
		m_pConnection->Close();
		m_pConnection = NULL;
	}

}

int CADO::OnInitADOConn(CString dbcName)
{
	HRESULT hr;  //创建对象

	//dbcName = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=E:\\database\\" + dbcName;    //调试版本
	dbcName = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbcName;                //运行版本

	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");    //创建Connection对象
		if (SUCCEEDED(hr))         //创建成功
		{
			hr = m_pConnection->Open((_bstr_t)dbcName,"","",adModeUnknown);///Á¬½ÓÊý¾Ý¿â
		//	AfxMessageBox(L"连接数据库成功！");
		}
	}
	catch(_com_error e)
	{
		CString errorMsg;
		errorMsg.Format(L"连接数据库失败！\r\n错误信息:%s",e.ErrorMessage());
		AfxMessageBox(errorMsg);         //显示错误信息
		return FALSE;
	}
	return TRUE;
	
}

_ConnectionPtr CADO::GetConnec(void)
{
	return m_pConnection;
}
