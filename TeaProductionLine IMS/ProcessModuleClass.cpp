#include "stdafx.h"
#include "ProcessModuleClass.h"


CProcessModuleClass::CProcessModuleClass()
{
	m_Id = 0;
	m_strCreateDateTime = _T("");
	m_strLastUpdateTime = _T("");
	//  m_strModuleName = _T("");
	m_ProcessLineId = 0;
	m_strDescription = _T("");
	m_strProcessModuleName = _T("");
	m_strProductionLineName = _T("");
	m_UserId = 0;
	m_SortIndex = 0;
}


CProcessModuleClass::~CProcessModuleClass()
{
}
