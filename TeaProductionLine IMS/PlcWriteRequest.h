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


	//把m_fSetValue浮点型转化为四个字节的数组，根据Index依次取出
	BYTE GetFloatValueByte(int Index);
	//把Float浮点型按照字取出
	WORD GetFloatValueWord(int Index);


};

