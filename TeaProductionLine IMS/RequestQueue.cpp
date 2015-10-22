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

//判断当前的队列是否为空，队列为空则返回True,否则返回False//
BOOL CRequestQueue::IsEmpty()
{
	BOOL isEmpty;
	EnterCriticalSection(&m_QueueCS);
	isEmpty = m_RequestQueue.empty();
	LeaveCriticalSection(&m_QueueCS);
	return isEmpty;
}


//把一个请求添加到队列末尾//
void CRequestQueue::PushBack(CPlcWriteRequest request)
{
	EnterCriticalSection(&m_QueueCS);
	m_RequestQueue.push(request);
	LeaveCriticalSection(&m_QueueCS);

}

//返回队列第一个元素，并删除第一个元素//
CPlcWriteRequest CRequestQueue::PopFront()
{

	CPlcWriteRequest tempRequest;
	EnterCriticalSection(&m_QueueCS);
	tempRequest=m_RequestQueue.front();
	m_RequestQueue.pop();
	LeaveCriticalSection(&m_QueueCS);

	return tempRequest;
}

//清空整个队列//
void  CRequestQueue::Clear()
{
	EnterCriticalSection(&m_QueueCS);
	while (m_RequestQueue.empty())
	{
		m_RequestQueue.pop();
	}
	LeaveCriticalSection(&m_QueueCS);
}

//删除队列第一个元素//
void CRequestQueue::Pop()
{ 
	EnterCriticalSection(&m_QueueCS);
	m_RequestQueue.pop();
	LeaveCriticalSection(&m_QueueCS);
}

