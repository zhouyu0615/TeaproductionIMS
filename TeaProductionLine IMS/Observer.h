#pragma once
#include "Subject.h"

class CSubject;

class CObserver
{
public:
	CObserver();
	virtual ~CObserver();
	virtual void Update(CSubject*) = 0;
};

