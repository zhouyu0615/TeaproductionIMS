#pragma once
class CDevicePara
{
private:
public:
	CDevicePara();
	~CDevicePara();
	int m_Id;
	CString m_strCreatedDateTime;
	CString m_strLastUpdatedDateTime;
	int m_ProductionLineId;
	int m_ProcessModuleId;
	int m_PlcId;
	int m_DeviceId;
	CString m_strProductionLineName;
	CString m_strProcessModuleName;
	CString m_strDeviceName;
	CString m_strPlcName;
	int m_ControlValue;
	int m_StateValue;
	CString m_strControlAddrIndex;
	CString m_strStateAddrIndex;
	CString m_strDescription;
	CString m_strParaName;


public:

	//获取BYTE字节的起始位置//
	int GetControlAddrByteIndex();
	//获取bit位在字节里面的偏移//
	int GetControlAddrBitOffSet();

	int GetStateAddrByteIndex();
	//获取bit位在字节里面的偏移//
	int GetStateAddrBitOffSet();

	void SetControlPara(BOOL ControlParaValue);
	void SetStatePara(BOOL StateParaValue);

};

