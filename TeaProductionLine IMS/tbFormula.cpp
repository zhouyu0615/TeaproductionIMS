// tbFormula.h : CtbFormula 类的实现



// CtbFormula 实现

// 代码生成在 2015年8月1日 星期六, 08:12

#include "stdafx.h"
#include "tbFormula.h"
IMPLEMENT_DYNAMIC(CtbFormula, CRecordset)

CtbFormula::CtbFormula(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_FormulaName = "";
	m_FormulaId = 0;
	m_ProductionLineId = 0;
	m_ProductionLineName = "";
	m_ProcessParaId = 0;
	m_ProcessParaName = "";
	m_DefaultValue = 0.0;
	m_ParaValueUnit = "";
	m_Note = "";
	m_CreatedDateTime;
	m_LastUpdatedDateTime;
	m_IsDefaultFormula = 0;
	m_IsCurrentFormula = 0;
	m_nFields = 14;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbFormula::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbFormula::GetDefaultSQL()
{
	return _T("[dbo].[tbFormula]");
}

void CtbFormula::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Text(pFX, _T("[FormulaName]"), m_FormulaName);
	RFX_Long(pFX, _T("[FormulaId]"), m_FormulaId);
	RFX_Long(pFX, _T("[ProductionLineId]"), m_ProductionLineId);
	RFX_Text(pFX, _T("[ProductionLineName]"), m_ProductionLineName);
	RFX_Long(pFX, _T("[ProcessParaId]"), m_ProcessParaId);
	RFX_Text(pFX, _T("[ProcessParaName]"), m_ProcessParaName);
	RFX_Single(pFX, _T("[DefaultValue]"), m_DefaultValue);
	RFX_Text(pFX, _T("[ParaValueUnit]"), m_ParaValueUnit);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Date(pFX, _T("[CreatedDateTime]"), m_CreatedDateTime);
	RFX_Date(pFX, _T("[LastUpdatedDateTime]"), m_LastUpdatedDateTime);
	RFX_Long(pFX, _T("[IsDefaultFormula]"), m_IsDefaultFormula);
	RFX_Long(pFX, _T("[IsCurrentFormula]"), m_IsCurrentFormula);

}
/////////////////////////////////////////////////////////////////////////////
// CtbFormula 诊断

#ifdef _DEBUG
void CtbFormula::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbFormula::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


