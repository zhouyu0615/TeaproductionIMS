#include "stdafx.h"
#include "AlarmClass.h"


CAlarmClass::CAlarmClass()
{
	m_strAlarmTime = _T("");
	m_strClearTime = _T("");
	m_strAlarmName = _T("");
	m_nPlcId=0;
	m_nState=0;  //警报的状态；0未处理；1挂起；2已处理//
	m_strNote = _T("");
	m_nErrorParaId=0;
	m_strAdressIndex = _T("");
	m_nIndexInErrorParaVect = -1;
}


CAlarmClass::~CAlarmClass()
{
}
