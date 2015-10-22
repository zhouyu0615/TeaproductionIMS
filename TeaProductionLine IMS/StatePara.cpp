#include "stdafx.h"
#include "StatePara.h"


CStatePara::CStatePara()
{
	m_Id = 0;
	m_ProductionLineId = 0;
	m_ProcessModuleId = 0;
	m_strProductionLineName = _T("");
	m_strProcessModuleName = _T("");
	m_PlcId = 0;
	m_strParaName = _T("");
	m_strAddressIndex = _T("");
	m_ParaValue = 0;
	m_strDescription = _T("");
	m_strPlcName = _T("");
	m_strCreatedDateTime = _T("");
	m_strLastUpdateDateTime = _T("");
}


CStatePara::~CStatePara()
{
}



void  CStatePara::SetParaValue(int ParaValue)
{
	this->m_ParaValue = ParaValue;
}


//获取BYTE字节的起始位置//
int CStatePara::GetReadAddrByteIndex()
{
	float temp = _ttof(m_strAddressIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //取十位为字节位置//

	return addIndex;

}


//获取bit位在字节里面的偏移//
int CStatePara::GetAddrBitOffSet()
{
	float temp = _ttof(m_strAddressIndex);

	temp = temp * 10;
	int addrIndex = (int)temp;
	int offset = addrIndex % 10; //获取BIT的位置//

	if (offset >= 8)
	{
		offset = 0;
	}
	return offset;
}
