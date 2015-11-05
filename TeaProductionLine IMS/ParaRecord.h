#pragma once
class CParaRecord
{
public:
	CParaRecord();
	~CParaRecord();
	int m_Id; 
	int m_ProParaId;
	CTime m_CreateTime;
	float m_fParaValue;


	CString getCreateTimeString();
};



