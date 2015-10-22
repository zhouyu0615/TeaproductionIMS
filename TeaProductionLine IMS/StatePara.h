#pragma once
class CStatePara
{
public:
	CStatePara();
	~CStatePara();
	int m_Id;
	int m_ProductionLineId;
	int m_ProcessModuleId;
	CString m_strProductionLineName;
	CString m_strProcessModuleName;
	int m_PlcId;
	CString m_strParaName;
	CString m_strAddressIndex;
	int m_ParaValue;
	CString m_strDescription;
	CString m_strPlcName;
	CString m_strCreatedDateTime;
	CString m_strLastUpdateDateTime;


public:
	void  SetParaValue(int ParaValue);


	//获取BYTE字节的起始位置//
	int GetReadAddrByteIndex();
	//获取bit位在字节里面的偏移//
	int GetAddrBitOffSet();
};

