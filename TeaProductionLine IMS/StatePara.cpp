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


//��ȡBYTE�ֽڵ���ʼλ��//
int CStatePara::GetReadAddrByteIndex()
{
	float temp = _ttof(m_strAddressIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //ȡʮλΪ�ֽ�λ��//

	return addIndex;

}


//��ȡbitλ���ֽ������ƫ��//
int CStatePara::GetAddrBitOffSet()
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
