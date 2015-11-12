#include "stdafx.h"
#include "ParaRecord.h"


CParaRecord::CParaRecord()
{
	m_Id = 0;
	m_ProParaId = 0;
	m_fParaValue = 0;
}


CParaRecord::~CParaRecord()
{
}

CString CParaRecord::getCreateTimeString()
{
	//return m_CreateTime.Format("%Y-%m-%d %H:%M:%S");
	return m_CreateTime.Format("%H:%M:%S");
}
