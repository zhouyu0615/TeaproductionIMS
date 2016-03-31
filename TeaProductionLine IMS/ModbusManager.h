#pragma once
#include "DataProvider.h"
#include "Thread.h"
#include "MobusClient.h"


class CModbusManager:public Runnable
{
public:
	CModbusManager();
	~CModbusManager();

private:
	CDataProvider* m_pData;
	CThread*  m_pMangerThread;

	int m_nCountClient = 0;

	int m_isFirstStart=TRUE;

public:
	//存储各个ModbusClient//
	std::vector<CMobusClient> m_vModbusClients;

	bool m_bIsCycling = false;                   //轮询启动标记;
	void CycleStart();
	void CycleDisconnect();
	virtual void Run();
	void DispatchWriteRequest();


	void AddParaToModbusClient();

};

