#include "stdafx.h"
#include "FaultPara.h"


CFaultPara::CFaultPara()
{
	m_Id = 0;
	m_strCreatedDateTime = _T("");
	m_strLastUpdatedDateTime = _T("");
	m_PLCId = 0;
	m_ProcessModuleId = 0;
	m_DeviceId = 0;
	m_ProductionLineId = 0;
	m_strProductionLineName = _T("");
	m_strProcessName = _T("");
	m_strPlcName = _T("");
	m_strDescription = _T("");
	m_strDeviceName = _T("");
	m_strParaName = _T("");
	m_ParaValue = 0;
	m_strAddressIndex = _T("");
	m_bIsIgnore = false;
	m_nIndexInAlarmVect = -1;
}


CFaultPara::~CFaultPara()
{
}


void  CFaultPara::SetParaValue(int ParaValue)
{
	this->m_ParaValue = ParaValue;
}


//获取BYTE字节的起始位置//
int CFaultPara::GetReadAddrByteIndex()
{
	float temp = _ttof(m_strAddressIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //取十位为字节位置//

	return addIndex;

}


//获取bit位在字节里面的偏移//
int CFaultPara::GetAddrBitOffSet()
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
