#pragma once
#include "Thread.h"


class CRecordThread:Runnable
{
public:
	CRecordThread();
	~CRecordThread();

	CThread *m_pThread;
	virtual void Run(); 

};

