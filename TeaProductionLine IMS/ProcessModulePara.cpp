#include "stdafx.h"
#include "ProcessModulePara.h"


CProcessPara::CProcessPara()
{
	m_Id = 0;
	m_ProcessModuleId = 0;
	m_ProductionLineId = 0;
	m_strProductionLineName = _T("");
	m_strProcessModuleName = _T("");
	m_strPlcName = _T("");
	m_strDescription = _T("");
	m_ParaValue = 0;
	m_strParaName = _T("");
	m_IsConfig = 0;
	m_IsVisible = 0;
	m_ValueType = 0;
	m_strReadAddrIndex = _T("");
	m_strAddressType = _T("");
	m_PlcId = 0;
	m_strCreatedDateTime = _T("");
	m_strLastUpdatedDateTime = _T("");
	m_strUnit = _T("");
	m_strWriteAddrIndex = _T("");
	m_fSetValue = -3001;

}


CProcessPara::~CProcessPara()
{
}

CString CProcessPara::ConvertValTypeToString()
{
	switch (m_ValueType)
	{
	case VALUETYPE_BOOL:
		return _T("Bool");
	case VALUETYPE_BYTE:
		return _T("Byte");
	case VALUETYPE_SHORT:
		return _T("Short");
	case VALUETYPE_LONG:
		return _T("Long");
	case VALUETYPE_FLOAT:
		return _T("Float");
	case VALUETYPE_DOUBLE:
		return _T("Double");
	default:
		return _T("Null");
	}

}

CString CProcessPara::ConvertIsConfigToString()
{
	if (m_IsConfig)
	{
		return _T("是");
	}
	else
	{
		return _T("否");
	}
}


CString CProcessPara::ConvertIsVisibleToString()
{
	if (m_IsVisible)
	{
		return _T("是");
	}
	else
	{
		return _T("否");
	}
}


CString CProcessPara::ConvertParaValueToString()
{

	CString strParaValue;
	switch (m_ValueType)
	{
	case CProcessPara::VALUETYPE_BOOL:
		strParaValue.Format(_T("%d"), (int)m_ParaValue);
		break;
	case CProcessPara::VALUETYPE_BYTE:
		strParaValue.Format(_T("%d"), (int)m_ParaValue);
		break;
	case CProcessPara::VALUETYPE_SHORT:
		strParaValue.Format(_T("%hd"), (short)m_ParaValue);
		break;
	case CProcessPara::VALUETYPE_LONG:
		strParaValue.Format(_T("%d"), (int)m_ParaValue);
		break;
	case CProcessPara::VALUETYPE_FLOAT:
		strParaValue.Format(_T("%.2f"), (float)m_ParaValue);
		break;
	case CProcessPara::VALUETYPE_DOUBLE:
		strParaValue.Format(_T("%.2lf"), (double)m_ParaValue);
		break;
	default:
		break;
	}
	return strParaValue;

}

CString CProcessPara::ConvertSetValueToString()
{
	CString strSetValue;

	switch (m_ValueType)
	{
	case CProcessPara::VALUETYPE_BOOL:
		strSetValue.Format(_T("%d"), (int)m_fSetValue);
		break;
	case CProcessPara::VALUETYPE_BYTE:
		strSetValue.Format(_T("%d"), (int)m_fSetValue);
		break;
	case CProcessPara::VALUETYPE_SHORT:
		strSetValue.Format(_T("%hd"), (short)m_fSetValue);
		break;
	case CProcessPara::VALUETYPE_LONG:
		strSetValue.Format(_T("%d"), (int)m_fSetValue);
		break;
	case CProcessPara::VALUETYPE_FLOAT:
		strSetValue.Format(_T("%.2f"), (float)m_fSetValue);
		break;
	case CProcessPara::VALUETYPE_DOUBLE:
		strSetValue.Format(_T("%.2lf"),(double) m_fSetValue);
		break;
	default:
		break;
	}
	return strSetValue;
}

int CProcessPara::SetParaValue(float fParaValue)
{
	//unsigned long *p = (unsigned long *)&m_ParaValue;
	//InterlockedExchange(p, fParaValue);

	m_ParaValue = fParaValue;
	return 0;
}

void CProcessPara::SetPreSetValue(float fSetValue)
{
	this->m_fSetValue = fSetValue;
}

int CProcessPara::GetReadAddrIndex()
{
	return _ttoi(m_strReadAddrIndex);
}


int CProcessPara::GetReadAddrByteIndex()
{
	float temp = _ttof(m_strReadAddrIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //取十位为字节位置

	return addIndex;
}


//获取bit位在字节里面的偏移//
int CProcessPara::GetReadBitOffSet()
{
	float temp = _ttof(m_strReadAddrIndex);
	
	temp = temp * 10;
	int addrIndex = (int)temp;
	int offset = addrIndex % 10; //获取BIT的位置

	if (offset>=8)
	{
		offset = 0;
	}
	return offset;
}




int CProcessPara::GetWriteAddrByteIndex()
{
	float temp = _ttof(m_strWriteAddrIndex);
	temp = temp * 10;
	int addIndex = temp / 10; //取十位为字节位置

	return addIndex;
}
int CProcessPara::GetWriteBitOffSet()
{
	float temp = _ttof(m_strWriteAddrIndex);

	temp = temp * 10;
	int addrIndex = (int)temp;
	int offset = addrIndex % 10; //获取BIT的位置

	if (offset >= 8)
	{
		offset = 0;
	}
	return offset;
}



//把存储的字符串类型转化为枚举类型表示的数值//
int CProcessPara::GetAddrTypeInEmType()
{

	if (m_strAddressType == _T("MX"))
	{
		return ADDR_TYPE_BIT;
	}
	else if (m_strAddressType == _T("MB"))
	{
		return ADDR_TYPE_BYTE;
	}
	else if (m_strAddressType == _T("MW"))
	{
		return ADDR_TYPE_WORD;
	}
	else if (m_strAddressType == _T("MD"))
	{
		return ADDR_TYPE_DWORD;
	}

	return ADDR_TYPE_WORD; //默认为双字节类型//


}