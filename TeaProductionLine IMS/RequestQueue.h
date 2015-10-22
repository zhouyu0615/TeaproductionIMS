#pragma once
#include <queue>
#include "PlcWriteRequest.h"

class CRequestQueue
{
private:
	CRITICAL_SECTION m_QueueCS; //���е��ٽ�������//
	std::queue<CPlcWriteRequest> m_RequestQueue;

public:
	CRequestQueue();
	~CRequestQueue();

public:
	BOOL IsEmpty();
	void PushBack(CPlcWriteRequest request);
	CPlcWriteRequest PopFront(); //���ض��е�һ��Ԫ��,��ɾ����һ��Ԫ��//
	void Pop();
	void Clear();
};

