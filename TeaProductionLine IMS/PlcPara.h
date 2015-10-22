#pragma once
#include <vector>
class CPlcPara
{
public:
	CPlcPara();
	~CPlcPara();
	int m_Id;
	CString m_strAddressType;    //地址类型//
	CString m_strValueType;      //数据类型//
	CString m_strAddressIndex;   //地址值//
	CString m_strValue;     //值//
	CString m_strPlc;       //所属PLC//
	CString m_strLine;      //所属生产线//
	CString m_strModule;    //所属模块名称//
	CString m_strDevice;    //所属设备名称//
	bool m_bIsVisible;    //用户是否可见//
	bool m_bIsConfig;     //是否进入配方//
	CString m_strNote;    //备注说明//
	bool m_bIsReadOnly;    //是否只读//

	int m_ProLineId;    //所属生产线的ID
	int m_ModuleId;
	int m_PlcId;
	int m_DeviceId;
};

