#pragma once
#include "DataProvider.h"
#include "DevicePara.h"
#include "StatePara.h"
#include "FaultPara.h"
#include "ProcessModulePara.h"

class CParaCheckUtil
{
public:
	CParaCheckUtil();
	~CParaCheckUtil();
	CDataProvider *m_pDataProvider;
	int DeviceParaCheck(CDevicePara &tempDevicePara,int Offset=-1);
	int FaultParaCheck(CFaultPara &tempFaultpara,int Offset=-1);
	int StateParaCheck(CStatePara &tempStatePara,int Offset=-1 ); 
	int ProcessParaCheck(CProcessPara &tempProcessPara, int Offset = -1);

};

