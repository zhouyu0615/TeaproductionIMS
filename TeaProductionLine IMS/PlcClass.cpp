#include "stdafx.h"
#include "PlcClass.h"
#include <winnt.h>



CPlcClass::CPlcClass()
{
	m_Id = 0;
	m_strPlcName = _T("");
	m_strDescription = _T("");
	m_strProductionLineName = _T("");
	m_strIPAddr = _T("");
	m_UserId = 0;
	m_ReadStartAddr = 0;
	m_ReadLength = 0;
	m_WriteStartAddr = 0;
	m_WriteLength = 0;
	m_SortIndex = 0;
	m_bIsConnected = 0;
}


CPlcClass::~CPlcClass()
{
}


void CPlcClass::SetConectedState(int IsConnected)
{
	unsigned long *p = (unsigned long *)&m_bIsConnected;
	InterlockedExchange(p, IsConnected);

}

CString CPlcClass::GetConectedString()
{
	if (m_bIsConnected)
	{
		return _T("������");
	}
	else
	{
		return _T("�Ͽ�");
	}

}


BOOL CPlcClass::GetConectedState()
{
	return (BOOL)m_bIsConnected;
}


int CPlcClass::GetPlcReadMemorySize()
{
	return m_PlcReadMemory.size();
}

//����PLC�������������ַ��Χ//
int CPlcClass::GetMaxReadAddr()
{
	return m_ReadStartAddr + m_ReadLength;
}

//����PLC��С����������ĵ�ַ��Χ//
int CPlcClass::GetMinReadAddr()
{
	return m_ReadStartAddr;
}




float CPlcClass::GetReadDWord(int ReadDWordAddr)
{
	ReadDWordAddr = ReadDWordAddr - m_ReadStartAddr; //��ȥ��ʼ��ַ�����ƫ�Ƶ�ַ//

	ReadDWordAddr = ReadDWordAddr * 2;
	FLOAT uTemp;
	uTemp.float_byte.low_byte = m_PlcReadMemory[ReadDWordAddr].reg_Byte.low_byte;
	uTemp.float_byte.mlow_byte = m_PlcReadMemory[ReadDWordAddr].reg_Byte.high_byte;
	uTemp.float_byte.mhigh_byte = m_PlcReadMemory[ReadDWordAddr+1].reg_Byte.low_byte;
	uTemp.float_byte.high_byte = m_PlcReadMemory[ReadDWordAddr+1].reg_Byte.high_byte;

	return uTemp.fvalue;
}


WORD   CPlcClass::GetReadMemoryWord(int ReadWordAddr)
{
	ReadWordAddr = ReadWordAddr - m_ReadStartAddr;
	int length = m_PlcReadMemory.size();
	ASSERT(ReadWordAddr < length);
	if (ReadWordAddr >= length-1)
	{
		return 0;
	}
	WORD temp;
	temp = m_PlcReadMemory[ReadWordAddr].regWord;

	return temp;
}

BYTE  CPlcClass::GetReadMemoryByte(int ReadByteAddr)
{
	ReadByteAddr = ReadByteAddr - m_ReadStartAddr;
	int length = m_PlcReadMemory.size();
	ASSERT(ReadByteAddr < length);
	if (ReadByteAddr>=length)
	{
		return 0;
	}
	int RegAddr = (int)ReadByteAddr / 2;

	if (ReadByteAddr%2==0)
	{
		return m_PlcReadMemory[RegAddr].reg_Byte.low_byte;
	}
	else
	{
		return m_PlcReadMemory[RegAddr].reg_Byte.high_byte;
	}
	

}
BOOL CPlcClass::GetReadMemoryBit(int ReadByteAddr, int bitOffset)
{
	ReadByteAddr = ReadByteAddr - m_ReadStartAddr;
	int length = m_PlcReadMemory.size();
	ASSERT(ReadByteAddr < length);
	ASSERT(bitOffset < 8);


	int RegAddr = (int)ReadByteAddr / 2;

	if (ReadByteAddr % 2 == 0)
	{
		return (m_PlcReadMemory[RegAddr].reg_Byte.low_byte >> bitOffset) & 0x01;
	}
	else
	{
		return (m_PlcReadMemory[RegAddr].reg_Byte.high_byte >> bitOffset) & 0x01;
	}

}


