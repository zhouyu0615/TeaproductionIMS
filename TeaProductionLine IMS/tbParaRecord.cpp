// tbParaRecord.h : CtbParaRecord 类的实现



// CtbParaRecord 实现

// 代码生成在 2015年10月31日 星期六, 11:47

#include "stdafx.h"
#include "tbParaRecord.h"
IMPLEMENT_DYNAMIC(CtbParaRecord, CRecordset)

CtbParaRecord::CtbParaRecord(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_CreatedTime;
	m_ProParaId = 0;
	m_ParaValue = 0.0;
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbParaRecord::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbParaRecord::GetDefaultSQL()
{
	return _T("[dbo].[tbParaRecord]");
}

void CtbParaRecord::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Date(pFX, _T("[CreatedTime]"), m_CreatedTime);
	RFX_Long(pFX, _T("[ProParaId]"), m_ProParaId);
	RFX_Single(pFX, _T("[ParaValue]"), m_ParaValue);

}
/////////////////////////////////////////////////////////////////////////////
// CtbParaRecord 诊断

#ifdef _DEBUG
void CtbParaRecord::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbParaRecord::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


