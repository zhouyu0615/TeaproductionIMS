#pragma once
class CFormulaClass
{
public:
	CFormulaClass();
	~CFormulaClass();

	long	m_Id;
	CString	m_strFormulaName;
	long	m_FormulaId;
	long	m_ProductionLineId;
	CString	m_strProductionLineName;
	long	m_ProcessParaId;
	CString	m_strProcessParaName;
	float	m_DefaultValue;
	CString	m_strParaValueUnit;
	CString	m_strNote;
	long	m_IsDefaultFormula;
	long	m_IsCurrentFormula;


};

