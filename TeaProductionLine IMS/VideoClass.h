#pragma once
class CVideoClass
{
public:
	CVideoClass();
	~CVideoClass();
	int m_Id;
	CString m_strVideoName;
	CString m_strProcessModuleName;
	CString m_strProductionLineName;
	CString m_strIPAddr;

	int m_port;

	int m_ProductionLineId;
	int m_ModuleId;
	int m_SortIndex;
};

