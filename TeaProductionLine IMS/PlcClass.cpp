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
		return _T("已连接");
	}
	else
	{
		return _T("断开");
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

//返回PLC最大允许的输入地址范围//
int CPlcClass::GetMaxReadAddr()
{
	return m_ReadStartAddr + m_ReadLength;
}

//返回PLC最小的允许输入的地址范围//
int CPlcClass::GetMinReadAddr()
{
	return m_ReadStartAddr;
}




float CPlcClass::GetReadDWord(int ReadDWordAddr)
{
	ReadDWordAddr = ReadDWordAddr - m_ReadStartAddr; //减去起始地址，获得偏移地址//

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




void CPlcClass::DispatchProPara(CPlcClass &Plc)
{
	int  ParaValue = 0;
	int  AddrByte = 0;
	int  BitOffset = 0;
	//把PLC里面的参数写入到各条工艺参数
	for (int i = 0; i < Plc.m_vProPara.size(); i++)
	{
		if (Plc.m_vProPara[i].m_PlcId == Plc.m_Id
			&& (Plc.GetPlcReadMemorySize() >  Plc.m_vProPara[i].GetReadAddrIndex()))
		{

			switch (Plc.m_vProPara[i].GetAddrTypeInEmType())
			{
			case CProcessPara::ADDR_TYPE_BIT:
				AddrByte = Plc.m_vProPara[i].GetReadAddrByteIndex();
				BitOffset = Plc.m_vProPara[i].GetReadBitOffSet();
				ParaValue = Plc.GetReadMemoryBit(AddrByte, BitOffset);
				Plc.m_vProPara[i].SetParaValue(ParaValue);

				//读出PLC里面的设定值，写入到预设值中
				AddrByte = Plc.m_vProPara[i].GetWriteAddrByteIndex();
				BitOffset = Plc.m_vProPara[i].GetWriteBitOffSet();
				ParaValue = Plc.GetReadMemoryBit(AddrByte, BitOffset);
				Plc.m_vProPara[i].SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_BYTE:
				AddrByte = Plc.m_vProPara[i].GetReadAddrByteIndex();
				ParaValue = Plc.GetReadMemoryByte(AddrByte);
				Plc.m_vProPara[i].SetParaValue(ParaValue);

				AddrByte = Plc.m_vProPara[i].GetWriteAddrByteIndex();
				ParaValue = Plc.GetReadMemoryByte(AddrByte);
				Plc.m_vProPara[i].SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_WORD:
				AddrByte = Plc.m_vProPara[i].GetReadAddrByteIndex();
				ParaValue = Plc.GetReadMemoryWord(AddrByte);
				Plc.m_vProPara[i].SetParaValue(ParaValue);

				AddrByte = Plc.m_vProPara[i].GetWriteAddrByteIndex();
				ParaValue = Plc.GetReadMemoryWord(AddrByte);
				Plc.m_vProPara[i].SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_DWORD:  //双字只有浮点型一种数据类型
				AddrByte = Plc.m_vProPara[i].GetReadAddrByteIndex();
				float	fParaValue = Plc.GetReadDWord(AddrByte);
				Plc.m_vProPara[i].SetParaValue(fParaValue);

				AddrByte = Plc.m_vProPara[i].GetWriteAddrByteIndex();
				fParaValue = Plc.GetReadDWord(AddrByte);
				Plc.m_vProPara[i].SetPreSetValue(fParaValue);
			}
		}
	}
}


void CPlcClass::DispatchDevicePara(CPlcClass &plc)
{
	int ParaValue = 0;
	for (int i = 0; i < plc.m_vDevPara.size(); i++)
	{
		if (plc.m_vDevPara[i].m_PlcId == plc.m_Id && (plc.GetPlcReadMemorySize() > plc.m_vDevPara[i].GetControlAddrByteIndex()))
		{
			int ByteAddr = plc.m_vDevPara[i].GetControlAddrByteIndex();
			int BitOffset = plc.m_vDevPara[i].GetControlAddrBitOffSet();
			ParaValue = plc.GetReadMemoryBit(ByteAddr, BitOffset);
			plc.m_vDevPara[i].SetControlPara(ParaValue);


			ByteAddr = plc.m_vDevPara[i].GetStateAddrByteIndex();
			BitOffset = plc.m_vDevPara[i].GetStateAddrBitOffSet();
			ParaValue = plc.GetReadMemoryBit(ByteAddr, BitOffset);
			plc.m_vDevPara[i].SetStatePara(ParaValue);
		}
	}


}
void CPlcClass::DispatchFaultPara(CPlcClass &plc)
{




}
void CPlcClass::DispatchStatePara(CPlcClass &plc)
{

}


