#include "stdafx.h"
#include "UserClass.h"




CUserClass::CUserClass()
{
	m_UserId = 0;
	m_strCreatedDateTime = _T("");
	m_strLastUpdatedDateTime = _T("");
	m_strUserName = _T("");
	m_strUserPasswd = _T("");
	m_strUserCode = _T("");
	m_strNote = _T("");
}


CUserClass::~CUserClass()
{
}
