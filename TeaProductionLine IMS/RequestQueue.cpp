#include "stdafx.h"
#include "RequestQueue.h"


CRequestQueue::CRequestQueue()
{
	InitializeCriticalSection(&m_QueueCS);
}


CRequestQueue::~CRequestQueue()
{
	DeleteCriticalSection(&m_QueueCS);
}

//�жϵ�ǰ�Ķ����Ƿ�Ϊ�գ�����Ϊ���򷵻�True,���򷵻�False//
BOOL CRequestQueue::IsEmpty()
{
	BOOL isEmpty;
	EnterCriticalSection(&m_QueueCS);
	isEmpty = m_RequestQueue.empty();
	LeaveCriticalSection(&m_QueueCS);
	return isEmpty;
}


//��һ��������ӵ�����ĩβ//
void CRequestQueue::PushBack(CPlcWriteRequest request)
{
	EnterCriticalSection(&m_QueueCS);
	m_RequestQueue.push(request);
	LeaveCriticalSection(&m_QueueCS);

}

//���ض��е�һ��Ԫ�أ���ɾ����һ��Ԫ��//
CPlcWriteRequest CRequestQueue::PopFront()
{

	CPlcWriteRequest tempRequest;
	EnterCriticalSection(&m_QueueCS);
	tempRequest=m_RequestQueue.front();
	m_RequestQueue.pop();
	LeaveCriticalSection(&m_QueueCS);

	return tempRequest;
}

//�����������//
void  CRequestQueue::Clear()
{
	EnterCriticalSection(&m_QueueCS);
	while (m_RequestQueue.empty())
	{
		m_RequestQueue.pop();
	}
	LeaveCriticalSection(&m_QueueCS);
}

//ɾ�����е�һ��Ԫ��//
void CRequestQueue::Pop()
{ 
	EnterCriticalSection(&m_QueueCS);
	m_RequestQueue.pop();
	LeaveCriticalSection(&m_QueueCS);
}

