#pragma once
class CUserClass
{
public:
	CUserClass();
	~CUserClass();
	int m_UserId;
	CString m_strCreatedDateTime;
	CString m_strLastUpdatedDateTime;
	CString m_strUserName;
	CString m_strUserPasswd;
	CString m_strUserCode;
	CString m_strNote;
};

