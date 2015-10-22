#include "stdafx.h"
#include "ParaCheckUtil.h"


CParaCheckUtil::CParaCheckUtil()
{
	m_pDataProvider = CDataProvider::getInstance();
}


CParaCheckUtil::~CParaCheckUtil()
{
}



//检查通过返回0.否则返回1
//Offset默认为-1，当进行添加的参数检查，offset为-1，默认对整个已存在的参数容器内进行对比
//若进行修改的参数检查，Offset为修改的参数在容器中的序号
int CParaCheckUtil::DeviceParaCheck(CDevicePara &tempDevicePara, int Offset)
{

	if (tempDevicePara.m_strParaName.IsEmpty())
	{
		AfxMessageBox(_T("参数名为空,不能添加"));
		return 1;
	}
	for (int i = 0; i < m_pDataProvider->m_vectDevicePara.size(); i++)
	{
		if (i==Offset)
		{
			continue;
		}

		if ((tempDevicePara.m_strParaName == m_pDataProvider->m_vectDevicePara[i].m_strParaName) && (tempDevicePara.m_strProductionLineName == m_pDataProvider->m_vectDevicePara[i].m_strProductionLineName) && (tempDevicePara.m_strProcessModuleName == m_pDataProvider->m_vectDevicePara[i].m_strProcessModuleName) && (tempDevicePara.m_strDeviceName == m_pDataProvider->m_vectDevicePara[i].m_strDeviceName))
		{
			AfxMessageBox(_T("同一生产线下同一模块的同一设备下已经存在同名参数，请改变参数名字"));
			return 1;
		}

	}

	return 0;
}


int CParaCheckUtil::FaultParaCheck(CFaultPara &tempFaultpara, int Offset)
{

	if (tempFaultpara.m_strParaName.IsEmpty())
	{
		AfxMessageBox(_T("参数名为空,不能添加"));
		return 1;
	}
	for (int i = 0; i < m_pDataProvider->m_vectFaultPara.size(); i++)
	{
		if (i == Offset)
		{
			continue;
		}
		if ((tempFaultpara.m_strParaName == m_pDataProvider->m_vectFaultPara[i].m_strParaName) && (tempFaultpara.m_strProductionLineName == m_pDataProvider->m_vectFaultPara[i].m_strProductionLineName) && (tempFaultpara.m_strProcessName == m_pDataProvider->m_vectFaultPara[i].m_strProcessName) && (tempFaultpara.m_strDeviceName == m_pDataProvider->m_vectFaultPara[i].m_strDeviceName))
		{
			AfxMessageBox(_T("同一生产线下同一模块的同一设备下已经存在同名参数，请改变参数名字"));
			return 1;
		}

	}

	return 0;
}
int CParaCheckUtil::StateParaCheck(CStatePara &tempStatePara,int Offset)
{
	if (tempStatePara.m_strParaName.IsEmpty())
	{
		AfxMessageBox(_T("参数名为空,不能添加"));
		return 1;
	}
	for (int i = 0; i < m_pDataProvider->m_vectStatePara.size(); i++)
	{
		if (i == Offset)
		{
			continue;
		}
		if ((tempStatePara.m_strParaName == m_pDataProvider->m_vectStatePara[i].m_strParaName) && (tempStatePara.m_strProductionLineName == m_pDataProvider->m_vectStatePara[i].m_strProductionLineName) && (tempStatePara.m_strProcessModuleName == m_pDataProvider->m_vectStatePara[i].m_strProcessModuleName))
		{
			AfxMessageBox(_T("同一生产线下同一模块的同一设备下已经存在同名参数，请改变参数名字"));
			return 1;
		}

	}

	return 0;
}
int CParaCheckUtil::ProcessParaCheck(CProcessPara &tempProcessPara, int Offset)
{
	if (tempProcessPara.m_strParaName.IsEmpty())
	{
		AfxMessageBox(_T("参数名为空,不能添加"));
		return 1;
	}
	for (int i = 0; i < m_pDataProvider->m_vectProModulePara.size(); i++)
	{
		if (i == Offset)
		{
			continue;
		}
		if ((tempProcessPara.m_strParaName == m_pDataProvider->m_vectProModulePara[i].m_strParaName) && (tempProcessPara.m_strProductionLineName == m_pDataProvider->m_vectProModulePara[i].m_strProductionLineName) && (tempProcessPara.m_strProcessModuleName == m_pDataProvider->m_vectProModulePara[i].m_strProcessModuleName))
		{
			AfxMessageBox(_T("同一生产线下同一模块的同一设备下已经存在同名参数，请改变参数名字"));
			return 1;
		}

	}
	return 0;
}