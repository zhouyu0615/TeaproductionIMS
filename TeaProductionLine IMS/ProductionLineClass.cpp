#include "stdafx.h"
#include "ProductionLineClass.h"


CProductionLineClass::CProductionLineClass()
{
	m_Id = 0;
	m_strCreateDateTime = _T("");
	m_strLastUpdateTime = _T("");
	m_strLineName = _T("");
	m_UserId = 0;
	m_strCapacity = _T("");
	m_strDescription = _T("");
	m_SortIndex = 0;
}


CProductionLineClass::~CProductionLineClass()
{
}
