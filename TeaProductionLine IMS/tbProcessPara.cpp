// tbProcessPara.h : CtbProcessPara 类的实现



// CtbProcessPara 实现

// 代码生成在 2015年7月31日 星期五, 15:14

#include "stdafx.h"
#include "tbProcessPara.h"
IMPLEMENT_DYNAMIC(CtbProcessPara, CRecordset)

CtbProcessPara::CtbProcessPara(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_CreatedDateTime;
	m_LastUpdatedDateTime;
	m_PLCId = 0;
	m_ProcessModuleId = 0;
	m_ProductionLineId = 0;
	m_ProductionLineName = "";
	m_ProcessModuleName = "";
	m_PlcName = "";
	m_Description = "";
	m_ParaValue = 0.0;
	m_ParaName = "";
	m_IsConfig = 0;
	m_IsVisible = 0;
	m_ValueType = 0;
	m_ReadAddressIndex = "";
	m_AddressType = "";
	m_WriteAddressIndex = "";
	m_Units = "";
	m_nFields = 19;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbProcessPara::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbProcessPara::GetDefaultSQL()
{
	return _T("[dbo].[tbProcessPara]");
}

void CtbProcessPara::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Date(pFX, _T("[CreatedDateTime]"), m_CreatedDateTime);
	RFX_Date(pFX, _T("[LastUpdatedDateTime]"), m_LastUpdatedDateTime);
	RFX_Long(pFX, _T("[PLCId]"), m_PLCId);
	RFX_Long(pFX, _T("[ProcessModuleId]"), m_ProcessModuleId);
	RFX_Long(pFX, _T("[ProductionLineId]"), m_ProductionLineId);
	RFX_Text(pFX, _T("[ProductionLineName]"), m_ProductionLineName);
	RFX_Text(pFX, _T("[ProcessModuleName]"), m_ProcessModuleName);
	RFX_Text(pFX, _T("[PlcName]"), m_PlcName);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Single(pFX, _T("[ParaValue]"), m_ParaValue);
	RFX_Text(pFX, _T("[ParaName]"), m_ParaName);
	RFX_Long(pFX, _T("[IsConfig]"), m_IsConfig);
	RFX_Long(pFX, _T("[IsVisible]"), m_IsVisible);
	RFX_Long(pFX, _T("[ValueType]"), m_ValueType);
	RFX_Text(pFX, _T("[ReadAddressIndex]"), m_ReadAddressIndex);
	RFX_Text(pFX, _T("[AddressType]"), m_AddressType);
	RFX_Text(pFX, _T("[WriteAddressIndex]"), m_WriteAddressIndex);
	RFX_Text(pFX, _T("[Units]"), m_Units);

}
/////////////////////////////////////////////////////////////////////////////
// CtbProcessPara 诊断

#ifdef _DEBUG
void CtbProcessPara::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbProcessPara::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