void CPlcClass::InitPlcMemory()
{
	Register temp;
	for (int i = 0; i < m_ReadLength+10;i++)
	{		
		temp.regWord = 0;
		m_PlcReadMemory.push_back(temp);
	}
}





void CPlcClass::DispatchProPara()
{
	int  ParaValue = 0;
	int  AddrByte = 0;
	int  BitOffset = 0;
	//��PLC����Ĳ���д�뵽�������ղ���
	for (int i = 0; i < m_vProPara.size(); i++)
	{
		if (m_vProPara[i]->m_PlcId == m_Id
			&& (GetPlcReadMemorySize() >  m_vProPara[i]->GetReadAddrIndex()))
		{

			switch (m_vProPara[i]->GetAddrTypeInEmType())
			{
			case CProcessPara::ADDR_TYPE_BIT:
				AddrByte = m_vProPara[i]->GetReadAddrByteIndex();
				BitOffset = m_vProPara[i]->GetReadBitOffSet();
				ParaValue = GetReadMemoryBit(AddrByte, BitOffset);
				m_vProPara[i]->SetParaValue(ParaValue);

				//����PLC������趨ֵ��д�뵽Ԥ��ֵ��
				AddrByte = m_vProPara[i]->GetWriteAddrByteIndex();
				BitOffset = m_vProPara[i]->GetWriteBitOffSet();
				ParaValue =GetReadMemoryBit(AddrByte, BitOffset);
				m_vProPara[i]->SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_BYTE:
				AddrByte = m_vProPara[i]->GetReadAddrByteIndex();
				ParaValue = GetReadMemoryByte(AddrByte);
				m_vProPara[i]->SetParaValue(ParaValue);

				AddrByte = m_vProPara[i]->GetWriteAddrByteIndex();
				ParaValue = GetReadMemoryByte(AddrByte);
				m_vProPara[i]->SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_WORD:
				AddrByte = m_vProPara[i]->GetReadAddrByteIndex();
				ParaValue = GetReadMemoryWord(AddrByte);
				m_vProPara[i]->SetParaValue(ParaValue);

				AddrByte = m_vProPara[i]->GetWriteAddrByteIndex();
				ParaValue = GetReadMemoryWord(AddrByte);
				m_vProPara[i]->SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_DWORD:  //˫��ֻ�и�����һ����������
				AddrByte = m_vProPara[i]->GetReadAddrByteIndex();
				float	fParaValue = GetReadDWord(AddrByte);
				m_vProPara[i]->SetParaValue(fParaValue);

				AddrByte = m_vProPara[i]->GetWriteAddrByteIndex();
				fParaValue = GetReadDWord(AddrByte);
				m_vProPara[i]->SetPreSetValue(fParaValue);
			}
		}
	}
}


