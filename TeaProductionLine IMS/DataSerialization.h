#pragma once

#include "DataProvider.h"

class CDataSerialization
{
public:
	CDataSerialization();
	~CDataSerialization();

	void LoginUserSerial();

	CDataProvider *pData = CDataProvider::getInstance();


};

