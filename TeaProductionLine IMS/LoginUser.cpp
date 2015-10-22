#include "stdafx.h"
#include "LoginUser.h"


CLoginUser::CLoginUser()
{
	m_Id = 0;
	m_strLoginName = _T("");
	m_strLoginPassWd = _T("");
	m_strCreatedDateTime = _T("");
	m_strLastLoginInTime = _T("");
	m_PermissionLevel = 0;
	m_strLastUpdatedDateTime = _T("");
	m_IsAutoLogin = 0;
	m_IsRememberPasswd = 0;
}


CLoginUser::~CLoginUser()
{
}