void CPlcClass::DispatchDevicePara()
{
	int ParaValue = 0;
	for (int i = 0; i < m_vDevPara.size(); i++)
	{
		if (m_vDevPara[i]->m_PlcId == m_Id && (GetPlcReadMemorySize() > m_vDevPara[i]->GetControlAddrByteIndex()))
		{
			int ByteAddr = m_vDevPara[i]->GetControlAddrByteIndex();
			int BitOffset = m_vDevPara[i]->GetControlAddrBitOffSet();
			ParaValue = GetReadMemoryBit(ByteAddr, BitOffset);
			m_vDevPara[i]->SetControlPara(ParaValue);


			ByteAddr = m_vDevPara[i]->GetStateAddrByteIndex();
			BitOffset = m_vDevPara[i]->GetStateAddrBitOffSet();
			ParaValue = GetReadMemoryBit(ByteAddr, BitOffset);
			m_vDevPara[i]->SetStatePara(ParaValue);
		}
	}


}
void CPlcClass::DispatchFaultPara()
{

	int ParaValue;
	for (int i = 0; i < m_vFaultPara.size(); i++)
	{

		if (m_vFaultPara[i]->m_PLCId == m_Id && (GetPlcReadMemorySize() > m_vFaultPara[i]->GetReadAddrByteIndex()))
		{
			int ByteAddr = m_vFaultPara[i]->GetReadAddrByteIndex();
			int BitOffset = m_vFaultPara[i]->GetAddrBitOffSet();
			ParaValue = GetReadMemoryBit(ByteAddr, BitOffset);
			m_vFaultPara[i]->SetParaValue(ParaValue);
		}
	}


}
void CPlcClass::DispatchStatePara()
{
	int ParaValue;
	for (int i = 0; i < m_vStatePara.size(); i++)
	{
		if (m_vStatePara[i]->m_PlcId == m_Id
			&& (GetPlcReadMemorySize() > m_vStatePara[i]->GetReadAddrByteIndex()))
		{
			int ByteAddr = m_vStatePara[i]->GetReadAddrByteIndex();
			int BitOffset = m_vStatePara[i]->GetAddrBitOffSet();
			ParaValue = GetReadMemoryBit(ByteAddr, BitOffset);
			m_vStatePara[i]->SetParaValue(ParaValue);
		}
	}
}


void CPlcClass::DispatchPara()
{
	DispatchProPara();
	DispatchDevicePara();
	DispatchFaultPara();
	DispatchStatePara();
}



void CPlcClass::AddParaToPlc(CProcessPara& proPara)
{
	m_vProPara.push_back(&proPara);
}

void CPlcClass::AddParaToPlc(CDevicePara& devPara)
{
	m_vDevPara.push_back(&devPara);
}
void CPlcClass::AddParaToPlc(CFaultPara& faultPara)
{
	m_vFaultPara.push_back(&faultPara);
}
void CPlcClass::AddParaToPlc(CStatePara& statePara)
{
	m_vStatePara.push_back(&statePara);
}

void CPlcClass::RemovePara(CProcessPara& proPara)
{
	std::vector<CProcessPara*>::iterator pI;
	for (pI = m_vProPara.begin(); pI != m_vProPara.end();++pI)
	{
		if ((*pI)->m_Id==proPara.m_Id)
		{
			pI=m_vProPara.erase(pI);
			return;
		}
	}
}
void CPlcClass::RemovePara(CDevicePara& devPara)
{
	std::vector<CDevicePara*>::iterator pI;
	for (pI = m_vDevPara.begin(); pI != m_vDevPara.end(); ++pI)
	{
		if ((*pI)->m_Id == devPara.m_Id)
		{
			pI = m_vDevPara.erase(pI);
			return;
		}
	}
}
void CPlcClass::RemovePara(CFaultPara& faultPara)
{
	std::vector<CFaultPara*>::iterator pI;
	for (pI = m_vFaultPara.begin(); pI != m_vFaultPara.end(); ++pI)
	{
		if ((*pI)->m_Id == faultPara.m_Id)
		{
			pI = m_vFaultPara.erase(pI);
			return;
		}
	}
}
void CPlcClass::RemovePara(CStatePara& statePara)
{
	std::vector<CStatePara*>::iterator pI;
	for (pI = m_vStatePara.begin(); pI != m_vStatePara.end(); ++pI)
	{
		if ((*pI)->m_Id == statePara.m_Id)
		{
			pI = m_vStatePara.erase(pI);
			return;
		}
	}
}

//������д洢�Ĳ���������//
void CPlcClass::ClearAllParas()
{
	m_vProPara.clear();
	m_vDevPara.clear();
	m_vFaultPara.clear();
	m_vStatePara.clear();
}


