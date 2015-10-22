#pragma once
#include <vector>

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
	int m_WriteStartAddr; //�Ѿ�����
	int m_WriteLength;		//�Ѿ�����
	int m_SortIndex;



	typedef union {
		struct {
			unsigned char low_byte;
			unsigned char high_byte;
		}reg_Byte;
		short  regWord;
	}Register;


	std::vector<Register> m_PlcReadMemory; //PLC�Ķ���洢��
	//std::vector<BYTE> m_PlcWriteMemory;  //PLC��д��洢��
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

	//��ȡ�ɶ��ڴ��������ֵ//
	float GetReadDWord(int ReadDWordAddr); //��ȡ˫�ֵ�ֵ��˫��ֻΪ��������//

	WORD  GetReadMemoryWord(int ReadWordAddr);
	BYTE  GetReadMemoryByte(int ReadByteAddr);
	BOOL GetReadMemoryBit(int ReadByteAddr, int bitOffset);

	

	void SetConectedState(BOOL IsConnected);
	BOOL GetConectedState();

	CString GetConectedString();

};

