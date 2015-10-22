#pragma once
#include <queue>
#include "PlcWriteRequest.h"

class CRequestQueue
{
private:
	CRITICAL_SECTION m_QueueCS; //队列的临界区变量//
	std::queue<CPlcWriteRequest> m_RequestQueue;

public:
	CRequestQueue();
	~CRequestQueue();

public:
	BOOL IsEmpty();
	void PushBack(CPlcWriteRequest request);
	CPlcWriteRequest PopFront(); //返回队列第一个元素,并删除第一个元素//
	void Pop();
	void Clear();
};

