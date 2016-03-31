#include "stdafx.h"
#include "ModbusManager.h"


CModbusManager::CModbusManager()
{
	m_pData = CDataProvider::getInstance();
	m_pMangerThread = NULL;
}


CModbusManager::~CModbusManager()
{
}


void CModbusManager::CycleStart()
{	
	if (m_bIsCycling==TRUE)
	{
		return;
	}

	if (m_isFirstStart==TRUE) //第一次启动//
	{
		m_nCountClient = m_pData->m_vectPlc.size();
		//根据PLC数量添加CTCPClient容器
		int temp = m_pData->m_vectPlc.size();
		CMobusClient* pModClient;
		for (int i = 0; i < m_nCountClient; i++)
		{
			pModClient = new CMobusClient(&m_pData->m_vectPlc[i]);
			pModClient->InitModbusClient();
			m_vModbusClients.push_back(*pModClient);

		}
		AddParaToModbusClient();

		if (m_pMangerThread == NULL)
		{
			m_pMangerThread = new CThread(this);
			m_pMangerThread->Start();
		}


		for (int i = 0; i < m_vModbusClients.size(); i++)
		{
			if (m_vModbusClients[i].m_pWorkThread == NULL)
			{
				m_vModbusClients[i].CreatePollThread();
				m_vModbusClients[i].StartPollThread();
			}
		}

		m_isFirstStart = FALSE;
	}
	else
	{
		m_pMangerThread->Resume();
		for (int i = 0; i < m_vModbusClients.size(); i++)
		{		
		   m_vModbusClients[i].ResumePollThread();
		}

	}	
	if (m_pMangerThread->GetThreadCurrentRunState())
	{
		m_bIsCycling = true;
	}
}
void CModbusManager::CycleDisconnect()
{
	if (m_bIsCycling==TRUE)
	{
		m_pMangerThread->Suspend();
		for (int i = 0; i < m_vModbusClients.size(); i++)
		{
			//if (m_vModbusClients[i].GetConnectedState() == TRUE)
			{
				m_vModbusClients[i].SuspendPollThread();
				m_vModbusClients[i].Close();
			}
		}
	}
	m_bIsCycling = FALSE;
};

void CModbusManager::AddParaToModbusClient()
{
	for (int pIndex = 0; pIndex < m_pData->m_vectProModulePara.size();pIndex++)
	{
		for (int clientIndex = 0; clientIndex < m_vModbusClients.size();clientIndex++)
		{
			if (m_pData->m_vectProModulePara[pIndex].m_PlcId == m_vModbusClients[clientIndex].m_Plc->m_Id)
			{
				m_vModbusClients[clientIndex].m_Plc->AddParaToPlc(m_pData->m_vectProModulePara[pIndex]);
				break;
			}
		}
	}
	for (int pIndex = 0; pIndex < m_pData->m_vectDevicePara.size(); pIndex++)
	{
		for (int clientIndex = 0; clientIndex < m_vModbusClients.size(); clientIndex++)
		{
			if (m_pData->m_vectDevicePara[pIndex].m_PlcId == m_vModbusClients[clientIndex].m_Plc->m_Id)
			{
				m_vModbusClients[clientIndex].m_Plc->AddParaToPlc(m_pData->m_vectDevicePara[pIndex]);
				break;
			}
		}
	}
	for (int pIndex = 0; pIndex < m_pData->m_vectFaultPara.size(); pIndex++)
	{
		for (int clientIndex = 0; clientIndex < m_vModbusClients.size(); clientIndex++)
		{
			if (m_pData->m_vectFaultPara[pIndex].m_PLCId == m_vModbusClients[clientIndex].m_Plc->m_Id)
			{
				m_vModbusClients[clientIndex].m_Plc->AddParaToPlc(m_pData->m_vectFaultPara[pIndex]);
				break;
			}
		}
	}
	for (int pIndex = 0; pIndex < m_pData->m_vectStatePara.size(); pIndex++)
	{
		for (int clientIndex = 0; clientIndex< m_vModbusClients.size(); clientIndex++)
		{
			if (m_pData->m_vectStatePara[pIndex].m_PlcId == m_vModbusClients[clientIndex].m_Plc->m_Id)
			{
				m_vModbusClients[clientIndex].m_Plc->AddParaToPlc(m_pData->m_vectStatePara[pIndex]);
				break;
			}
		}
	}

}


void CModbusManager::Run()
{
	 

	while (TRUE)
	{
		DispatchWriteRequest();
	}
	
}


//把写请求WriteRequest分发到各个ModbusClinet通信线程//
void CModbusManager::DispatchWriteRequest()
{
	CPlcWriteRequest tempRequest;
	while (!(m_pData->m_wRequestQueue.IsEmpty()))
	{
		tempRequest = m_pData->m_wRequestQueue.PopFront();
		for (int i = 0; i < m_vModbusClients.size(); i++)
		{
			if (tempRequest.m_PlcId == m_vModbusClients[i].m_Plc->m_Id)
			{
				if (m_vModbusClients[i].GetConnectedState() == TRUE)
				{
					m_vModbusClients[i].AddWriteRequest(tempRequest);
				}
			}
		}
	}

}
