#pragma once
#include <queue>
#include "TcpSendBuffFrame.h"

class CSendBuffQueue
{
private:
	CRITICAL_SECTION m_QueueCS; //���е��ٽ�������//
	std::queue<CTcpSendBuffFrame> m_SendQueue;
public:
	CSendBuffQueue();
	~CSendBuffQueue();

public:
	BOOL IsEmpty();
	void PushBack(CTcpSendBuffFrame tcpSendFrame);
	CTcpSendBuffFrame PopFront(); //���ض��е�һ��Ԫ��,��ɾ����һ��Ԫ��//
	void Pop();
	void Clear();


};

