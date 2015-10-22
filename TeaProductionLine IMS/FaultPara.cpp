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


//��ȡBYTE�ֽڵ���ʼλ��//
int CFaultPara::GetReadAddrByteIndex()
{
	float temp = _ttof(m_strAddressIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //ȡʮλΪ�ֽ�λ��//

	return addIndex;

}


//��ȡbitλ���ֽ������ƫ��//
int CFaultPara::GetAddrBitOffSet()
{
	float temp = _ttof(m_strAddressIndex);

	temp = temp * 10;
	int addrIndex = (int)temp;
	int offset = addrIndex % 10; //��ȡBIT��λ��//

	if (offset >= 8)
	{
		offset = 0;
	}
	return offset;
}
