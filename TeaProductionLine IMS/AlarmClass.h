#pragma once
class CAlarmClass
{
public:
	CAlarmClass();
	~CAlarmClass();
	CString m_strAlarmTime;
	CString m_strClearTime;
	CString m_strAlarmName;
	int m_nPlcId;
	int m_nState;  //������״̬��0δ����1����2�Ѵ���//
	CString m_strNote;
	int m_nErrorParaId;
	CString m_strAdressIndex;
	int m_nIndexInErrorParaVect;//�ò���������Դ���ϲ����������±�//
};

