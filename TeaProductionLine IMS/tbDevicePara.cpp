// tbDevicePara.h : CtbDevicePara 类的实现



// CtbDevicePara 实现

// 代码生成在 2015年7月26日 星期日, 16:18

#include "stdafx.h"
#include "tbDevicePara.h"
IMPLEMENT_DYNAMIC(CtbDevicePara, CRecordset)

CtbDevicePara::CtbDevicePara(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_CreatedDateTime;
	m_LastUpdatedDateTime;
	m_ProductionLineId = 0;
	m_ProcessModuleId = 0;
	m_PLCId = 0;
	m_DeviceId = 0;
	m_ProductionLineName = "";
	m_ProcessModuleName = "";
	m_PlcName = "";
	m_DeviceName = "";
	m_ControlValue = 0;
	m_StateValue = 0;
	m_ControlAddrIndex = "";
	m_StateAddrIndex = "";
	m_Description = "";
	m_ParaName = "";
	m_nFields = 17;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbDevicePara::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbDevicePara::GetDefaultSQL()
{
	return _T("[dbo].[tbDevicePara]");
}

void CtbDevicePara::DoFieldExchange(CFieldExchange* pFX)
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
	RFX_Long(pFX, _T("[PLCId]"), m_PLCId);
	RFX_Long(pFX, _T("[DeviceId]"), m_DeviceId);
	RFX_Text(pFX, _T("[ProductionLineName]"), m_ProductionLineName);
	RFX_Text(pFX, _T("[ProcessModuleName]"), m_ProcessModuleName);
	RFX_Text(pFX, _T("[PlcName]"), m_PlcName);
	RFX_Text(pFX, _T("[DeviceName]"), m_DeviceName);
	RFX_Long(pFX, _T("[ControlValue]"), m_ControlValue);
	RFX_Long(pFX, _T("[StateValue]"), m_StateValue);
	RFX_Text(pFX, _T("[ControlAddrIndex]"), m_ControlAddrIndex);
	RFX_Text(pFX, _T("[StateAddrIndex]"), m_StateAddrIndex);
	RFX_Text(pFX, _T("[Description]"), m_Description);
	RFX_Text(pFX, _T("[ParaName]"), m_ParaName);

}
/////////////////////////////////////////////////////////////////////////////
// CtbDevicePara 诊断

#ifdef _DEBUG
void CtbDevicePara::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbDevicePara::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


