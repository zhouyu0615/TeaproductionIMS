#pragma once
class CProcessPara
{
public:
	CProcessPara();
	~CProcessPara();


	enum em_VALUE_TYPE {
		VALUETYPE_BOOL, VALUETYPE_BYTE, VALUETYPE_SHORT, VALUETYPE_LONG,VALUETYPE_FLOAT, VALUETYPE_DOUBLE
	};


	enum em_ADDR_TYPE {
		ADDR_TYPE_BIT, ADDR_TYPE_BYTE, ADDR_TYPE_WORD, ADDR_TYPE_DWORD
	};


	int m_Id;
	int m_ProcessModuleId;
	int m_ProductionLineId;
	CString m_strCreatedDateTime;
	CString  m_strLastUpdatedDateTime;
	CString m_strProductionLineName;
	CString m_strProcessModuleName;
	CString m_strPlcName;
	CString m_strDescription;
	float m_ParaValue;
	CString m_strParaName;
	int m_IsConfig;
	int m_IsVisible;
	int m_ValueType;
	CString m_strReadAddrIndex;
	CString m_strAddressType;
	int m_PlcId;
	CString m_strUnit;
	CString m_strWriteAddrIndex;

	int m_IsRecord;

	float m_fSetValue;

	//把地址类型转化为数值形式的枚举的类型//
	int     GetAddrTypeInEmType();

	CString ConvertValTypeToString();
	CString ConvertIsConfigToString();
	CString ConvertIsVisibleToString();

	CString ConvertIsRecordToString();

	CString ConvertParaValueToString();
	CString ConvertSetValueToString();

	int SetParaValue(float fParaValue);

	//设定预设值//
	void SetPreSetValue(float fSetValue);

	int GetReadAddrIndex();

	//获取参数BYTE字节的起始位置//
	int GetReadAddrByteIndex();
	//获取bit位在字节里面的偏移//
	int GetReadBitOffSet();

	//获取预设值的地址//
	int GetWriteAddrByteIndex();
	int GetWriteBitOffSet();
};

