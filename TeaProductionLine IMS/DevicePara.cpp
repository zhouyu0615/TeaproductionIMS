#include "stdafx.h"
#include "DevicePara.h"


CDevicePara::CDevicePara()
{
	m_Id = 0;
	m_strCreatedDateTime = _T("");
	m_strLastUpdatedDateTime = _T("");
	m_ProductionLineId = 0;
	m_ProcessModuleId = 0;
	m_PlcId = 0;
	m_DeviceId = 0;
	m_strProductionLineName = _T("");
	m_strProcessModuleName = _T("");
	m_strDeviceName = _T("");
	m_strPlcName = _T("");
	m_ControlValue = 0;
	m_StateValue = 0;
	m_strControlAddrIndex = _T("");
	m_strStateAddrIndex = _T("");
	m_strDescription = _T("");
	m_strParaName = _T("");
}


CDevicePara::~CDevicePara()
{
}


int CDevicePara::GetControlAddrByteIndex()
{
	float temp = _ttof(m_strControlAddrIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //取十位为字节位置

	return addIndex;
}


//获取bit位在字节里面的偏移//
int CDevicePara::GetControlAddrBitOffSet()
{
	float temp = _ttof(m_strControlAddrIndex);

	temp = temp * 10;
	int addrIndex = (int)temp;
	int offset = addrIndex % 10; //获取BIT的位置

	if (offset >= 8)
	{
		offset = 0;
	}
	return offset;
}




int CDevicePara::GetStateAddrByteIndex()
{
	float temp = _ttof(m_strStateAddrIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //取十位为字节位置

	return addIndex;

}



//获取bit位在字节里面的偏移//
int CDevicePara::GetStateAddrBitOffSet()
{
	float temp = _ttof(m_strStateAddrIndex);

	temp = temp * 10;
	int addrIndex = (int)temp;
	int offset = addrIndex % 10; //获取BIT的位置

	if (offset >= 8)
	{
		offset = 0;
	}
	return offset;

}

void CDevicePara::SetControlPara(BOOL ControlParaValue)
{
	
	this->m_ControlValue = ControlParaValue;

}
void CDevicePara::SetStatePara(BOOL StateParaValue)
{
	this->m_StateValue = StateParaValue;
}