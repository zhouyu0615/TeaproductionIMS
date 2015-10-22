#pragma once
#include <queue>
#include "TcpSendBuffFrame.h"

class CSendBuffQueue
{
private:
	CRITICAL_SECTION m_QueueCS; //队列的临界区变量//
	std::queue<CTcpSendBuffFrame> m_SendQueue;
public:
	CSendBuffQueue();
	~CSendBuffQueue();

public:
	BOOL IsEmpty();
	void PushBack(CTcpSendBuffFrame tcpSendFrame);
	CTcpSendBuffFrame PopFront(); //返回队列第一个元素,并删除第一个元素//
	void Pop();
	void Clear();


};

