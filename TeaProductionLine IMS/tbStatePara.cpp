// tbStatePara.h : CtbStatePara 类的实现



// CtbStatePara 实现

// 代码生成在 2015年7月27日 星期一, 09:27

#include "stdafx.h"
#include "tbStatePara.h"
IMPLEMENT_DYNAMIC(CtbStatePara, CRecordset)

CtbStatePara::CtbStatePara(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_CreatedDateTime;
	m_LastUpdatedDateTime;
	m_ProductionLineId = 0;
	m_ProcessModuleId = 0;
	m_ProductionLineName = "";
	m_ProcessModuleName = "";
	m_PLCId = 0;
	m_ParaName = "";
	m_AddressIndex = "";
	m_ParaValue = 0;
	m_Description = "";
	m_PlcName = "";
	m_nFields = 13;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbStatePara::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbStatePara::GetDefaultSQL()
{
	return _T("[dbo].[tbStatePara]");
}

void CtbStatePara::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Date(pFX, _T("[CreatedDateTime]"), m_CreatedDateTime);
	RFX_Date(pFX, _T("[LastUpdatedDateTime]"), m_LastUpdatedDateTime);
	RFX_Long(pFX, _T("[ProductionLineId]"), m_ProductionLineId);
	RFX_Long(pFX, _T("[ProcessModuleId]"), m_ProcessModuleId);
	RFX_Text(pFX, _T("[ProductionLineName]"), m_ProductionLineName);
	RFX_Text(pFX, _T("[ProcessModuleName]"), m_ProcessModuleName);
	RFX_Long(pFX, _T("[PLCId]"), m_PLCId);
	RFX_Text(pFX, _T("[ParaName]"), m_ParaName);
	RFX_Text(pFX, _T("[AddressIndex]"), m_AddressIndex);
	RFX_Long(pFX, _T("[ParaValue]"), m_ParaValue);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[PlcName]"), m_PlcName);

}
/////////////////////////////////////////////////////////////////////////////
// CtbStatePara 诊断

#ifdef _DEBUG
void CtbStatePara::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbStatePara::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


