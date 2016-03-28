#include "stdafx.h"
#include "DataSerialization.h"


CDataSerialization::CDataSerialization()
{
}


CDataSerialization::~CDataSerialization()
{
}


void CDataSerialization::LoginUserSerial()
{
	CFile myFile(_T("../Cache/LoginUser.ser"), CFile::modeCreate | CFile::modeReadWrite);

	// Create a storing archive.  
	CArchive arStore(&myFile, CArchive::store);

	for (int i = 0; i < pData->m_vectLoginUser.size();i++)
	{
		// Write the object to the archive  
		arStore.WriteObject(&pData->m_vectLoginUser[i]);
	}
	arStore.Flush();
	// Close the storing archive//
	arStore.Close();

}