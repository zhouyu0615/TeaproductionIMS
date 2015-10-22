#pragma once
class CFaultPara
{
public:
	CFaultPara();
	~CFaultPara();
	int m_Id;
	CString m_strCreatedDateTime;
	CString m_strLastUpdatedDateTime;
	int m_PLCId;
	int m_ProcessModuleId;
	int m_DeviceId;
	int m_ProductionLineId;
	CString m_strProductionLineName;
	CString m_strProcessName;
	CString m_strPlcName;
	CString m_strDescription;
	CString m_strDeviceName;
	CString m_strParaName;
	int m_ParaValue;
	CString m_strAddressIndex;


public:
	void  SetParaValue(int ParaValue);
	bool m_bIsIgnore;         //是否暂时忽略该警报//
	int m_nIndexInAlarmVect;  //在报警列表容器的下标，如果为-1则表示尚未进入报警列表//
	//获取BYTE字节的起始位置//
	int GetReadAddrByteIndex();
	//获取bit位在字节里面的偏移//
	int GetAddrBitOffSet();
};

