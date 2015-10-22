// tbProductionLine.h : CtbProductionLine 类的实现



// CtbProductionLine 实现

// 代码生成在 2015年7月29日 星期三, 19:34

#include "stdafx.h"
#include "tbProductionLine.h"
IMPLEMENT_DYNAMIC(CtbProductionLine, CRecordset)

CtbProductionLine::CtbProductionLine(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_CreatedDateTime;
	m_LastUpdatedDateTime;
	m_LineName = "";
	m_UserId = 0;
	m_Capacity = "";
	m_Description = "";
	m_SortIndex = 0;
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbProductionLine::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbProductionLine::GetDefaultSQL()
{
	return _T("[dbo].[tbProductionLine]");
}

void CtbProductionLine::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Date(pFX, _T("[CreatedDateTime]"), m_CreatedDateTime);
	RFX_Date(pFX, _T("[LastUpdatedDateTime]"), m_LastUpdatedDateTime);
	RFX_Text(pFX, _T("[LineName]"), m_LineName);
	RFX_Long(pFX, _T("[UserId]"), m_UserId);
	RFX_Text(pFX, _T("[Capacity]"), m_Capacity);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Long(pFX, _T("[SortIndex]"), m_SortIndex);

}
/////////////////////////////////////////////////////////////////////////////
// CtbProductionLine 诊断

#ifdef _DEBUG
void CtbProductionLine::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbProductionLine::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


