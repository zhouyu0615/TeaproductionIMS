#pragma once



class CPlcWriteRequest
{
public:
	CPlcWriteRequest();
	~CPlcWriteRequest();

public:
	enum em_ADDR_TYPE {
		ADDR_TYPE_BIT, ADDR_TYPE_BYTE, ADDR_TYPE_WORD, ADDR_TYPE_DWORD
	};



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
		float  value;
	}FLOAT;




	int m_PlcId;
	CString m_strIPAddr;
	int m_WriteByteAddrIndex;
	int m_ByteOffset;
	int m_AddrType;
	int  m_SetValue;
	float m_fSetValue;


	//��m_fSetValue������ת��Ϊ�ĸ��ֽڵ����飬����Index����ȡ��
	BYTE GetFloatValueByte(int Index);
	//��Float�����Ͱ�����ȡ��
	WORD GetFloatValueWord(int Index);


};

