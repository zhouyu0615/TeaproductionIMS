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
	int m_nState;  //警报的状态；0未处理；1挂起；2已处理//
	CString m_strNote;
	int m_nErrorParaId;
	CString m_strAdressIndex;
	int m_nIndexInErrorParaVect;//该参数在数据源故障参数容器的下标//
};

