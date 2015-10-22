#include "stdafx.h"
#include "SendBuffQueue.h"


CSendBuffQueue::CSendBuffQueue()
{
	InitializeCriticalSection(&m_QueueCS);
}


CSendBuffQueue::~CSendBuffQueue()
{
	//DeleteCriticalSection(&m_QueueCS);
}

BOOL CSendBuffQueue::IsEmpty()
{
	BOOL isEmpty;
	EnterCriticalSection(&m_QueueCS);
	isEmpty = m_SendQueue.empty();
	LeaveCriticalSection(&m_QueueCS);
	return isEmpty;
}


void CSendBuffQueue::PushBack(CTcpSendBuffFrame tcpSendFrame)
{
	EnterCriticalSection(&m_QueueCS);
	m_SendQueue.push(tcpSendFrame);
	LeaveCriticalSection(&m_QueueCS);
}
//���ض��е�һ��Ԫ��,��ɾ����һ��Ԫ��//
CTcpSendBuffFrame CSendBuffQueue::PopFront()
{
	CTcpSendBuffFrame tempSend;
	EnterCriticalSection(&m_QueueCS);
	tempSend = m_SendQueue.front();
	m_SendQueue.pop();
	LeaveCriticalSection(&m_QueueCS);
	return tempSend;
}


void CSendBuffQueue::Pop()
{
	EnterCriticalSection(&m_QueueCS);
	m_SendQueue.pop();
	LeaveCriticalSection(&m_QueueCS);
}
void CSendBuffQueue::Clear()
{
	EnterCriticalSection(&m_QueueCS);
	while (m_SendQueue.empty())
	{
		m_SendQueue.pop();
	}
	LeaveCriticalSection(&m_QueueCS);
}