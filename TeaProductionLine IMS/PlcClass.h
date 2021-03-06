#pragma once
#include <vector>
#include "ProcessModulePara.h"
#include "DevicePara.h"
#include "FaultPara.h"
#include "StatePara.h"

class CPlcClass
{

private:
	
public:
	CPlcClass();
	~CPlcClass();
	int m_Id;
	CString m_strPlcName;
	CString m_strDescription;
	CString m_strProductionLineName;
	int m_ProductionLineId;

	int m_bIsConnected;
    //int m_Port;
	CString m_strIPAddr;
	int m_UserId;
	int m_ReadStartAddr;
	int m_ReadLength;
	int m_WriteStartAddr; //已经不用
	int m_WriteLength;		//已经不用
	int m_SortIndex;



	typedef union {
		struct {
			unsigned char low_byte;
			unsigned char high_byte;
		}reg_Byte;
		short  regWord;
	}Register;


	std::vector<Register> m_PlcReadMemory; //PLC的读入存储区
	//std::vector<BYTE> m_PlcWriteMemory;  //PLC的写入存储区
	//


	typedef union {
		struct {
			unsigned char low_byte;
			unsigned char mlow_byte;
			unsigned char mhigh_byte;
			unsigned char high_byte;
		}float_byte;
		struct {
			unsigned int low_word;
			unsigned int high_word;
		}float_word;
		float  fvalue;
	}FLOAT;



	void InitPlcMemory();

	int GetPlcReadMemorySize();
	//int GetPlcWriteMemorySize();   

	int GetMaxReadAddr();
	int GetMinReadAddr();

	//获取可读内存区域的数值//
	float GetReadDWord(int ReadDWordAddr); //获取双字的值，双字只为浮点类型//

	WORD  GetReadMemoryWord(int ReadWordAddr);
	BYTE  GetReadMemoryByte(int ReadByteAddr);
	BOOL GetReadMemoryBit(int ReadByteAddr, int bitOffset);

	

	void SetConectedState(BOOL IsConnected);
	BOOL GetConectedState();

	CString GetConectedString();



	enum enumPARATYPE{PROCESSPARA,DEVICEPARA,FAULTPARA,STATEPARA};

	std::vector<CProcessPara*> m_vProPara;
	std::vector<CDevicePara*>  m_vDevPara;
	std::vector<CFaultPara*>  m_vFaultPara;
	std::vector<CStatePara*>  m_vStatePara;


	void DispatchPara();

	void DispatchProPara();
	void DispatchDevicePara();
	void DispatchFaultPara();
	void DispatchStatePara();

	void AddParaToPlc(CProcessPara& proPara);
	void AddParaToPlc(CDevicePara& devPara);
	void AddParaToPlc(CFaultPara& faultPara);
	void AddParaToPlc(CStatePara& statePara);

	void RemovePara(CProcessPara& proPara);
	void RemovePara(CDevicePara& devPara);
	void RemovePara(CFaultPara& faultPara);
	void RemovePara(CStatePara& statePara);

	void ClearAllParas(); //清空所有存储的参数引用//


};

