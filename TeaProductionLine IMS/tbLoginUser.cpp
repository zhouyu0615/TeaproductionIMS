// tbLoginUser.h : CtbLoginUser 类的实现



// CtbLoginUser 实现

// 代码生成在 2015年7月26日 星期日, 08:58

#include "stdafx.h"
#include "tbLoginUser.h"
IMPLEMENT_DYNAMIC(CtbLoginUser, CRecordset)

CtbLoginUser::CtbLoginUser(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_LoginName = "";
	m_LoginPassWd = "";
	m_LastLoginInTime;
	m_CreatedDateTime;
	m_PermissionLevel = 0;
	m_LastUpdatedDateTime;
	m_IsAutoLogin = 0;
	m_IsRememberPasswd = 0;
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error 安全问题：连接字符串可能包含密码。
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CtbLoginUser::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbLoginUser::GetDefaultSQL()
{
	return _T("[dbo].[tbLoginUser]");
}

void CtbLoginUser::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Text(pFX, _T("[LoginName]"), m_LoginName);
	RFX_Text(pFX, _T("[LoginPassWd]"), m_LoginPassWd);
	RFX_Date(pFX, _T("[LastLoginInTime]"), m_LastLoginInTime);
	RFX_Date(pFX, _T("[CreatedDateTime]"), m_CreatedDateTime);
	RFX_Long(pFX, _T("[PermissionLevel]"), m_PermissionLevel);
	RFX_Date(pFX, _T("[LastUpdatedDateTime]"), m_LastUpdatedDateTime);
	RFX_Long(pFX, _T("[IsAutoLogin]"), m_IsAutoLogin);
	RFX_Long(pFX, _T("[IsRememberPasswd]"), m_IsRememberPasswd);

}
/////////////////////////////////////////////////////////////////////////////
// CtbLoginUser 诊断

#ifdef _DEBUG
void CtbLoginUser::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbLoginUser::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


