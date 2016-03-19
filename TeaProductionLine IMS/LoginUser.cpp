#include "stdafx.h"
#include "LoginUser.h"

#include <afxwin.h>  
#include <tchar.h>  
#include <afx.h>

IMPLEMENT_SERIAL(CLoginUser, CObject, VERSIONABLE_SCHEMA | 2);


CLoginUser::CLoginUser(void)
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

CLoginUser::CLoginUser(const CLoginUser& otherUser)
{

	this->m_Id = otherUser.m_Id;
	this->m_strLoginName = otherUser.m_strLoginName;
	this->m_strLoginPassWd = otherUser.m_strLoginPassWd;
	this->m_strCreatedDateTime = otherUser.m_strCreatedDateTime;
	this->m_strLastLoginInTime = otherUser.m_strLastLoginInTime;
	this->m_PermissionLevel = otherUser.m_PermissionLevel;
	this->m_strLastUpdatedDateTime = otherUser.m_strLastUpdatedDateTime;
	this->m_IsAutoLogin = otherUser.m_IsAutoLogin;
	this->m_IsRememberPasswd = otherUser.m_IsRememberPasswd;
	this->m_LastLoginInTime = otherUser.m_LastLoginInTime;
	this->m_LastUpdatedDateTime = otherUser.m_LastUpdatedDateTime;
}

CLoginUser& CLoginUser::operator = (const CLoginUser &otherUser)
{
	if (&otherUser==this)
	{
		return *this;
	}

	this->m_Id = otherUser.m_Id;
	this->m_strLoginName = otherUser.m_strLoginName;
	this->m_strLoginPassWd = otherUser.m_strLoginPassWd;
	this->m_strCreatedDateTime = otherUser.m_strCreatedDateTime;
	this->m_strLastLoginInTime = otherUser.m_strLastLoginInTime;
	this->m_PermissionLevel = otherUser.m_PermissionLevel;
	this->m_strLastUpdatedDateTime = otherUser.m_strLastUpdatedDateTime;
	this->m_IsAutoLogin = otherUser.m_IsAutoLogin;
	this->m_IsRememberPasswd = otherUser.m_IsRememberPasswd;
	this->m_LastLoginInTime = otherUser.m_LastLoginInTime;
	this->m_LastUpdatedDateTime = otherUser.m_LastUpdatedDateTime;

	return *this;
}

void CLoginUser::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	//关键代码  
	if (ar.IsStoring())
	{
		//序列化  
		   ar << this->m_Id 
			<< this->m_strLoginName 
			<< this->m_strLoginPassWd
			<< this->m_strCreatedDateTime
			<< this->m_strLastLoginInTime
			<< this->m_PermissionLevel
			<< this->m_strLastUpdatedDateTime
			<< this->m_IsAutoLogin
			<< this->m_IsRememberPasswd;
	}
	else
	{
		//反序列化  
		ar >> this->m_Id
			>> this->m_strLoginName
			>> this->m_strLoginPassWd
			>> this->m_strCreatedDateTime
			>> this->m_strLastLoginInTime
			>> this->m_PermissionLevel
			>> this->m_strLastUpdatedDateTime
			>> this->m_IsAutoLogin
			>> this->m_IsRememberPasswd;
	 }
};



