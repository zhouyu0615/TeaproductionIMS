#include "stdafx.h"
#include "FormulaClass.h"


CFormulaClass::CFormulaClass()
{
	m_Id = 0;
	m_strFormulaName = _T("");
	m_FormulaId = 0;
	m_ProductionLineId = 0;
	m_strProductionLineName = _T("");
	m_ProcessParaId = 0;
	m_strProcessParaName = _T("");
	m_DefaultValue = 0.0;
	m_strParaValueUnit = _T("");
	m_strNote = _T("");
	m_IsDefaultFormula=0;
	m_IsCurrentFormula=0;
}


CFormulaClass::~CFormulaClass()
{
}
