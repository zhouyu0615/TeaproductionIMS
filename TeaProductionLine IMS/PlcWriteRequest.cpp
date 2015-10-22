#include "stdafx.h"
#include "PlcWriteRequest.h"



CPlcWriteRequest::CPlcWriteRequest()
{
	m_PlcId = 0;
	m_strIPAddr = _T("");
	m_WriteByteAddrIndex = 0;
	m_ByteOffset = 0;
	m_AddrType = 0;
	m_SetValue = 0;



}


CPlcWriteRequest::~CPlcWriteRequest()
{
}

//��m_fSetValue������ת��Ϊ�ĸ��ֽڵ����飬����Index����ȡ��
//Index ������ΧΪ0��1��2��3 .���ֽ���ǰ��
BYTE CPlcWriteRequest::GetFloatValueByte(int Index)
{
	FLOAT temp;
	temp.value = m_fSetValue;

	switch (Index)
	{
	case 0:
		return temp.float_byte.low_byte;
		break;
	case 1:
		return temp.float_byte.mlow_byte;
		break;
	case 2:
		return temp.float_byte.mhigh_byte;
		break;
	case 3:
		return temp.float_byte.high_byte;
		break;
	default:
		break;
	}
	return 0;

}

//��m_fSetValue������ת��Ϊ�ĸ��ֽڵ����飬����Index����ȡ��
//Index ������ΧΪ0��1��2��3��
WORD CPlcWriteRequest::GetFloatValueWord(int Index)
{
	FLOAT temp;
	temp.value = m_fSetValue;

	switch (Index)
	{
	case 0:
		return temp.float_word.low_word;
		break;
	case 1:
		return temp.float_word.high_word;
		break;
	default:
		break;
	}
	return 0;

}


