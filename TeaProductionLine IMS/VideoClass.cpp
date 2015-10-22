#include "stdafx.h"
#include "VideoClass.h"


CVideoClass::CVideoClass()
{
	m_Id = 0;
	m_strVideoName = _T("");
	m_strProcessModuleName = _T("");
	m_strProductionLineName = _T("");
	m_strIPAddr = _T("");
	m_port = 0;
	m_SortIndex = 0;
}


CVideoClass::~CVideoClass()
{
}
