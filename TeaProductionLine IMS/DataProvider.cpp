#include "stdafx.h"
#include "DataProvider.h"

#include "tbUser.h"
#include "tbProductionLine.h"
#include "tbProcessModule.h"
#include "tbDevice.h"
#include "tbPLc.h"
#include "tbVideo.h"
#include "tbLoginUser.h"
#include "tbDevicePara.h"
#include "tbFaultPara.h"
#include "tbProcessPara.h"
#include "tbStatePara.h"
#include "tbParaRecordIndex.h"
#include "tbParaRecord.h"



CDataProvider::CDataProvider()
{

}

CRITICAL_SECTION Cs;
CDataProvider::~CDataProvider()
{
	delete(Instance);
	DeleteCriticalSection(&Cs);
}

CDataProvider::CDataProvider(const CDataProvider&)
{
};

CDataProvider*   CDataProvider::Instance = NULL;

CDataProvider* CDataProvider::getInstance(){
	
	InitializeCriticalSection(&Cs);
	if (Instance==NULL)
	{
		EnterCriticalSection(&Cs);
		if (Instance==NULL)
		{
			Instance = new CDataProvider();
		}	
		LeaveCriticalSection(&Cs);
	}

	return Instance;
}

void CDataProvider::InitDataProvider()
{
	this->ReadLoginUserFormDatabase();

	this->ReadUserFromDatabase();
	this->ReadProLineFromDatabase();
	this->ReadProModuleFromDatabase();
	this->ReadDeviceFromDatabase();
	this->ReadPlcFromDatabase();
	this->ReadVideoFromDatabase();

	this->ReadDeviceParaFromDatabase();
	this->ReadProcessParaFromDatabase();
	this->ReadFaultParaFromDatabase();
	this->ReadStateParaFromDatabase();
	this->ReadFormulaFormDatabase();
}



int CDataProvider::ExecutionSQL(CString strSql)
{
	CtbUser tbUser;
	try{
		if (tbUser.Open(CRecordset::dynaset)){
			tbUser.m_pDatabase->ExecuteSQL(strSql);
		}
		else{
			AfxMessageBox(_T("打开数据库失败！"));
			tbUser.Close();
			return 1;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	tbUser.Close();

	return 0;
}


int CDataProvider::FindProLineId(CString ProducitonLineName)
{
	int ProLineId;
	for (int i = 0; i < m_vectProductionLine.size(); i++)
	{
		if (ProducitonLineName == m_vectProductionLine[i].m_strLineName)
		{
			ProLineId = m_vectProductionLine[i].m_Id;
			return ProLineId;
		}
	}
	return 0;
}


int CDataProvider::FindProModuleId(CString ProductionLineName, CString ModuleName)
{
	int ProModuleId;
	for (int i = 0; i < m_vectProcessModule.size(); i++)
	{
		if (ProductionLineName == m_vectProcessModule[i].m_strProductionLineName
			&&ModuleName == m_vectProcessModule[i].m_strProcessModuleName)
		{
			ProModuleId = m_vectProcessModule[i].m_Id;
			return ProModuleId;
		}
	}
	return 0;
}


int CDataProvider::FindPlcId(CString PlcName)
{
	int PlcId;
	for (int i = 0; i < m_vectPlc.size();i++)
	{
		if (PlcName==m_vectPlc[i].m_strPlcName)
		{
			PlcId = m_vectPlc[i].m_Id;
			return PlcId;
		}
	}
	return 0;

}

int CDataProvider::FindDeviceId(CString ProductionLineName, CString ModuleName, CString DeviceName)
{
	int DeviceId;
	for (int i = 0; i < m_vectDevice.size();i++)
	{
		if (ProductionLineName==m_vectDevice[i].m_strProductionLineName
			&&ModuleName==m_vectDevice[i].m_strProcessModuleName
			&&DeviceName==m_vectDevice[i].m_strDeviceName)
		{
			DeviceId = m_vectDevice[i].m_Id;
			return DeviceId;
		}
	}

	return 0;
}

int CDataProvider::FindProcessParaId(CString ProcessParaName)
{
	for (int i = 0; i < m_vectProModulePara.size(); i++)
	{
		if (ProcessParaName == m_vectProModulePara[i].m_strParaName)
		{
			return m_vectProModulePara[i].m_Id;
		}
	}
	return 0;
}

CString CDataProvider::FindProcessParaName(int ProcessParaId)
{
	for (int i = 0; i < m_vectProModulePara.size(); i++)
	{
		if (ProcessParaId == m_vectProModulePara[i].m_strParaName)
		{
			return m_vectProModulePara[i].m_strParaName;
		}
	}
	return _T("");
}


void CDataProvider::AddLoginUserToDatabase(CLoginUser tempLoginUser)
{
	int length = m_vectLoginUser.size();
	if (length==0)
	{
		tempLoginUser.m_Id = 1000; //登陆用户起始ID
	}
	else{
		tempLoginUser.m_Id = m_vectLoginUser[length - 1].m_Id + 1;
	}

	m_vectLoginUser.push_back(tempLoginUser);

	CtbLoginUser m_tbLoginUserSet;
	try{
		if (m_tbLoginUserSet.IsOpen())
			m_tbLoginUserSet.Close();
		if (!m_tbLoginUserSet.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (m_tbLoginUserSet.CanUpdate()){
		m_tbLoginUserSet.AddNew();

		CTime time = CTime::GetCurrentTime();
		m_tbLoginUserSet.m_Id = tempLoginUser.m_Id;
		m_tbLoginUserSet.m_CreatedDateTime = time;
		m_tbLoginUserSet.m_LastUpdatedDateTime = time;
		m_tbLoginUserSet.m_LoginName = tempLoginUser.m_strLoginName;
		m_tbLoginUserSet.m_LoginPassWd = tempLoginUser.m_strLoginPassWd;
		m_tbLoginUserSet.m_PermissionLevel = tempLoginUser.m_PermissionLevel;
		m_tbLoginUserSet.m_IsAutoLogin = tempLoginUser.m_IsAutoLogin;
		m_tbLoginUserSet.m_IsRememberPasswd = tempLoginUser.m_IsRememberPasswd;

		m_tbLoginUserSet.Update();

	}
	m_tbLoginUserSet.Close();

}

void CDataProvider::UpdateUserLoginTime(int LoginUserId)
{
	CString strsql;
	strsql.Format(_T("UPDATE tbLoginUser SET LastLoginInTime=getdate() where ID=%d"),LoginUserId);
	CtbLoginUser tbLoginUser;
	try{
		if (tbLoginUser.Open(CRecordset::dynaset)){
			tbLoginUser.m_pDatabase->ExecuteSQL(strsql);
		}
		else{
			AfxMessageBox(_T("打开数据库失败！"));
			tbLoginUser.Close();
			return ;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	tbLoginUser.Close();
}

void CDataProvider::AddUserToDatabase(CUserClass tempUser)
{
	CtbUser m_tbUserSet;
	try{
		if (m_tbUserSet.IsOpen())
			m_tbUserSet.Close();
		if (!m_tbUserSet.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	

	int length = m_vectUser.size();
	if (m_vectUser.size() == 0) //设置第一个用户的ID
	{
		tempUser.m_UserId = 1000;
	}
	else
	{
		tempUser.m_UserId = m_vectUser[length - 1].m_UserId + 1;
	}

	m_vectUser.push_back(tempUser);

	if (m_tbUserSet.CanUpdate()){
		m_tbUserSet.AddNew();

		CTime time = CTime::GetCurrentTime();
		m_tbUserSet.m_Id = tempUser.m_UserId;
		m_tbUserSet.m_CreatedDateTime = time;
		m_tbUserSet.m_LastUpdatedDateTime = time;
		m_tbUserSet.m_UserName = tempUser.m_strUserName;
		m_tbUserSet.m_UserPassword = tempUser.m_strUserPasswd;
		m_tbUserSet.m_UserCode = tempUser.m_strUserCode;
		m_tbUserSet.m_Note = tempUser.m_strNote;
		m_tbUserSet.Update();

	}
	m_tbUserSet.Close();
}




void CDataProvider::AddProLineToDatabase(CProductionLineClass tempProLine)
{

	CtbProductionLine tbProductionLine;
	try{
		if (tbProductionLine.IsOpen())
			tbProductionLine.Close();
		if (!tbProductionLine.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	
	int length = m_vectProductionLine.size();

	if (length == 0)//设置第一条生产线的ID
	{
		tempProLine.m_Id = 1;
		tempProLine.m_SortIndex = 1;
	}
	else{
		tempProLine.m_Id = GetMaxProlineId() + 1;
		tempProLine.m_SortIndex = m_vectProductionLine[length - 1].m_SortIndex + 1;
	}
	tempProLine.m_UserId = m_vectUser[0].m_UserId; ////唯一用户ID

	m_vectProductionLine.push_back(tempProLine);

	if (tbProductionLine.CanUpdate()){
		tbProductionLine.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbProductionLine.m_Id = tempProLine.m_Id;
		tbProductionLine.m_CreatedDateTime = time;
		tbProductionLine.m_LastUpdatedDateTime = time;
		tbProductionLine.m_LineName = tempProLine.m_strLineName;
		tbProductionLine.m_UserId = tempProLine.m_UserId; 
		tbProductionLine.m_Capacity = tempProLine.m_strCapacity;
		tbProductionLine.m_Description = tempProLine.m_strDescription;
		tbProductionLine.m_SortIndex = tempProLine.m_SortIndex;
		tbProductionLine.Update();
	}

	tbProductionLine.Close();
}




void CDataProvider::AddProModuleToDatabase(CProcessModuleClass tempProModule)
{
	CtbProcessModule tbProcessModule;
	try{
		if (tbProcessModule.IsOpen())
			tbProcessModule.Close();
		if (!tbProcessModule.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	int length = m_vectProcessModule.size();
	if (length == 0)//设置第一个模块ID
	{
		tempProModule.m_Id = 1000;
		tempProModule.m_SortIndex = 1000;
	}
	else{
		tempProModule.m_Id = GetMaxModuleId() + 1;
		tempProModule.m_SortIndex = m_vectProcessModule[length - 1].m_SortIndex + 1;
	}

	//存储模块所属用户ID
	tempProModule.m_UserId = m_vectUser[0].m_UserId;
	//找出工艺模块所属生产线的ID
	int ProLineId = FindProLineId(tempProModule.m_strProductionLineName);
	tempProModule.m_ProcessLineId = ProLineId;

	m_vectProcessModule.push_back(tempProModule);
	
	if (tbProcessModule.CanUpdate()){
		tbProcessModule.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbProcessModule.m_Id = tempProModule.m_Id;
		tbProcessModule.m_CreatedDateTime = time;
		tbProcessModule.m_LastUpdatedDateTime = time;
		tbProcessModule.m_ProductionLineName = tempProModule.m_strProductionLineName;
		tbProcessModule.m_UserId = tempProModule.m_UserId; //唯一用户ID
		tbProcessModule.m_ProductionLineId = tempProModule.m_ProcessLineId; //所属生产线ID
		tbProcessModule.m_ModuleName = tempProModule.m_strProcessModuleName;
		tbProcessModule.m_Description = tempProModule.m_strDescription;
		tbProcessModule.m_SortIndex = tempProModule.m_SortIndex;
		tbProcessModule.Update();
	}

	tbProcessModule.Close();


}

void CDataProvider::AddDeviceToDatabase(CDeviceClass tempDevice)
{
	CtbDevice tbDevice;
	try{
		if (tbDevice.IsOpen())
			tbDevice.Close();
		if (!tbDevice.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	
	int length = m_vectDevice.size();
	if (length == 0)//设置第一个模块ID
	{
		tempDevice.m_Id = 1000;
		tempDevice.m_SortIndex = 1000;
	}
	else{
		tempDevice.m_Id = GetMaxDeviceId() + 1;
		tempDevice.m_SortIndex = m_vectDevice[length - 1].m_SortIndex + 1;
	}
	

	int ProLineId, ProModuleId;
	//找到该设备所属生产线的ID
	ProLineId = FindProLineId(tempDevice.m_strProductionLineName);
	//找到设备所属的工艺模块的ID
	ProModuleId = FindProModuleId(tempDevice.m_strProductionLineName, tempDevice.m_strProcessModuleName);

	tempDevice.m_ProductionLineId = ProLineId;
	tempDevice.m_ProcessModuleId = ProModuleId;

	m_vectDevice.push_back(tempDevice);

	if (tbDevice.CanUpdate()){
		tbDevice.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbDevice.m_Id = tempDevice.m_Id;
		tbDevice.m_CreatedDateTime = time;
		tbDevice.m_LastUpdatedDateTime = time;
		tbDevice.m_ProductionLineName = tempDevice.m_strProductionLineName;
		tbDevice.m_ProcessModuleName = tempDevice.m_strProcessModuleName;
		tbDevice.m_DeviceName = tempDevice.m_strDeviceName;
		tbDevice.m_DeviceType = tempDevice.m_strDeviceType;
		tbDevice.m_ProductionLineId = tempDevice.m_ProductionLineId; //所属生产线的ID
		tbDevice.m_ProcessModuleId = tempDevice.m_ProcessModuleId;// 所属工艺模块的ID
		tbDevice.m_SortIndex = tempDevice.m_SortIndex;

		tbDevice.Update();

	}

	tbDevice.Close();
}


void CDataProvider::AddPlcToDatabase(CPlcClass tempPlc)
{
	CtbPLc tbPlc;
	try{
		if (tbPlc.IsOpen())
			tbPlc.Close();
		if (!tbPlc.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	
	int length = m_vectPlc.size();
	if (length == 0)//设置第一个PLC ID
	{
		tempPlc.m_Id = 1000;
		tempPlc.m_SortIndex = 1000;
	}
	else{
		tempPlc.m_Id = GetMaxPlcId() + 1;
		tempPlc.m_SortIndex = m_vectPlc[length - 1].m_SortIndex+ 1;
	}
	
	tempPlc.m_UserId = m_vectUser[0].m_UserId;
	int ProlineId = FindProLineId(tempPlc.m_strProductionLineName); //找出PLC所属生产线的ID
	tempPlc.m_ProductionLineId = ProlineId;

	m_vectPlc.push_back(tempPlc);

	if (tbPlc.CanUpdate()){
		tbPlc.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbPlc.m_Id = tempPlc.m_Id;
		tbPlc.m_CreatedDateTime = time;
		tbPlc.m_LastUpdatedDateTime = time;
		tbPlc.m_ProductionLineName = tempPlc.m_strProductionLineName;
		tbPlc.m_UserId = tempPlc.m_UserId; //唯一用户ID
		tbPlc.m_PLCName = tempPlc.m_strPlcName;
		tbPlc.m_strIPAddr = tempPlc.m_strIPAddr;
		tbPlc.m_ReadStartAddr = tempPlc.m_ReadStartAddr;
		tbPlc.m_ReadLength = tempPlc.m_ReadLength;
		tbPlc.m_WriteStartAddr = tempPlc.m_WriteStartAddr;
		tbPlc.m_WriteLength = tempPlc.m_WriteLength;
		tbPlc.m_Description = tempPlc.m_strDescription;
		tbPlc.m_SortIndex = tempPlc.m_SortIndex;

		tbPlc.Update();

	}

	tbPlc.Close();
}


void CDataProvider::AddVideoToDatabase(CVideoClass tempVideo)
{

	CtbVideo tbVideo;
	try{
		if (tbVideo.IsOpen())
			tbVideo.Close();
		if (!tbVideo.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}


	int length = m_vectVideo.size();
	if (length == 0)//设置第一个模块ID
	{
		tempVideo.m_Id = 1000;
		tempVideo.m_SortIndex = 1000;
	}
	else{
		tempVideo.m_Id = GetMaxVideoId() + 1;
		tempVideo.m_SortIndex = m_vectVideo[length - 1].m_SortIndex + 1;
	}
	
	
	tempVideo.m_ProductionLineId = FindProLineId(tempVideo.m_strProductionLineName);
	tempVideo.m_ModuleId = FindProModuleId(tempVideo.m_strProductionLineName, tempVideo.m_strProcessModuleName);


	m_vectVideo.push_back(tempVideo);

	if (tbVideo.CanUpdate()){
		tbVideo.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbVideo.m_Id = tempVideo.m_Id;
		tbVideo.m_CreatedDateTime = time;
		tbVideo.m_LastUpdatedDateTime = time;
		tbVideo.m_ProductionLineName = tempVideo.m_strProductionLineName;
		tbVideo.m_ProcessModuleName = tempVideo.m_strProcessModuleName;
		tbVideo.m_VideoName = tempVideo.m_strVideoName;
		tbVideo.m_IPAddr = tempVideo.m_strIPAddr;
		tbVideo.m_Port = tempVideo.m_port;

		tbVideo.m_ProductionLineId = tempVideo.m_ProductionLineId;
		tbVideo.m_ProcessModuleId = tempVideo.m_ModuleId;
		tbVideo.m_SortIndex = tempVideo.m_SortIndex;

		tbVideo.Update();

	}

	tbVideo.Close();

}


void CDataProvider::AddDeviceParaToDatabase(CDevicePara tempDevicePara)
{
	CtbDevicePara tbDevicePara;
	try{
		if (tbDevicePara.IsOpen())
			tbDevicePara.Close();
		if (!tbDevicePara.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}


	int length = m_vectDevicePara.size();
	if (length == 0)//设置第一个模块ID
	{
		tempDevicePara.m_Id = 1000;
	}
	else{
		tempDevicePara.m_Id = m_vectDevicePara[length - 1].m_Id + 1;
	}

	tempDevicePara.m_ProductionLineId = FindProLineId(tempDevicePara.m_strProductionLineName);
	tempDevicePara.m_ProcessModuleId = FindProModuleId(tempDevicePara.m_strProductionLineName, tempDevicePara.m_strProcessModuleName);
	tempDevicePara.m_PlcId = FindPlcId(tempDevicePara.m_strPlcName);
	tempDevicePara.m_DeviceId = FindDeviceId(tempDevicePara.m_strProductionLineName, tempDevicePara.m_strProcessModuleName, tempDevicePara.m_strDeviceName);

	m_vectDevicePara.push_back(tempDevicePara);

	if (tbDevicePara.CanUpdate()){
		tbDevicePara.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbDevicePara.m_Id = tempDevicePara.m_Id;
		tbDevicePara.m_CreatedDateTime = time;
		tbDevicePara.m_LastUpdatedDateTime = time;
		tbDevicePara.m_ProductionLineName = tempDevicePara.m_strProductionLineName;
		tbDevicePara.m_ProcessModuleName = tempDevicePara.m_strProcessModuleName;
		tbDevicePara.m_PlcName = tempDevicePara.m_strPlcName;
		tbDevicePara.m_PLCId = tempDevicePara.m_PlcId;
		tbDevicePara.m_DeviceName = tempDevicePara.m_strDeviceName;
		tbDevicePara.m_DeviceId = tempDevicePara.m_DeviceId;
		tbDevicePara.m_ProductionLineId = tempDevicePara.m_ProductionLineId;
		tbDevicePara.m_ProcessModuleId = tempDevicePara.m_ProcessModuleId;
		tbDevicePara.m_ControlAddrIndex = tempDevicePara.m_strControlAddrIndex;
		tbDevicePara.m_ControlValue = tempDevicePara.m_ControlValue;
		tbDevicePara.m_StateValue = tempDevicePara.m_StateValue;
		tbDevicePara.m_StateAddrIndex = tempDevicePara.m_strStateAddrIndex;
		tbDevicePara.m_ParaName = tempDevicePara.m_strParaName;
		tbDevicePara.m_Description = tempDevicePara.m_strDescription;


		tbDevicePara.Update();

	}
	tbDevicePara.Close();
}


void CDataProvider::AddFaultParaToDatabase(CFaultPara tempFaultPara)
{
	CtbFaultPara tbFaultPara;
	try{
		if (tbFaultPara.IsOpen())
			tbFaultPara.Close();
		if (!tbFaultPara.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}


	int length = m_vectFaultPara.size();
	if (length == 0)//设置第一个模块ID
	{
		tempFaultPara.m_Id = 1000;
	}
	else{
		tempFaultPara.m_Id = m_vectFaultPara[length - 1].m_Id + 1;
	}

	tempFaultPara.m_ProductionLineId = FindProLineId(tempFaultPara.m_strProductionLineName);
	tempFaultPara.m_ProcessModuleId = FindProModuleId(tempFaultPara.m_strProductionLineName, tempFaultPara.m_strProcessName);
	tempFaultPara.m_DeviceId = FindDeviceId(tempFaultPara.m_strProductionLineName, tempFaultPara.m_strProcessName, tempFaultPara.m_strDeviceName);
	tempFaultPara.m_PLCId = FindPlcId(tempFaultPara.m_strPlcName);

	m_vectFaultPara.push_back(tempFaultPara);

	if (tbFaultPara.CanUpdate()){
		tbFaultPara.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbFaultPara.m_Id = tempFaultPara.m_Id;
		tbFaultPara.m_CreatedDateTime = time;
		tbFaultPara.m_LastUpdatedDateTime = time;
		tbFaultPara.m_ProductionLineName = tempFaultPara.m_strProductionLineName;
		tbFaultPara.m_ProcessModuleName = tempFaultPara.m_strProcessName;
		tbFaultPara.m_ProductionLineId = tempFaultPara.m_ProductionLineId;
		tbFaultPara.m_ProcessModuleId = tempFaultPara.m_ProcessModuleId;
		tbFaultPara.m_DeviceName = tempFaultPara.m_strDeviceName;
		tbFaultPara.m_DeviceId = tempFaultPara.m_DeviceId;
		tbFaultPara.m_PlcName = tempFaultPara.m_strPlcName;
		tbFaultPara.m_PLCId = tempFaultPara.m_PLCId;
		tbFaultPara.m_AddressIndex = tempFaultPara.m_strAddressIndex;
		tbFaultPara.m_ParaValue = tempFaultPara.m_ParaValue;
		tbFaultPara.m_ParaName = tempFaultPara.m_strParaName;
		tbFaultPara.m_Description = tempFaultPara.m_strDescription;

		tbFaultPara.Update();

	}

	tbFaultPara.Close();

}

void CDataProvider::AddProcessParaToDatabase(CProcessPara tempProcessPara)
{
	CtbProcessPara tbProcessPara;
	try{
		if (tbProcessPara.IsOpen())
			tbProcessPara.Close();
		if (!tbProcessPara.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}


	int length = m_vectProModulePara.size();
	if (length == 0)//设置第一个模块ID
	{
		tempProcessPara.m_Id = 1000;
	}
	else{
		tempProcessPara.m_Id = m_vectProModulePara[length - 1].m_Id + 1;
	}

	tempProcessPara.m_ProductionLineId = FindProLineId(tempProcessPara.m_strProductionLineName);
	tempProcessPara.m_ProcessModuleId = FindProModuleId(tempProcessPara.m_strProductionLineName, tempProcessPara.m_strProcessModuleName);
	tempProcessPara.m_PlcId = FindPlcId(tempProcessPara.m_strPlcName);

	m_vectProModulePara.push_back(tempProcessPara);

	if (tbProcessPara.CanUpdate()){
		tbProcessPara.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbProcessPara.m_Id = tempProcessPara.m_Id;
		tbProcessPara.m_CreatedDateTime = time;
		tbProcessPara.m_LastUpdatedDateTime = time;
		tbProcessPara.m_ProductionLineName = tempProcessPara.m_strProductionLineName;
		tbProcessPara.m_ProcessModuleName = tempProcessPara.m_strProcessModuleName;
		tbProcessPara.m_ProductionLineId = tempProcessPara.m_ProductionLineId;
		tbProcessPara.m_ProcessModuleId = tempProcessPara.m_ProcessModuleId;
		tbProcessPara.m_PlcName = tempProcessPara.m_strPlcName;
		tbProcessPara.m_PLCId = tempProcessPara.m_PlcId;
		tbProcessPara.m_ReadAddressIndex = tempProcessPara.m_strReadAddrIndex;
		tbProcessPara.m_WriteAddressIndex = tempProcessPara.m_strWriteAddrIndex;
		tbProcessPara.m_Units = tempProcessPara.m_strUnit;
		tbProcessPara.m_AddressType = tempProcessPara.m_strAddressType;
		tbProcessPara.m_ParaValue = tempProcessPara.m_ParaValue;
		tbProcessPara.m_ParaName = tempProcessPara.m_strParaName;
		tbProcessPara.m_Description = tempProcessPara.m_strDescription;
		tbProcessPara.m_ValueType = tempProcessPara.m_ValueType;
		tbProcessPara.m_IsConfig = tempProcessPara.m_IsConfig;
		tbProcessPara.m_IsVisible = tempProcessPara.m_IsVisible;

		tbProcessPara.m_IsRecord = tempProcessPara.m_IsRecord;

		tbProcessPara.Update();

	}
	tbProcessPara.Close();


	if (tempProcessPara.m_IsRecord == TRUE)
	{
		CParaRecordIndex paraIndex;
		paraIndex.m_ProParaId = tempProcessPara.m_Id;
		AddRecordTbIndex(paraIndex);
	}



}

void CDataProvider::AddStateParaToDatabase(CStatePara tempStatePara)
{
	CtbStatePara tbStatePara;
	try{
		if (tbStatePara.IsOpen())
			tbStatePara.Close();
		if (!tbStatePara.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	int length = m_vectStatePara.size();
	if (length == 0)//设置第一个模块ID
	{
		tempStatePara.m_Id = 1000;
	}
	else{
		tempStatePara.m_Id = m_vectStatePara[length - 1].m_Id + 1;
	}

	tempStatePara.m_ProductionLineId = FindProLineId(tempStatePara.m_strProductionLineName);
	tempStatePara.m_ProcessModuleId = FindProModuleId(tempStatePara.m_strProductionLineName, tempStatePara.m_strProcessModuleName);
	tempStatePara.m_PlcId = FindPlcId(tempStatePara.m_strPlcName);


	m_vectStatePara.push_back(tempStatePara);

	if (tbStatePara.CanUpdate()){
		tbStatePara.AddNew();

		CTime time = CTime::GetCurrentTime();
		tbStatePara.m_Id = tempStatePara.m_Id;
		tbStatePara.m_CreatedDateTime = time;
		tbStatePara.m_LastUpdatedDateTime = time;
		tbStatePara.m_ProductionLineName = tempStatePara.m_strProductionLineName;
		tbStatePara.m_ProcessModuleName = tempStatePara.m_strProcessModuleName;
		tbStatePara.m_ProductionLineId = tempStatePara.m_ProductionLineId;
		tbStatePara.m_ProcessModuleId = tempStatePara.m_ProcessModuleId;
		tbStatePara.m_PlcName = tempStatePara.m_strPlcName;
		tbStatePara.m_PLCId = tempStatePara.m_PlcId;
		tbStatePara.m_AddressIndex = tempStatePara.m_strAddressIndex;
		tbStatePara.m_ParaValue = tempStatePara.m_ParaValue;
		tbStatePara.m_ParaName = tempStatePara.m_strParaName;
		tbStatePara.m_Description = tempStatePara.m_strDescription;

		tbStatePara.Update();

	}

	tbStatePara.Close();
	

}


void CDataProvider::AddFormulaToDatabase(CFormulaClass tempFormula)
{
	CtbFormula tbFormula;
	try{
		if (tbFormula.IsOpen())
			tbFormula.Close();
		if (!tbFormula.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	int length = m_vectFormula.size();
	if (length == 0)//设置第一个模块ID
	{
		tempFormula.m_Id = 1000;
		
	}
	else{
		tempFormula.m_Id = m_vectFormula[length - 1].m_Id + 1;		
	}

	tempFormula.m_FormulaId = FindFormulaId(tempFormula.m_strFormulaName);
	//tempFormula.m_ProductionLineId = FindProLineId(tempFormula.m_strProductionLineName);
	//tempFormula.m_strProcessParaName = FindProcessParaName(tempFormula.m_ProcessParaId);

	m_vectFormula.push_back(tempFormula);
	if (tbFormula.CanUpdate()){
		tbFormula.AddNew();
		CTime time = CTime::GetCurrentTime();
		tbFormula.m_Id = tempFormula.m_Id;
		tbFormula.m_CreatedDateTime = time;
		tbFormula.m_LastUpdatedDateTime = time;
		tbFormula.m_ProductionLineName = tempFormula.m_strProductionLineName;
		tbFormula.m_ProductionLineId = tempFormula.m_ProductionLineId;
		tbFormula.m_FormulaId = tempFormula.m_FormulaId;
		tbFormula.m_FormulaName = tempFormula.m_strFormulaName;
		tbFormula.m_Note = tempFormula.m_strNote;
		tbFormula.m_ParaValueUnit = tempFormula.m_strParaValueUnit;
		tbFormula.m_DefaultValue = tempFormula.m_DefaultValue;
		tbFormula.m_ProcessParaId = tempFormula.m_ProcessParaId;
		tbFormula.m_ProcessParaName = tempFormula.m_strProcessParaName;
		tbFormula.m_IsDefaultFormula = tempFormula.m_IsDefaultFormula;
		tbFormula.m_IsCurrentFormula = tempFormula.m_IsCurrentFormula;
		tbFormula.Update();
	}

	tbFormula.Close();
}

//如果为第一个配方的第一条记录，则分配FormulaID为1000，若存在同一个配方，则返回同一配方的ID
//若为新建配方，则另外分配一个FormulaID
int CDataProvider::FindFormulaId(CString FormulaName)
{
	int len = m_vectFormula.size();
	if(len==0)
	{
		return 1000;
	}
	else{
	
		for (int i = 0; i < m_vectFormula.size(); i++)
		{
			if (FormulaName == m_vectFormula[i].m_strFormulaName)
			{
				return m_vectFormula[i].m_FormulaId;
			}
		}
		return m_vectFormula[len - 1].m_FormulaId + 1;
	}
}

void CDataProvider::ReadDeviceParaFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbDevicePara order by Id"));

	CtbDevicePara  tbDevicePara;
	try{
		if (tbDevicePara.IsOpen())
			tbDevicePara.Close();
		if (!tbDevicePara.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbDevicePara.IsBOF())
	{
		return;
	}

	m_vectDevicePara.clear();
	CDevicePara tempDevicePara;
	tbDevicePara.MoveFirst();
	while (!tbDevicePara.IsEOF()){
		tempDevicePara.m_Id = tbDevicePara.m_Id;
		tempDevicePara.m_strCreatedDateTime = tbDevicePara.m_CreatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempDevicePara.m_strLastUpdatedDateTime=tbDevicePara.m_LastUpdatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempDevicePara.m_strProductionLineName = tbDevicePara.m_ProductionLineName;
		tempDevicePara.m_strProcessModuleName = tbDevicePara.m_ProcessModuleName;		
		tempDevicePara.m_strPlcName=tbDevicePara.m_PlcName ;

		tempDevicePara.m_strDeviceName=tbDevicePara.m_DeviceName ;
		tempDevicePara.m_DeviceId=tbDevicePara.m_DeviceId;
		tempDevicePara.m_PlcId = tbDevicePara.m_PLCId;
		tempDevicePara.m_ProductionLineId = tbDevicePara.m_ProductionLineId;
		tempDevicePara.m_ProcessModuleId=tbDevicePara.m_ProcessModuleId;
		tempDevicePara.m_strControlAddrIndex=tbDevicePara.m_ControlAddrIndex ;
		tempDevicePara.m_ControlValue=tbDevicePara.m_ControlValue;
		tempDevicePara.m_StateValue=tbDevicePara.m_StateValue;
		tempDevicePara.m_strStateAddrIndex=tbDevicePara.m_StateAddrIndex;
		tempDevicePara.m_strParaName=tbDevicePara.m_ParaName;
		tempDevicePara.m_strDescription=tbDevicePara.m_Description;

		m_vectDevicePara.push_back(tempDevicePara);
		tbDevicePara.MoveNext();
	}

	tbDevicePara.Close();
}

void CDataProvider::ReadFaultParaFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbFaultPara order by Id"));

	CtbFaultPara  tbFaultPara;
	try{
		if (tbFaultPara.IsOpen())
			tbFaultPara.Close();
		if (!tbFaultPara.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbFaultPara.IsBOF())
	{
		return;
	}
	m_vectFaultPara.clear();
	CFaultPara tempFaultPara;
	
	tbFaultPara.MoveFirst();
	while (!tbFaultPara.IsEOF()){

		tempFaultPara.m_Id=tbFaultPara.m_Id;
		tempFaultPara.m_strCreatedDateTime = tbFaultPara.m_CreatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempFaultPara.m_strLastUpdatedDateTime=tbFaultPara.m_LastUpdatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempFaultPara.m_strProductionLineName=tbFaultPara.m_ProductionLineName;
		tempFaultPara.m_strProcessName=tbFaultPara.m_ProcessModuleName;
		tempFaultPara.m_ProductionLineId=tbFaultPara.m_ProductionLineId;
		tempFaultPara.m_ProcessModuleId=tbFaultPara.m_ProcessModuleId;
		tempFaultPara.m_strDeviceName=tbFaultPara.m_DeviceName;
		tempFaultPara.m_DeviceId=tbFaultPara.m_DeviceId;
		tempFaultPara.m_strPlcName=tbFaultPara.m_PlcName;
		tempFaultPara.m_PLCId=tbFaultPara.m_PLCId;
		tempFaultPara.m_strAddressIndex=tbFaultPara.m_AddressIndex;
		tempFaultPara.m_ParaValue=tbFaultPara.m_ParaValue;
		tempFaultPara.m_strParaName=tbFaultPara.m_ParaName;
		tempFaultPara.m_strDescription=tbFaultPara.m_Description;

		m_vectFaultPara.push_back(tempFaultPara);
		tbFaultPara.MoveNext();
	}
	tbFaultPara.Close();

}

void CDataProvider::ReadProcessParaFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbProcessPara order by Id"));

	CtbProcessPara  tbProcessPara;
	try{
		if (tbProcessPara.IsOpen())
			tbProcessPara.Close();
		if (!tbProcessPara.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbProcessPara.IsBOF())
	{
		return;
	}

	m_vectProModulePara.clear();
	CProcessPara tempProcessPara;
	tbProcessPara.MoveFirst();
	while (!tbProcessPara.IsEOF()){

		tempProcessPara.m_Id=tbProcessPara.m_Id;
		tempProcessPara.m_strCreatedDateTime= tbProcessPara.m_CreatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempProcessPara.m_strLastUpdatedDateTime = tbProcessPara.m_LastUpdatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempProcessPara.m_strProductionLineName=tbProcessPara.m_ProductionLineName;
		tempProcessPara.m_strProcessModuleName=tbProcessPara.m_ProcessModuleName;
		tempProcessPara.m_ProductionLineId=tbProcessPara.m_ProductionLineId;
		tempProcessPara.m_ProcessModuleId=tbProcessPara.m_ProcessModuleId;
		tempProcessPara.m_strPlcName=tbProcessPara.m_PlcName;
		tempProcessPara.m_PlcId=tbProcessPara.m_PLCId;
		tempProcessPara.m_strReadAddrIndex=tbProcessPara.m_ReadAddressIndex;
		tempProcessPara.m_strWriteAddrIndex = tbProcessPara.m_WriteAddressIndex;
		tempProcessPara.m_strUnit = tbProcessPara.m_Units;
		tempProcessPara.m_strAddressType=tbProcessPara.m_AddressType;
		tempProcessPara.m_ParaValue=tbProcessPara.m_ParaValue;
		tempProcessPara.m_strParaName=tbProcessPara.m_ParaName;
		tempProcessPara.m_strDescription=tbProcessPara.m_Description;
		tempProcessPara.m_ValueType=tbProcessPara.m_ValueType;
		tempProcessPara.m_IsConfig=tbProcessPara.m_IsConfig;
		tempProcessPara.m_IsVisible=tbProcessPara.m_IsVisible;

		tempProcessPara.m_IsRecord = tbProcessPara.m_IsRecord;

		m_vectProModulePara.push_back(tempProcessPara);
		tbProcessPara.MoveNext();
	}
	tbProcessPara.Close();

}

void CDataProvider::ReadStateParaFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbStatePara order by Id"));

	CtbStatePara  tbStatePara;
	try{
		if (tbStatePara.IsOpen())
			tbStatePara.Close();
		if (!tbStatePara.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbStatePara.IsBOF())
	{
		return;
	}
	m_vectStatePara.clear();
	CStatePara tempStatePara;
	tbStatePara.MoveFirst();
	while (!tbStatePara.IsEOF()){
		tempStatePara.m_Id=tbStatePara.m_Id;
		tempStatePara.m_strCreatedDateTime = tbStatePara.m_CreatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempStatePara.m_strLastUpdateDateTime=tbStatePara.m_LastUpdatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempStatePara.m_strProductionLineName=tbStatePara.m_ProductionLineName;
		tempStatePara.m_strProcessModuleName=tbStatePara.m_ProcessModuleName;
		tempStatePara.m_ProductionLineId=tbStatePara.m_ProductionLineId;
		tempStatePara.m_ProcessModuleId=tbStatePara.m_ProcessModuleId;
		tempStatePara.m_strPlcName=tbStatePara.m_PlcName;
		tempStatePara.m_PlcId=tbStatePara.m_PLCId;
		tempStatePara.m_strAddressIndex=tbStatePara.m_AddressIndex;
		tempStatePara.m_ParaValue=tbStatePara.m_ParaValue;
		tempStatePara.m_strParaName=tbStatePara.m_ParaName;
		tempStatePara.m_strDescription=tbStatePara.m_Description;

		m_vectStatePara.push_back(tempStatePara);
		tbStatePara.MoveNext();
	}
	tbStatePara.Close();



}

void CDataProvider::ReadLoginUserFormDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbLoginUser order by Id"));

	CtbLoginUser tbLoginUser;
	try{
		if (tbLoginUser.IsOpen())
			tbLoginUser.Close();
		if (!tbLoginUser.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbLoginUser.IsBOF())
	{
		return;
	}


	 m_vectLoginUser.clear();

	CLoginUser tempLoginUser;
	tbLoginUser.MoveFirst();
	while (!tbLoginUser.IsEOF()){
		//获取类成员变量 唯一的 用户ID
		tempLoginUser.m_Id = tbLoginUser.m_Id;
		tempLoginUser.m_strCreatedDateTime = tbLoginUser.m_CreatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempLoginUser.m_strLastUpdatedDateTime = tbLoginUser.m_LastUpdatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempLoginUser.m_strLastLoginInTime = tbLoginUser.m_LastLoginInTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempLoginUser.m_strLoginName = tbLoginUser.m_LoginName;
		tempLoginUser.m_strLoginPassWd = tbLoginUser.m_LoginPassWd;

		tempLoginUser.m_PermissionLevel = tbLoginUser.m_PermissionLevel;
		tempLoginUser.m_IsRememberPasswd = tbLoginUser.m_IsRememberPasswd;
		tempLoginUser.m_IsAutoLogin = tbLoginUser.m_IsAutoLogin;

		m_vectLoginUser.push_back(tempLoginUser);

		tbLoginUser.MoveNext();
	}

	tbLoginUser.Close();
}


void CDataProvider::ReadUserFromDatabase(){
	CString strsql;
	strsql.Format(_T("select * from tbUser order by Id"));

	CtbUser tbUser;
	try{
		if (tbUser.IsOpen())
			tbUser.Close();
		if (!tbUser.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbUser.IsBOF())
	{
		return;
	}

	m_vectUser.clear();
	CUserClass tempUser;
	tbUser.MoveFirst();
	while (!tbUser.IsEOF()){
		//获取类成员变量 唯一的 用户ID//
		tempUser.m_UserId = tbUser.m_Id;
		tempUser.m_strUserName = tbUser.m_UserName;
		tempUser.m_strUserPasswd = tbUser.m_UserPassword;
		tempUser.m_strUserCode = tbUser.m_UserCode;
		tempUser.m_strNote = tbUser.m_Note;
		m_vectUser.push_back(tempUser);

		tbUser.MoveNext();

	}

	tbUser.Close();

}



void CDataProvider::ReadProLineFromDatabase(){
	CString strsql;
	strsql.Format(_T("select * from tbProductionLine order by SortIndex"));

	CtbProductionLine tbProductionLine;
	try{
		if (tbProductionLine.IsOpen())
			tbProductionLine.Close();
		if (!tbProductionLine.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbProductionLine.IsBOF())
	{
		return;
	}

	m_vectProductionLine.clear();
	CProductionLineClass tempProductionLine;
	tbProductionLine.MoveFirst();
	while (!tbProductionLine.IsEOF()){

		tempProductionLine.m_Id = tbProductionLine.m_Id;
		tempProductionLine.m_strLineName = tbProductionLine.m_LineName;
		tempProductionLine.m_strCapacity = tbProductionLine.m_Capacity;
		tempProductionLine.m_strDescription = tbProductionLine.m_Description;

		tempProductionLine.m_UserId = tbProductionLine.m_UserId;
		tempProductionLine.m_SortIndex = tbProductionLine.m_SortIndex;

		m_vectProductionLine.push_back(tempProductionLine);
		tbProductionLine.MoveNext();
	}

	tbProductionLine.Close();

}


void CDataProvider::ReadProModuleFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbProcessModule order by SortIndex"));

	CtbProcessModule  tbProcessModule;
	try{
		if (tbProcessModule.IsOpen())
			tbProcessModule.Close();
		if (!tbProcessModule.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbProcessModule.IsBOF())
	{
		return;
	}

	m_vectProcessModule.clear();
	CProcessModuleClass tempModule;
	tbProcessModule.MoveFirst();
	while (!tbProcessModule.IsEOF()){

		tempModule.m_Id = tbProcessModule.m_Id;
		tempModule.m_strProductionLineName = tbProcessModule.m_ProductionLineName;
		tempModule.m_strProcessModuleName = tbProcessModule.m_ModuleName;
		tempModule.m_strDescription = tbProcessModule.m_Description;

		tempModule.m_ProcessLineId = tbProcessModule.m_ProductionLineId;

		tempModule.m_UserId = tbProcessModule.m_UserId;
		tempModule.m_SortIndex = tbProcessModule.m_SortIndex;

		m_vectProcessModule.push_back(tempModule);
		tbProcessModule.MoveNext();
	}

	tbProcessModule.Close();

}


void CDataProvider::ReadDeviceFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbDevice order by SortIndex"));

	CtbDevice  tbDevice;
	try{
		if (tbDevice.IsOpen())
			tbDevice.Close();
		if (!tbDevice.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}
	if (tbDevice.IsEOF())
	{
		return;
	}

	m_vectDevice.clear();
	CDeviceClass tempDevice;
	tbDevice.MoveFirst();
	while (!tbDevice.IsEOF()){
		tempDevice.m_Id = tbDevice.m_Id;
		tempDevice.m_strProductionLineName = tbDevice.m_ProductionLineName;
		tempDevice.m_strProcessModuleName = tbDevice.m_ProcessModuleName;
		tempDevice.m_strDeviceName = tbDevice.m_DeviceName;
		tempDevice.m_strDeviceType = tbDevice.m_DeviceType;
		tempDevice.m_ProcessModuleId = tbDevice.m_ProcessModuleId;
		tempDevice.m_ProductionLineId = tbDevice.m_ProductionLineId;
		tempDevice.m_SortIndex = tbDevice.m_SortIndex;

		m_vectDevice.push_back(tempDevice);
		tbDevice.MoveNext();
	}

	tbDevice.Close();

}





void CDataProvider::ReadPlcFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbPLc order by SortIndex"));

	CtbPLc  tbPlc;
	try{
		if (tbPlc.IsOpen())
			tbPlc.Close();
		if (!tbPlc.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbPlc.IsEOF())
	{
		return;
	}

	m_vectPlc.clear();
	CPlcClass tempPlc;
	tbPlc.MoveFirst();
	while (!tbPlc.IsEOF()){

		tempPlc.m_Id = tbPlc.m_Id;
		tempPlc.m_strProductionLineName = tbPlc.m_ProductionLineName;
		tempPlc.m_ProductionLineId = tbPlc.m_ProductionLineId;

		tempPlc.m_strPlcName = tbPlc.m_PLCName;
		tempPlc.m_strIPAddr = tbPlc.m_strIPAddr;
		tempPlc.m_strDescription = tbPlc.m_Description;
		tempPlc.m_ReadStartAddr = tbPlc.m_ReadStartAddr;
		tempPlc.m_ReadLength = tbPlc.m_ReadLength;
		tempPlc.m_WriteStartAddr = tbPlc.m_WriteStartAddr;
		tempPlc.m_WriteLength = tbPlc.m_WriteLength;

		tempPlc.m_SortIndex = tbPlc.m_SortIndex;

		m_vectPlc.push_back(tempPlc);
		tbPlc.MoveNext();
	}

	tbPlc.Close();


}




void CDataProvider::ReadVideoFromDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbVideo order by SortIndex"));

	CtbVideo  tbVideo;
	try{
		if (tbVideo.IsOpen())
			tbVideo.Close();
		if (!tbVideo.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbVideo.IsEOF())
	{
		tbVideo.Close();
		return;
	}

	m_vectVideo.clear();
	CVideoClass tempVideo;
	tbVideo.MoveFirst();
	while (!tbVideo.IsEOF()){

		tempVideo.m_Id = tbVideo.m_Id;
		tempVideo.m_strProductionLineName = tbVideo.m_ProductionLineName;
		tempVideo.m_strProcessModuleName = tbVideo.m_ProcessModuleName;
		tempVideo.m_strIPAddr = tbVideo.m_IPAddr;
		tempVideo.m_strVideoName = tbVideo.m_VideoName;

		tempVideo.m_port = tbVideo.m_Port;

		tempVideo.m_ProductionLineId = tbVideo.m_ProductionLineId;
		tempVideo.m_ModuleId = tbVideo.m_ProcessModuleId;
		tempVideo.m_SortIndex = tbVideo.m_SortIndex;

		m_vectVideo.push_back(tempVideo);
		tbVideo.MoveNext();
	}

	tbVideo.Close();


}

void CDataProvider::ReadFormulaFormDatabase()
{
	CString strsql;
	strsql.Format(_T("select * from tbFormula order by Id"));

	CtbFormula  tbFormula;
	try{
		if (tbFormula.IsOpen())
			tbFormula.Close();
		if (!tbFormula.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbFormula.IsEOF())
	{
		tbFormula.Close();
		return;
	}

	m_vectFormula.clear();
	CFormulaClass tempFormula;
	tbFormula.MoveFirst();
	while (!tbFormula.IsEOF()){

		tempFormula.m_Id = tbFormula.m_Id;

		tempFormula.m_strProductionLineName = tbFormula.m_ProductionLineName ;
		tempFormula.m_ProductionLineId = tbFormula.m_ProductionLineId ;
		tempFormula.m_FormulaId = tbFormula.m_FormulaId ;
		tempFormula.m_strFormulaName = tbFormula.m_FormulaName;
		tempFormula.m_strNote = tbFormula.m_Note;
		tempFormula.m_strParaValueUnit = tbFormula.m_ParaValueUnit ;
		tempFormula.m_DefaultValue = tbFormula.m_DefaultValue;
		tempFormula.m_ProcessParaId = tbFormula.m_ProcessParaId;
		tempFormula.m_strProcessParaName = tbFormula.m_ProcessParaName;

		tempFormula.m_IsDefaultFormula = tbFormula.m_IsDefaultFormula;
		tempFormula.m_IsCurrentFormula = tbFormula.m_IsCurrentFormula ;

		m_vectFormula.push_back(tempFormula);
		tbFormula.MoveNext();
	}

	tbFormula.Close();
}



/*函数功能：删除枚举类型dbTable类型中提供的各表的所有列
举例：DeleteDbTable(tbProductionLine)删除生产线表格所包含的所有的列
*/

int CDataProvider::DeleteDbTable(enumDBTABLE dbTable)
{
	CString strsql;
	
	switch (dbTable)
	{
	case CDataProvider::tbUser:
		strsql.Format(_T("DELETE FROM tbUser"));
		m_vectUser.clear();
		break;
	case CDataProvider::tbProductionLine:
		strsql.Format(_T("DELETE FROM tbProductionLine"));
		m_vectProductionLine.clear();
		break;
	case CDataProvider::tbProcessModule:
		strsql.Format(_T("DELETE FROM tbProcessModule"));
		m_vectProcessModule.clear();
		break;
	case CDataProvider::tbDevice:
		strsql.Format(_T("DELETE FROM tbDevice"));
		m_vectDevice.clear();
		break;
	case CDataProvider::tbPLc:
		strsql.Format(_T("DELETE FROM tbPLc"));
		m_vectPlc.clear();
		break;
	case CDataProvider::tbVideo:
		strsql.Format(_T("DELETE FROM tbVideo"));
		m_vectVideo.clear();
		break;
	case CDataProvider::tbLoginUser:
		strsql.Format(_T("DELETE FROM tbLoginUser"));
		m_vectLoginUser.clear();
		break;
	case CDataProvider::tbDevicePara:
		strsql.Format(_T("DELETE FROM tbDevicePara"));
		m_vectDevicePara.clear();
		break;
	case CDataProvider::tbStatePara:
		strsql.Format(_T("DELETE FROM tbStatePara"));
		m_vectStatePara.clear();
		break;
	case CDataProvider::tbFaultPara:
		strsql.Format(_T("DELETE FROM tbFaultPara"));
		m_vectFaultPara.clear();
		break;
	case CDataProvider::tbProcessPara:
		strsql.Format(_T("DELETE FROM tbProcessPara"));
		m_vectProModulePara.clear();
		break;
	case CDataProvider::tbFormula:
		strsql.Format(_T("DELETE FROM tbFormula"));
		m_vectFormula.clear();
		break;
	default:
		break;
	}

	ExecutionSQL(strsql);

	return 0;
}


/*函数功能：特定删除枚举类型dbTable提供的表中删除特定ID的列*/

 int CDataProvider::DeleteDbTableItem(enumDBTABLE dbTable, int Id)
{
	CString strsql;

	switch (dbTable)
	{
	case CDataProvider::tbUser:
		strsql.Format(_T("DELETE FROM tbUser WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbProductionLine:
		strsql.Format(_T("DELETE FROM tbProductionLine WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbProcessModule:
		strsql.Format(_T("DELETE FROM tbProcessModule WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbDevice:
		strsql.Format(_T("DELETE FROM tbDevice WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbPLc:
		strsql.Format(_T("DELETE FROM tbPLc WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbVideo:
		strsql.Format(_T("DELETE FROM tbVideo WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbLoginUser:
		strsql.Format(_T("DELETE FROM tbLoginUser WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbDevicePara:
		strsql.Format(_T("DELETE FROM tbDevicePara WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbFaultPara:
		strsql.Format(_T("DELETE FROM tbFaultPara WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbProcessPara:
		strsql.Format(_T("DELETE FROM tbProcessPara WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbStatePara:
		strsql.Format(_T("DELETE FROM tbStatePara WHERE Id = '%d'"), Id);
		break;
	case CDataProvider::tbFormula:
		strsql.Format(_T("DELETE FROM tbFormula WHERE Id = '%d'"), Id);
		break;
	default:
		break;
	}

	ExecutionSQL(strsql);
	return 0;
}



/*
函数说明：在修改对应的m_vect+对应项中的值后，再调用此函数，修改数据库中的值
函数功能：在枚举类型dbDBTABLE 中的几个表中，修改特定ID的条目对应的信息
*/
int CDataProvider::UpdateTableItem(enumDBTABLE dbTable, int Id)
{
	CString strsql;
	CProductionLineClass tempProLine;
	CProcessModuleClass tempModule;
	CDeviceClass    tempDevice;
	CPlcClass      tempPlc;
	CVideoClass    tempVideo;

	CLoginUser tempLoginUser;
	CDevicePara tempDevicePara;
	CFaultPara tempFaultPara;
	CProcessPara tempProcessPara;
	CStatePara tempStatePara;
	CFormulaClass tempFormula;

	int i=0;

	switch (dbTable)
	{
	case CDataProvider::tbUser: //用户仅有一个
		strsql.Format(_T("UPDATE tbUser SET LastUpdatedDateTime=getdate(), UserName = '%s',UserPassword='%s',UserCode='%s',Note='%s' WHERE Id ='%d'"),
			m_vectUser[0].m_strUserName, m_vectUser[0].m_strUserPasswd,
			m_vectUser[0].m_strUserCode, m_vectUser[0].m_strNote, Id);
		break;
	case CDataProvider::tbProductionLine:

		for (i = 0; i < m_vectProductionLine.size(); i++)
		{
			if (Id == m_vectProductionLine[i].m_Id)
			{
				tempProLine = m_vectProductionLine[i];
				break;
			}

		}
		strsql.Format(_T("UPDATE tbProductionLine SET LastUpdatedDateTime=getdate(), LineName = '%s',Capacity='%s',Description='%s',SortIndex='%d' WHERE Id = '%d'"),
			tempProLine.m_strLineName,
			tempProLine.m_strCapacity,
			tempProLine.m_strDescription, 
			tempProLine.m_SortIndex,
			Id);
		break;
	case CDataProvider::tbProcessModule:
		for (i = 0; i < m_vectProcessModule.size(); i++)
		{
			if (Id == m_vectProcessModule[i].m_Id)
			{
				tempModule = m_vectProcessModule[i];
				break;
			}

		}
		tempModule.m_ProcessLineId = FindProLineId(tempModule.m_strProductionLineName);
		m_vectProcessModule[i] = tempModule;
		strsql.Format(_T("UPDATE tbProcessModule SET LastUpdatedDateTime=getdate(), ModuleName = '%s',ProductionLineName='%s',Description='%s',ProductionLineId='%d',SortIndex='%d' WHERE Id = '%d'"),
			tempModule.m_strProcessModuleName, 
			tempModule.m_strProductionLineName,
			tempModule.m_strDescription,
			tempModule.m_ProcessLineId,
			tempModule.m_SortIndex,
			Id);
		break;
	case CDataProvider::tbDevice:
		for (i = 0; i < m_vectDevice.size(); i++)
		{
			if (Id == m_vectDevice[i].m_Id)
			{
				tempDevice = m_vectDevice[i];
				break;
			}
		}
		tempDevice.m_ProductionLineId = FindProLineId(tempDevice.m_strProductionLineName);
		tempDevice.m_ProcessModuleId = FindProModuleId(tempDevice.m_strProductionLineName,tempDevice.m_strProcessModuleName);
		m_vectDevice[i] = tempDevice;

		strsql.Format(_T("UPDATE tbDevice  SET LastUpdatedDateTime=getdate(), ProductionLineName='%s',ProcessModuleName = '%s',ProductionLineId='%d',ProcessModuleId='%d',DeviceName='%s',DeviceType='%s',SortIndex='%d' WHERE Id = '%d'"),
			tempDevice.m_strProductionLineName,
			tempDevice.m_strProcessModuleName,
			tempDevice.m_ProductionLineId,
			tempDevice.m_ProcessModuleId,
			tempDevice.m_strDeviceName,
			tempDevice.m_strDeviceType, 
			tempDevice.m_SortIndex,
			Id);

		break;
	case CDataProvider::tbPLc:
		for (i = 0; i < m_vectPlc.size(); i++)
		{
			if (Id == m_vectPlc[i].m_Id)
			{
				tempPlc = m_vectPlc[i];
				break;
			}

		}
		strsql.Format(_T("UPDATE tbPLc SET LastUpdatedDateTime=getdate(), ProductionLineName = '%s',PLCName='%s',strIPAddr='%s',ReadStartAddr='%d',ReadLength='%d',WriteStartAddr='%d',WriteLength='%d',Description='%s',SortIndex='%d' WHERE Id = '%d'"),
			tempPlc.m_strProductionLineName, tempPlc.m_strPlcName,
			tempPlc.m_strIPAddr,
			tempPlc.m_ReadStartAddr,
			tempPlc.m_ReadLength,
			tempPlc.m_WriteStartAddr,
			tempPlc.m_WriteLength,
			tempPlc.m_strDescription, 
			tempPlc.m_SortIndex,
			Id);
		break;
	case CDataProvider::tbVideo:
		for (i = 0; i < m_vectVideo.size(); i++)
		{
			if (Id == m_vectVideo[i].m_Id)
			{
				tempVideo = m_vectVideo[i];
				break;
			}

		}
		tempVideo.m_ProductionLineId = FindProLineId(tempVideo.m_strProductionLineName);
		tempVideo.m_ModuleId = FindProModuleId(tempVideo.m_strProductionLineName, tempVideo.m_strProcessModuleName);
		m_vectVideo[i] = tempVideo;

		strsql.Format(_T("UPDATE tbVideo SET LastUpdatedDateTime=getdate(), ProductionLineName = '%s',ProcessModuleName='%s',ProductionLineId='%d',ProcessModuleId='%d',VideoName='%s',IPAddr='%s',SortIndex='%d' WHERE Id = '%d'"),
			tempVideo.m_strProductionLineName, 
			tempVideo.m_strProcessModuleName,
			tempVideo.m_ProductionLineId,
			tempVideo.m_ModuleId,
			tempVideo.m_strVideoName,
			tempVideo.m_strIPAddr,
			tempVideo.m_SortIndex,
			Id);
		break;

	case CDataProvider::tbLoginUser:
		for (i = 0; i < m_vectLoginUser.size(); i++)
		{
			if (Id == m_vectLoginUser[i].m_Id)
			{
				tempLoginUser = m_vectLoginUser[i];
				break;
			}
		}

		strsql.Format(_T("UPDATE tbLoginUser SET  LastUpdatedDateTime=getdate(), LoginName = '%s',LoginPassWd='%s',PermissionLevel='%d',IsAutoLogin='%d',IsRememberPasswd='%d' WHERE Id = '%d'"),
			tempLoginUser.m_strLoginName,
			tempLoginUser.m_strLoginPassWd,
			tempLoginUser.m_PermissionLevel,
			tempLoginUser.m_IsAutoLogin,
			tempLoginUser.m_IsRememberPasswd,
			Id);
		break;
  
	case CDataProvider::tbDevicePara:
		for (i = 0; i < m_vectDevicePara.size();i++)
		{
			if (Id==m_vectDevicePara[i].m_Id)
			{
				tempDevicePara = m_vectDevicePara[i];
				break;
			}

		}
		tempDevicePara.m_ProductionLineId = FindProLineId(tempDevicePara.m_strProductionLineName);
		tempDevicePara.m_ProcessModuleId = FindProModuleId(tempDevicePara.m_strProductionLineName, tempDevicePara.m_strProcessModuleName);
		tempDevicePara.m_DeviceId = FindDeviceId(tempDevicePara.m_strProductionLineName, tempDevicePara.m_strProcessModuleName, tempDevicePara.m_strDeviceName);
		tempDevicePara.m_PlcId = FindPlcId(tempDevicePara.m_strPlcName);
		m_vectDevicePara[i] = tempDevicePara;

		strsql.Format(_T("UPDATE tbDevicePara SET LastUpdatedDateTime=getdate(), ProductionLineId='%d',ProcessModuleId='%d',PLCId='%d',DeviceId='%d',ProductionLineName='%s',ProcessModuleName='%s',PlcName='%s',DeviceName='%s',ParaName='%s',ControlValue='%d',StateValue='%d',ControlAddrIndex='%s',StateAddrIndex='%s',Description='%s' WHERE Id='%d'"),
			tempDevicePara.m_ProductionLineId,
			tempDevicePara.m_ProcessModuleId,
			tempDevicePara.m_PlcId,
			tempDevicePara.m_DeviceId,
			tempDevicePara.m_strProductionLineName,
			tempDevicePara.m_strProcessModuleName,
			tempDevicePara.m_strPlcName,
			tempDevicePara.m_strDeviceName,
			tempDevicePara.m_strParaName,
			tempDevicePara.m_ControlValue,
			tempDevicePara.m_StateValue,
			tempDevicePara.m_strControlAddrIndex,
			tempDevicePara.m_strStateAddrIndex,
			tempDevicePara.m_strDescription,
			Id);
		break;

	case CDataProvider::tbFaultPara:
		for (i = 0; i < m_vectFaultPara.size(); i++)
		{
			if (Id == m_vectFaultPara[i].m_Id)
			{
				tempFaultPara = m_vectFaultPara[i];
				break;
			}

		}
		tempFaultPara.m_ProductionLineId = FindProLineId(tempFaultPara.m_strProductionLineName);
		tempFaultPara.m_ProcessModuleId = FindProModuleId(tempFaultPara.m_strProductionLineName, tempFaultPara.m_strProcessName);
		tempFaultPara.m_DeviceId = FindDeviceId(tempDevicePara.m_strProductionLineName, tempFaultPara.m_strProcessName, tempFaultPara.m_strDeviceName);
		tempFaultPara.m_PLCId = FindPlcId(tempFaultPara.m_strPlcName);

		m_vectFaultPara[i] = tempFaultPara;

		strsql.Format(_T("UPDATE tbFaultPara SET LastUpdatedDateTime=getdate(), ProductionLineId='%d',ProcessModuleId='%d',PLCId='%d',DeviceId='%d',ProductionLineName='%s',ProcessModuleName='%s',PlcName='%s',DeviceName='%s',ParaName='%s',ParaValue='%d',AddressIndex='%s',Description='%s' WHERE Id='%d'"),
			tempFaultPara.m_ProductionLineId,
			tempFaultPara.m_ProcessModuleId,
			tempFaultPara.m_PLCId,
			tempFaultPara.m_DeviceId,
			tempFaultPara.m_strProductionLineName,
			tempFaultPara.m_strProcessName,
			tempFaultPara.m_strPlcName,
			tempFaultPara.m_strDeviceName,
			tempFaultPara.m_strParaName,
			tempFaultPara.m_ParaValue,
			tempFaultPara.m_strAddressIndex,
			tempFaultPara.m_strDescription,
			Id);
		break;
	case tbProcessPara:
		for (i = 0; i < m_vectProModulePara.size();i++)
		{
			if (Id==m_vectProModulePara[i].m_Id)
			{
				tempProcessPara = m_vectProModulePara[i];
				break;
			}
		}
		tempProcessPara.m_ProductionLineId = FindProLineId(tempProcessPara.m_strProductionLineName);
		tempProcessPara.m_ProcessModuleId = FindProModuleId(tempProcessPara.m_strProductionLineName, tempProcessPara.m_strProcessModuleName);
		tempProcessPara.m_PlcId = FindPlcId(tempProcessPara.m_strPlcName);
		m_vectProModulePara[i] = tempProcessPara;

		strsql.Format(_T("UPDATE tbProcessPara SET LastUpdatedDateTime=getdate(), ProductionLineId='%d',ProcessModuleId='%d',PLCId='%d',ProductionLineName='%s',ProcessModuleName='%s',PlcName='%s',ParaName='%s',ParaValue='%f',IsConfig='%d',IsVisible='%d',ReadAddressIndex='%s',WriteAddressIndex='%s',Units='%s',ValueType='%d',AddressType='%s',Description='%s',IsRecord=%d WHERE Id='%d'"),
			tempProcessPara.m_ProductionLineId,
			tempProcessPara.m_ProcessModuleId,
			tempProcessPara.m_PlcId,
			tempProcessPara.m_strProductionLineName,
			tempProcessPara.m_strProcessModuleName,
			tempProcessPara.m_strPlcName,
			tempProcessPara.m_strParaName,
			tempProcessPara.m_ParaValue,
			tempProcessPara.m_IsConfig,
			tempProcessPara.m_IsVisible,
			tempProcessPara.m_strReadAddrIndex,
			tempProcessPara.m_strWriteAddrIndex,
			tempProcessPara.m_strUnit,
			tempProcessPara.m_ValueType,
			tempProcessPara.m_strAddressType,
			tempProcessPara.m_strDescription,
			tempProcessPara.m_IsRecord,
			Id);	
		break;

	case tbStatePara:
		for (i = 0; i < m_vectStatePara.size(); i++)
		{
			if (Id == m_vectStatePara[i].m_Id)
			{
				tempStatePara = m_vectStatePara[i];
				break;
			}
		}
		tempStatePara.m_ProductionLineId = FindProLineId(tempStatePara.m_strProductionLineName);
		tempStatePara.m_ProcessModuleId = FindProModuleId(tempStatePara.m_strProductionLineName, tempStatePara.m_strProcessModuleName);
		tempStatePara.m_PlcId = FindPlcId(tempStatePara.m_strPlcName);
		
		m_vectStatePara[i] = tempStatePara;

		strsql.Format(_T("UPDATE tbStatePara SET LastUpdatedDateTime=getdate(), ProductionLineId='%d',ProcessModuleId='%d',PLCId='%d',ProductionLineName='%s',ProcessModuleName='%s',PlcName='%s',ParaName='%s',ParaValue='%d',AddressIndex='%s',Description='%s' WHERE Id='%d'"),
			tempStatePara.m_ProductionLineId,
			tempStatePara.m_ProcessModuleId,
			tempStatePara.m_PlcId,
			tempStatePara.m_strProductionLineName,
			tempStatePara.m_strProcessModuleName,
			tempStatePara.m_strPlcName,
			tempStatePara.m_strParaName,
			tempStatePara.m_ParaValue,
			tempStatePara.m_strAddressIndex,
			tempStatePara.m_strDescription,
			Id);
		break;
	case tbFormula:
		for (i = 0; i < m_vectFormula.size(); i++)
		{
			if (Id == m_vectFormula[i].m_Id)
			{
				tempFormula = m_vectFormula[i];
				break;
			}
		}
		tempFormula.m_ProductionLineId = FindProLineId(tempFormula.m_strProductionLineName);

		tempFormula.m_FormulaId = FindFormulaId(tempFormula.m_strFormulaName);
		//tempFormula.m_strProcessParaName = FindProcessParaName(tempFormula.m_ProcessParaId);

		m_vectFormula[i] = tempFormula;

		strsql.Format(_T("UPDATE tbFormula SET LastUpdatedDateTime=getdate(), ProductionLineId='%d',ProductionLineName='%s',FormulaName='%s',FormulaId='%d',ProcessParaId='%d',ProcessParaName='%s',ParaValueUnit='%s',Note='%s',DefaultValue='%f',IsDefaultFormula='%d',IsCurrentFormula='%d'WHERE Id='%d'"),
			tempFormula.m_ProductionLineId,	
			tempFormula.m_strProductionLineName,
			tempFormula.m_strFormulaName,
			tempFormula.m_FormulaId,
			tempFormula.m_ProcessParaId,
			tempFormula.m_strProcessParaName,
			tempFormula.m_strParaValueUnit,
			tempFormula.m_strNote,
			tempFormula.m_DefaultValue,
			tempFormula.m_IsDefaultFormula,
			tempFormula.m_IsCurrentFormula,
			Id);
		break;
	default:
		break;
	}

	ExecutionSQL(strsql);
	return 0;
}



int CDataProvider::DeleteModule(CString ProductionLineName){

	for (pModuleIter = m_vectProcessModule.begin();
		pModuleIter != m_vectProcessModule.end();)
	{
		if (pModuleIter->m_strProductionLineName == ProductionLineName)
		{
			//删除内存容器里面的数据//
			pModuleIter = m_vectProcessModule.erase(pModuleIter);
		}
		else
		{
			pModuleIter++;
		}

	}

	CString strsql;
	strsql.Format(_T("DELETE FROM tbProcessModule WHERE ProductionLineName='%s' "), ProductionLineName);

	ExecutionSQL(strsql);

	return 0;
}


int CDataProvider::DeleteDevice(CString ProductionLineName, CString ModuleName){

	for (pDeviceIter = m_vectDevice.begin();
		pDeviceIter != m_vectDevice.end();
		)
	{
		if (pDeviceIter->m_strProductionLineName == ProductionLineName
			&& (ModuleName.IsEmpty() || pDeviceIter->m_strProcessModuleName == ModuleName))
		{
			//删除内存容器里面的数据//
			pDeviceIter = m_vectDevice.erase(pDeviceIter);
		}
		else{
			pDeviceIter++;
		}

	}

	//删除数据库里面的数据//
	CString strsql;
	strsql.Format(_T("DELETE FROM tbDevice WHERE ProductionLineName='%s'AND ProcessModuleName='%s'"), ProductionLineName, ModuleName);

	ExecutionSQL(strsql);
	return 0;
}


int CDataProvider::DeletePlc(CString ProductionLineName){

	for (pPlcIter = m_vectPlc.begin();
		pPlcIter != m_vectPlc.end();
		)
	{
		if (pPlcIter->m_strProductionLineName == ProductionLineName)
		{		
			//删除内存容器里面的数据
			pPlcIter = m_vectPlc.erase(pPlcIter);
		}
		else
		{
			pPlcIter++;
		}

	}
	//删除数据库里面的数据//
	CString strsql;
	strsql.Format(_T("DELETE FROM tbPLc WHERE ProductionLineName='%s' "),ProductionLineName);

	ExecutionSQL(strsql);

	return 0;
}

int CDataProvider::DeleteVideo(CString ProductionLineName, CString ModuleName)
{

	for (pVideoIter = m_vectVideo.begin();
		pVideoIter != m_vectVideo.end();
		)
	{
		if (pVideoIter->m_strProductionLineName == ProductionLineName
			&& (ModuleName.IsEmpty() || pVideoIter->m_strProcessModuleName == ModuleName))
		{
			//删除内存容器里面的数据//
			pVideoIter = m_vectVideo.erase(pVideoIter);
		}
		else
		{
			pVideoIter++;
		}
	}

	//删除数据库里面的数据
	CString strsql;
	strsql.Format(_T("DELETE FROM tbVideo WHERE ProductionLineName='%s' AND  ProcessModuleName='%s'"), ProductionLineName, ModuleName);

	ExecutionSQL(strsql);

	return 0;
}


int CDataProvider::DeleteParaRelatedToLine(int ProductionLineId)
{
	//删除相关的设备参数//
	for (pDeviceParaIter = m_vectDevicePara.begin(); pDeviceParaIter != m_vectDevicePara.end();)
	{
		if (pDeviceParaIter->m_ProductionLineId==ProductionLineId)
		{
			pDeviceParaIter = m_vectDevicePara.erase(pDeviceParaIter);
		}
		else
		{
			++pDeviceParaIter;
		}
	}
	CString strsql;
	strsql.Format(_T("DELETE FROM tbDevicePara WHERE ProductionLineId='%d'"), ProductionLineId);
	ExecutionSQL(strsql);

	//删除状态参数相关//
	for (pStateParaIter = m_vectStatePara.begin(); pStateParaIter != m_vectStatePara.end();)
	{
		if (pStateParaIter->m_ProductionLineId == ProductionLineId)
		{
			pStateParaIter = m_vectStatePara.erase(pStateParaIter);
		}
		else
		{
			++pStateParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbStatePara WHERE ProductionLineId='%d'"), ProductionLineId);
	ExecutionSQL(strsql);

	//删除相关的工艺参数//
	for (pProcessParaIter = m_vectProModulePara.begin(); pProcessParaIter != m_vectProModulePara.end();)
	{
		if (pProcessParaIter->m_ProductionLineId == ProductionLineId)
		{
			pProcessParaIter = m_vectProModulePara.erase(pProcessParaIter);
		}
		else
		{
			++pProcessParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbProcessPara WHERE ProductionLineId='%d'"), ProductionLineId);
	ExecutionSQL(strsql);
	
	//删除相关的故障参数//
	for (pFaultParaIter = m_vectFaultPara.begin(); pFaultParaIter != m_vectFaultPara.end();)
	{
		if (pFaultParaIter->m_ProductionLineId == ProductionLineId)
		{
			pFaultParaIter = m_vectFaultPara.erase(pFaultParaIter);
		}
		else
		{
			++pFaultParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbFaultPara WHERE ProductionLineId='%d'"), ProductionLineId);
	ExecutionSQL(strsql);


	return 0;
}

//根据提供的工艺模块的ID,删除与工艺模块相关的所有参数//
int CDataProvider::DeleteParaRelatedToModule(int ModuleId)
{
	//删除相关的设备参数//
	for (pDeviceParaIter = m_vectDevicePara.begin(); pDeviceParaIter != m_vectDevicePara.end();)
	{
		if (pDeviceParaIter->m_ProcessModuleId == ModuleId)
		{
			pDeviceParaIter = m_vectDevicePara.erase(pDeviceParaIter);
		}
		else
		{
			++pDeviceParaIter;
		}
	}
	CString strsql;
	strsql.Format(_T("DELETE FROM tbDevicePara WHERE ProcessModuleId='%d'"), ModuleId);
	ExecutionSQL(strsql);

	//删除状态参数相关//
	for (pStateParaIter = m_vectStatePara.begin(); pStateParaIter != m_vectStatePara.end();)
	{
		if (pStateParaIter->m_ProcessModuleId == ModuleId)
		{
			pStateParaIter = m_vectStatePara.erase(pStateParaIter);
		}
		else
		{
			++pStateParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbStatePara WHERE ProcessModuleId='%d'"), ModuleId);
	ExecutionSQL(strsql);

	//删除相关的工艺参数//
	for (pProcessParaIter = m_vectProModulePara.begin(); pProcessParaIter != m_vectProModulePara.end();)
	{
		if (pProcessParaIter->m_ProcessModuleId == ModuleId)
		{
			pProcessParaIter = m_vectProModulePara.erase(pProcessParaIter);
		}
		else
		{
			++pProcessParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbProcessPara WHERE ProcessModuleId='%d'"), ModuleId);
	ExecutionSQL(strsql);

	//删除相关的故障参数//
	for (pFaultParaIter = m_vectFaultPara.begin(); pFaultParaIter != m_vectFaultPara.end();)
	{
		if (pFaultParaIter->m_ProcessModuleId == ModuleId)
		{
			pFaultParaIter = m_vectFaultPara.erase(pFaultParaIter);
		}
		else
		{
			++pFaultParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbFaultPara WHERE ProcessModuleId='%d'"), ModuleId);
	ExecutionSQL(strsql);


	return 0;
}




//根据提供的设备的ID,删除与设备相关的所有参数//
int CDataProvider::DeleteParaRelatedToDevice(int DeviceId)
{
	//删除相关的设备参数//
	for (pDeviceParaIter = m_vectDevicePara.begin(); pDeviceParaIter != m_vectDevicePara.end();)
	{
		if (pDeviceParaIter->m_DeviceId == DeviceId)
		{
			pDeviceParaIter = m_vectDevicePara.erase(pDeviceParaIter);
		}
		else
		{
			++pDeviceParaIter;
		}
	}
	CString strsql;
	strsql.Format(_T("DELETE FROM tbDevicePara WHERE DeviceId='%d'"), DeviceId);
	ExecutionSQL(strsql);

	//删除相关的故障参数//
	for (pFaultParaIter = m_vectFaultPara.begin(); pFaultParaIter != m_vectFaultPara.end();)
	{
		if (pFaultParaIter->m_ProcessModuleId == DeviceId)
		{
			pFaultParaIter = m_vectFaultPara.erase(pFaultParaIter);
		}
		else
		{
			++pFaultParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbFaultPara WHERE DeviceId='%d'"), DeviceId);
	ExecutionSQL(strsql);

	return 0;
}


int CDataProvider::DeleteParaRelatedToPlc(int PlcId)
{
	//删除相关的设备参数//
	for (pDeviceParaIter = m_vectDevicePara.begin(); pDeviceParaIter != m_vectDevicePara.end();)
	{
		if (pDeviceParaIter->m_PlcId == PlcId)
		{
			pDeviceParaIter = m_vectDevicePara.erase(pDeviceParaIter);
		}
		else
		{
			++pDeviceParaIter;
		}
	}
	CString strsql;
	strsql.Format(_T("DELETE FROM tbDevicePara WHERE PLCId='%d'"), PlcId);
	ExecutionSQL(strsql);

	//删除状态参数相关//
	for (pStateParaIter = m_vectStatePara.begin(); pStateParaIter != m_vectStatePara.end();)
	{
		if (pStateParaIter->m_PlcId == PlcId)
		{
			pStateParaIter = m_vectStatePara.erase(pStateParaIter);
		}
		else
		{
			++pStateParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbStatePara WHERE PLCId='%d'"), PlcId);
	ExecutionSQL(strsql);

	//删除相关的工艺参数//
	for (pProcessParaIter = m_vectProModulePara.begin(); pProcessParaIter != m_vectProModulePara.end();)
	{
		if (pProcessParaIter->m_PlcId == PlcId)
		{
			pProcessParaIter = m_vectProModulePara.erase(pProcessParaIter);
		}
		else
		{
			++pProcessParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbProcessPara WHERE PLCId='%d'"), PlcId);
	ExecutionSQL(strsql);

	//删除相关的故障参数//
	for (pFaultParaIter = m_vectFaultPara.begin(); pFaultParaIter != m_vectFaultPara.end();)
	{
		if (pFaultParaIter->m_PLCId == PlcId)
		{
			pFaultParaIter = m_vectFaultPara.erase(pFaultParaIter);
		}
		else
		{
			++pFaultParaIter;
		}
	}
	strsql.Format(_T("DELETE FROM tbFaultPara WHERE PLCId='%d'"), PlcId);
	ExecutionSQL(strsql);

	return 0;
}


//根据提供的生产线的ID参数，删除整条生产线有关的配方
int CDataProvider::DeleteFormulaRelatedToLine(int ProductionLineID )
{
	//删除容器里面的数据//
	for (pFormulaIter = m_vectFormula.begin(); pFormulaIter != m_vectFormula.end();)
	{
		if (ProductionLineID == pFormulaIter->m_ProductionLineId)
		{
			pFormulaIter=m_vectFormula.erase(pFormulaIter);
		}
		else
		{
			++pFormulaIter;
		}
	}
	//删除数据库里面的数据//
	CString strsql;
	strsql.Format(_T("DELETE FROM tbFormula WHERE ProductionLineId='%d'"), ProductionLineID);
	ExecutionSQL(strsql);

	return 0;
}

//根据提供的配方的FormulaId参数，删除整个配方//
int CDataProvider::DeleteFormula(int FormulaId)
{	
	for (pFormulaIter = m_vectFormula.begin(); pFormulaIter != m_vectFormula.end();)
	{
		if (FormulaId == pFormulaIter->m_FormulaId)
		{
			pFormulaIter=m_vectFormula.erase(pFormulaIter);
		}
		else
		{
			++pFormulaIter;
		}
	}

	CString strsql;
	strsql.Format(_T("DELETE FROM tbFormula WHERE FormulaId='%d'"), FormulaId);

	ExecutionSQL(strsql);
	return 0;
}



/*  根据传入的参数 	ExecutionSQL(strsql); ModuleName，
   从m_vectDevice容器中找出相关的Device
   存储到成员变量 m_vTempDevice中

   */
int CDataProvider::SearchDevice(CString ProductionLineName, CString ModuleName)
{
	m_vTempDevice.clear();
	for (pDeviceIter = m_vectDevice.begin(); pDeviceIter != m_vectDevice.end(); pDeviceIter++)
	{
		if (pDeviceIter->m_strProductionLineName==ProductionLineName
			&&pDeviceIter->m_strProcessModuleName==ModuleName)
		{
			m_vTempDevice.push_back(*pDeviceIter);
		}

	}
	return 0;
}




int CDataProvider::SearchVideo(CString ProductionLineName, CString ModuleName)
{
	m_vTempVideo.clear();
	for (pVideoIter = m_vectVideo.begin(); pVideoIter != m_vectVideo.end(); pVideoIter++)
	{
		if (pVideoIter->m_strProductionLineName == ProductionLineName
			&&pVideoIter->m_strProcessModuleName == ModuleName)
		{
			m_vTempVideo.push_back(*pVideoIter);
		}
	}
	return 0;
}


/*修改与生产线相关的模块，设备，摄像头,工艺参数，设备参数，故障参数，模块参数，状态参数中存储的所属生产线名
   参数说明:int LineId,修改的生产线的ID,
			CString modifyLineName ,将生产线名修改成modifyLineName

*/
int CDataProvider::UpdateRelatedToLine(int LineId, CString modifyLineName)
{
	CString strsql;

	for (int i = 0; i < m_vectProcessModule.size();i++)
	{
		if (m_vectProcessModule[i].m_ProcessLineId == LineId)
		{
			m_vectProcessModule[i].m_strProductionLineName = modifyLineName;			
		}
	}
	//更新数据库//
	strsql.Format(_T("UPDATE tbProcessModule SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);


	for (int i = 0; i < m_vectDevice.size();i++)
	{
		if (m_vectDevice[i].m_ProductionLineId==LineId)
		{
			m_vectDevice[i].m_strProductionLineName = modifyLineName;
		}
	}

	//更新数据库//
	strsql.Format(_T("UPDATE tbDevice SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);

	//修改摄像头
	for (int i = 0; i < m_vectVideo.size();i++)
	{
		if (m_vectVideo[i].m_ProductionLineId==LineId)
		{
			m_vectVideo[i].m_strProductionLineName = modifyLineName;
		}
	}
	//更新数据库//
	strsql.Format(_T("UPDATE tbVideo SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);


	//修改工艺参数//
	for (int i = 0; i < m_vectProModulePara.size();i++)
	{

		if (m_vectProModulePara[i].m_ProductionLineId == LineId)
		{
			m_vectProModulePara[i].m_strProductionLineName = modifyLineName;
		}
	}

	strsql.Format(_T("UPDATE tbProcessPara SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);


	//修改故障参数//
	for (int i = 0; i < m_vectFaultPara.size(); i++)
	{

		if (m_vectFaultPara[i].m_ProductionLineId == LineId)
		{
			m_vectFaultPara[i].m_strProductionLineName = modifyLineName;
		}
	}

	strsql.Format(_T("UPDATE tbFaultPara SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);

	//修改状态参数//
	for (int i = 0; i < m_vectStatePara.size(); i++)
	{

		if (m_vectStatePara[i].m_ProductionLineId == LineId)
		{
			m_vectStatePara[i].m_strProductionLineName = modifyLineName;
		}
	}

	strsql.Format(_T("UPDATE tbStatePara SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);


	//修改设备参数//
	for (int i = 0; i < m_vectDevicePara.size(); i++)
	{

		if (m_vectDevicePara[i].m_ProductionLineId == LineId)
		{
			m_vectDevicePara[i].m_strProductionLineName = modifyLineName;
		}
	}

	strsql.Format(_T("UPDATE tbDevicePara SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);
	

	//修改配方//
	for (int i = 0; i < m_vectFormula.size();i++)
	{
		if (m_vectFormula[i].m_ProductionLineId==LineId)
		{
			m_vectDevicePara[i].m_strProductionLineName = modifyLineName;
		}
	}
	strsql.Format(_T("UPDATE tbFormula SET LastUpdatedDateTime=getdate(), ProductionLineName='%s' WHERE ProductionLineId ='%d'"), modifyLineName, LineId);
	ExecutionSQL(strsql);

	return 0;
}


int CDataProvider::UpdateRelatedToModule(int ModuleId, CString modifyModuleName)
{		
	CString strsql;
	//修改设备//
	for (int i = 0; i < m_vectDevice.size(); i++)
	{
		if (m_vectDevice[i].m_ProcessModuleId == ModuleId)
		{
			m_vectDevice[i].m_strProcessModuleName = modifyModuleName;
		}
	}
	strsql.Format(_T("UPDATE tbDevice SET LastUpdatedDateTime=getdate(), ProcessModuleName='%s' WHERE ProcessModuleId ='%d'"), modifyModuleName, ModuleId);
	ExecutionSQL(strsql);


	//修改摄像头//
	for (int i = 0; i < m_vectVideo.size(); i++)
	{
		if (m_vectVideo[i].m_ModuleId == ModuleId)
		{
			m_vectVideo[i].m_strProcessModuleName = modifyModuleName;
			UpdateTableItem(tbVideo, m_vectVideo[i].m_Id);
		}
	}

	strsql.Format(_T("UPDATE tbVideo SET LastUpdatedDateTime=getdate(), ProcessModuleName='%s' WHERE ProcessModuleId ='%d'"), modifyModuleName, ModuleId);
	ExecutionSQL(strsql);

	//修改工艺参数//
	for (int i = 0; i < m_vectProModulePara.size(); i++)
	{

		if (m_vectProModulePara[i].m_ProcessModuleId == ModuleId)
		{
			m_vectProModulePara[i].m_strProcessModuleName = modifyModuleName;
		}
	}
	strsql.Format(_T("UPDATE tbProcessPara SET LastUpdatedDateTime=getdate(), ProcessModuleName='%s' WHERE ProcessModuleId ='%d'"), modifyModuleName, ModuleId);
	ExecutionSQL(strsql);


	//修改故障参数//
	for (int i = 0; i < m_vectFaultPara.size(); i++)
	{

		if (m_vectFaultPara[i].m_ProcessModuleId == ModuleId)
		{
			m_vectFaultPara[i].m_strProcessName = modifyModuleName;
		}
	}
	strsql.Format(_T("UPDATE tbFaultPara SET LastUpdatedDateTime=getdate(),ProcessModuleName='%s' WHERE ProcessModuleId ='%d'"), modifyModuleName, ModuleId);
	ExecutionSQL(strsql);

	//修改状态参数//
	for (int i = 0; i < m_vectStatePara.size(); i++)
	{

		if (m_vectStatePara[i].m_ProcessModuleId == ModuleId)
		{
			m_vectStatePara[i].m_strProcessModuleName = modifyModuleName;
		}
	}
	
	strsql.Format(_T("UPDATE tbStatePara SET LastUpdatedDateTime=getdate(), ProcessModuleName='%s' WHERE ProcessModuleId ='%d'"), modifyModuleName, ModuleId);
	ExecutionSQL(strsql);

	//修改设备参数//
	for (int i = 0; i < m_vectDevicePara.size(); i++)
	{

		if (m_vectDevicePara[i].m_ProcessModuleId == ModuleId)
		{
			m_vectDevicePara[i].m_strProcessModuleName = modifyModuleName;
		}
	}

	strsql.Format(_T("UPDATE tbDevicePara SET LastUpdatedDateTime=getdate(), ProcessModuleName='%s' WHERE ProcessModuleId ='%d'"), modifyModuleName, ModuleId);
	ExecutionSQL(strsql);

	return 0;
}


int CDataProvider::UpdataRelatedToDevice(int DeviceId, CString modifyDeviceName)
{
	CString strsql;
	//修改设备参数//
	for (int i = 0; i < m_vectDevicePara.size(); i++)
	{

		if (m_vectDevicePara[i].m_DeviceId == DeviceId)
		{
			m_vectDevicePara[i].m_strDeviceName = modifyDeviceName;
		}
	}

	strsql.Format(_T("UPDATE tbDevicePara SET LastUpdatedDateTime=getdate(), DeviceName='%s' WHERE DeviceId ='%d'"), modifyDeviceName, DeviceId);
	ExecutionSQL(strsql);

	//修改故障参数//
	for (int i = 0; i < m_vectFaultPara.size(); i++)
	{

		if (m_vectFaultPara[i].m_DeviceId == DeviceId)
		{
			m_vectFaultPara[i].m_strDeviceName = modifyDeviceName;
		}
	}
	strsql.Format(_T("UPDATE tbFaultPara SET LastUpdatedDateTime=getdate(), DeviceName='%s' WHERE DeviceId ='%d'"), modifyDeviceName, DeviceId);
	ExecutionSQL(strsql);

	return 0;
}

int CDataProvider::UpdateRelatedToPlc(int PlcId, CString modifyPlcName)
{
	CString strsql;
	//修改工艺参数//
	for (int i = 0; i < m_vectProModulePara.size(); i++)
	{

		if (m_vectProModulePara[i].m_PlcId == PlcId)
		{
			m_vectProModulePara[i].m_strPlcName = modifyPlcName;
		}
	}
	strsql.Format(_T("UPDATE tbProcessPara SET LastUpdatedDateTime=getdate(), PlcName='%s' WHERE PLCId ='%d'"), modifyPlcName, PlcId);
	ExecutionSQL(strsql);


	//修改故障参数//
	for (int i = 0; i < m_vectFaultPara.size(); i++)
	{

		if (m_vectFaultPara[i].m_PLCId == PlcId)
		{
			m_vectFaultPara[i].m_strPlcName = modifyPlcName;
		}
	}
	strsql.Format(_T("UPDATE tbFaultPara SET LastUpdatedDateTime=getdate(), PlcName='%s' WHERE PLCId ='%d'"), modifyPlcName, PlcId);
	ExecutionSQL(strsql);

	//修改状态参数//
	for (int i = 0; i < m_vectStatePara.size(); i++)
	{

		if (m_vectStatePara[i].m_PlcId == PlcId)
		{
			m_vectStatePara[i].m_strPlcName = modifyPlcName;
		}
	}

	strsql.Format(_T("UPDATE tbStatePara SET LastUpdatedDateTime=getdate(), PlcName='%s' WHERE PLCId ='%d'"), modifyPlcName, PlcId);
	ExecutionSQL(strsql);

	//修改设备参数//
	for (int i = 0; i < m_vectDevicePara.size(); i++)
	{

		if (m_vectDevicePara[i].m_PlcId == PlcId)
		{
			m_vectDevicePara[i].m_strPlcName = modifyPlcName;
		}
	}

	strsql.Format(_T("UPDATE tbDevicePara SET LastUpdatedDateTime=getdate(), PlcName='%s' WHERE PLCId ='%d'"), modifyPlcName, PlcId);
	ExecutionSQL(strsql);

	return 0;
}


void CDataProvider::SetDefaultConfig(int LineID, int ConfigID)
{
	if (!ConfigID)           //如果传入的配方ID为0，则使该生产线没有默认配方//
	{
		for (int x = 0; x < m_vectFormula.size();x++)
		{
			if (m_vectFormula[x].m_ProductionLineId == LineID && m_vectFormula[x].m_IsDefaultFormula != 0)
			{
				m_vectFormula[x].m_IsDefaultFormula = 0;
				UpdateTableItem(tbFormula, m_vectFormula[x].m_Id);
			}
		}
	}
	else                 //如果传入的是配方ID，则将它设为默认配方//
	{
		for (int x = 0; x < m_vectFormula.size(); x++)
		{
			if (m_vectFormula[x].m_ProductionLineId == LineID)
			{
				if ( m_vectFormula[x].m_FormulaId == ConfigID)
				{
					m_vectFormula[x].m_IsDefaultFormula = 1;
					UpdateTableItem(tbFormula, m_vectFormula[x].m_Id);
				}
				else
				{
					if (m_vectFormula[x].m_IsDefaultFormula != 0)
					{
						m_vectFormula[x].m_IsDefaultFormula = 0;
						UpdateTableItem(tbFormula, m_vectFormula[x].m_Id);
					}
				}
			}
		}
	}
}

//ConfigID为0时，清空当前使用配方；ConfigID为1时使用默认配方；ConfigID为其他值，则使用ConfigID指向的配方//
void CDataProvider::SetCurConfig(int LineID, int ConfigID) 
{
	int nDefaultConfigID = 0;   //当前生产线的默认配方的ID//
	int switchID = ConfigID;
	CPlcWriteRequest tempPlcWriteQueue;
	if (ConfigID==1)
	{
		for (int x = 0; x < m_vectFormula.size();x++)
			if (m_vectFormula[x].m_IsDefaultFormula == 1 && LineID == m_vectFormula[x].m_ProductionLineId)
			{
				nDefaultConfigID = m_vectFormula[x].m_FormulaId;
				break;
			}
		if (nDefaultConfigID == 0)   //如默认配方不存在，则不使用任何配方//
			switchID = 0;
		else
			switchID = nDefaultConfigID;
	}
	switch (switchID)
	{
	case 0:           //配方ID为0，则清空当前选择配方//
		for (int x = 0; x < m_vectFormula.size();x++)
			if (LineID==m_vectFormula[x].m_ProductionLineId && m_vectFormula[x].m_IsCurrentFormula==1)
			{
				m_vectFormula[x].m_IsCurrentFormula = 0;
				UpdateTableItem(tbFormula, m_vectFormula[x].m_Id);
			}
		break;
	default:                         //其他配方ID，则使用该ID指向的配方//
		for (int x = 0; x < m_vectFormula.size(); x++)
			if (m_vectFormula[x].m_ProductionLineId == LineID)
				if (m_vectFormula[x].m_FormulaId == switchID)
				{
					if (m_vectFormula[x].m_IsCurrentFormula != 1)
					{
						m_vectFormula[x].m_IsCurrentFormula = 1;
						UpdateTableItem(tbFormula, m_vectFormula[x].m_Id);
					}
					for (int y = 0; y < m_vectProModulePara.size(); y++)   //将配方的设定值写入PLC//
						if (m_vectProModulePara[y].m_Id == m_vectFormula[x].m_ProcessParaId)
						{
							switch (m_vectProModulePara[y].m_ValueType)   //根据数据类型赋值//
							{
							case 0:
								if (m_vectFormula[x].m_DefaultValue == 0)
									tempPlcWriteQueue.m_SetValue = 0;
								else
									tempPlcWriteQueue.m_SetValue = 1;
								break;
							case 1:
							case 2:
							case 3:
								tempPlcWriteQueue.m_SetValue = m_vectFormula[x].m_DefaultValue;
								break;
							case 4:
							case 5:
								tempPlcWriteQueue.m_fSetValue = m_vectFormula[x].m_DefaultValue;
								break;
							default:
								break;
							}
							tempPlcWriteQueue.m_PlcId = m_vectProModulePara[y].m_PlcId;
							tempPlcWriteQueue.m_AddrType = m_vectProModulePara[y].GetAddrTypeInEmType();
							tempPlcWriteQueue.m_ByteOffset = m_vectProModulePara[y].GetWriteBitOffSet();
							tempPlcWriteQueue.m_WriteByteAddrIndex = m_vectProModulePara[y].GetWriteAddrByteIndex();
							m_wRequestQueue.PushBack(tempPlcWriteQueue);
							break;
						}
				}
				else
					if (m_vectFormula[x].m_IsCurrentFormula == 1)
					{
						m_vectFormula[x].m_IsCurrentFormula = 0;
						UpdateTableItem(tbFormula, m_vectFormula[x].m_Id);
					}
		break;
	}
}

bool CDataProvider::ShowPicture(CString imgPath, CStatic &staticdlg, CImage &image)
{
	int height, width;
	CRect rect;//定义矩形类
	CRect rect1;
	 //创建图片类//
	image.Load(imgPath);
	height = image.GetHeight();
	width = image.GetWidth();
	staticdlg.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	CDC *pDc = staticdlg.GetDC();   //获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	float xScale = (float)rect.Width() / (float)width;
	float yScale = (float)rect.Height() / (float)height;
	rect1 = CRect(rect.TopLeft(), CSize((int)width*xScale, (int)height*yScale));
	image.StretchBlt(pDc->m_hDC, rect1, SRCCOPY); //将图片画到Picture控件表示的矩形区域
	staticdlg.ReleaseDC(pDc);//释放picture控件的Dc
	return TRUE;
}

void CDataProvider::DispatchParaValue(int PlcIndex)
{

	if (PlcIndex >= m_vectPlc.size())
	{
		return;
	}
	DispatchProPara(PlcIndex);
	DispatchDevicePara(PlcIndex);
	DispatchStatePara(PlcIndex);
	DispatchFaultPara(PlcIndex);
}


void CDataProvider::DispatchProPara(int PlcIndex)
{
	int  ParaValue = 0;
	int  AddrByte = 0;
	int  BitOffset = 0;
	//把PLC里面的参数写入到各条工艺参数
	for (int i = 0; i < m_vectProModulePara.size(); i++)
	{
		if (m_vectProModulePara[i].m_PlcId == m_vectPlc[PlcIndex].m_Id
			&& (m_vectPlc[PlcIndex].GetPlcReadMemorySize() > m_vectProModulePara[i].GetReadAddrIndex()))
		{

			switch (m_vectProModulePara[i].GetAddrTypeInEmType())
			{
			case CProcessPara::ADDR_TYPE_BIT:
				AddrByte = m_vectProModulePara[i].GetReadAddrByteIndex();
				BitOffset = m_vectProModulePara[i].GetReadBitOffSet();
				ParaValue = m_vectPlc[PlcIndex].GetReadMemoryBit(AddrByte, BitOffset);
				m_vectProModulePara[i].SetParaValue(ParaValue);

				//读出PLC里面的设定值，写入到预设值中
				AddrByte = m_vectProModulePara[i].GetWriteAddrByteIndex();
				BitOffset = m_vectProModulePara[i].GetWriteBitOffSet();
				ParaValue = m_vectPlc[PlcIndex].GetReadMemoryBit(AddrByte, BitOffset);
				m_vectProModulePara[i].SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_BYTE:
				AddrByte = m_vectProModulePara[i].GetReadAddrByteIndex();
				ParaValue = m_vectPlc[PlcIndex].GetReadMemoryByte(AddrByte);
				m_vectProModulePara[i].SetParaValue(ParaValue);

				AddrByte = m_vectProModulePara[i].GetWriteAddrByteIndex();
				ParaValue = m_vectPlc[PlcIndex].GetReadMemoryByte(AddrByte);
				m_vectProModulePara[i].SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_WORD:
				AddrByte = m_vectProModulePara[i].GetReadAddrByteIndex();
				ParaValue = m_vectPlc[PlcIndex].GetReadMemoryWord(AddrByte);
				m_vectProModulePara[i].SetParaValue(ParaValue);

				AddrByte = m_vectProModulePara[i].GetWriteAddrByteIndex();
				ParaValue = m_vectPlc[PlcIndex].GetReadMemoryWord(AddrByte);
				m_vectProModulePara[i].SetPreSetValue(ParaValue);

				break;
			case CProcessPara::ADDR_TYPE_DWORD:  //双字只有浮点型一种数据类型
				AddrByte = m_vectProModulePara[i].GetReadAddrByteIndex();
				float	fParaValue = m_vectPlc[PlcIndex].GetReadDWord(AddrByte);
				m_vectProModulePara[i].SetParaValue(fParaValue);

				AddrByte = m_vectProModulePara[i].GetWriteAddrByteIndex();
				fParaValue = m_vectPlc[PlcIndex].GetReadDWord(AddrByte);
				m_vectProModulePara[i].SetPreSetValue(fParaValue);
			}
		}
	}
}

void CDataProvider::DispatchDevicePara(int PlcIndex)
{
	int ParaValue = 0;
	for (int i = 0; i < m_vectDevicePara.size(); i++)
	{
		if (m_vectDevicePara[i].m_PlcId == m_vectPlc[PlcIndex].m_Id && (m_vectPlc[PlcIndex].GetPlcReadMemorySize() > m_vectDevicePara[i].GetControlAddrByteIndex()))
		{
			int ByteAddr = m_vectDevicePara[i].GetControlAddrByteIndex();
			int BitOffset = m_vectDevicePara[i].GetControlAddrBitOffSet();
			ParaValue = m_vectPlc[PlcIndex].GetReadMemoryBit(ByteAddr, BitOffset);
			m_vectDevicePara[i].SetControlPara(ParaValue);


			ByteAddr = m_vectDevicePara[i].GetStateAddrByteIndex();
			BitOffset = m_vectDevicePara[i].GetStateAddrBitOffSet();
			ParaValue = m_vectPlc[PlcIndex].GetReadMemoryBit(ByteAddr, BitOffset);
			m_vectDevicePara[i].SetStatePara(ParaValue);
		}
	}
}

void CDataProvider::DispatchFaultPara(int PlcIndex)
{
	int ParaValue;

	for (int i = 0; i < m_vectFaultPara.size(); i++)
	{

		if (m_vectFaultPara[i].m_PLCId == m_vectPlc[PlcIndex].m_Id && (m_vectPlc[PlcIndex].GetPlcReadMemorySize() > m_vectFaultPara[i].GetReadAddrByteIndex()))
		{
			int ByteAddr = m_vectFaultPara[i].GetReadAddrByteIndex();
			int BitOffset = m_vectFaultPara[i].GetAddrBitOffSet();
			ParaValue = m_vectPlc[PlcIndex].GetReadMemoryBit(ByteAddr, BitOffset);
			m_vectFaultPara[i].SetParaValue(ParaValue);
		}
	}
}

void CDataProvider::DispatchStatePara(int PlcIndex)
{
	int ParaValue;
	for (int i = 0; i < m_vectStatePara.size(); i++)
	{

		if (m_vectStatePara[i].m_PlcId == m_vectPlc[PlcIndex].m_Id && (m_vectPlc[PlcIndex].GetPlcReadMemorySize() > m_vectStatePara[i].GetReadAddrByteIndex()))
		{
			int ByteAddr = m_vectStatePara[i].GetReadAddrByteIndex();
			int BitOffset = m_vectStatePara[i].GetAddrBitOffSet();
			ParaValue = m_vectPlc[PlcIndex].GetReadMemoryBit(ByteAddr, BitOffset);
			m_vectStatePara[i].SetParaValue(ParaValue);
		}
	}
}


int CDataProvider::searchModuleParaType(int nModuleParaId) //根据工艺参数的ID确认他的值类型，并返回//
{
	for (int k = 0; k < m_vectProModulePara.size();k++)
		if (m_vectProModulePara[k].m_Id==nModuleParaId)
			return m_vectProModulePara[k].m_ValueType;
	return -1;
}




int CDataProvider::GetMaxProlineId()
{
	int maxId = 0;
	for (int i = 0; i < m_vectProductionLine.size();i++)
	{

		if (maxId<m_vectProductionLine[i].m_Id)
		{
			maxId = m_vectProductionLine[i].m_Id;
		}
	}

	return maxId;

}
int CDataProvider::GetMaxModuleId()
{
	int maxId = 0;
	for (int i = 0; i < m_vectProcessModule.size(); i++)
	{

		if (maxId < m_vectProcessModule[i].m_Id)
		{
			maxId = m_vectProcessModule[i].m_Id;
		}
	}

	return maxId;
}
int CDataProvider::GetMaxDeviceId()
{
	int maxId = 0;
	for (int i = 0; i < m_vectDevice.size(); i++)
	{

		if (maxId < m_vectDevice[i].m_Id)
		{
			maxId = m_vectDevice[i].m_Id;
		}
	}

	return maxId;

}
int CDataProvider::GetMaxPlcId()
{
	int maxId = 0;
	for (int i = 0; i < m_vectPlc.size(); i++)
	{

		if (maxId < m_vectPlc[i].m_Id)
		{
			maxId = m_vectPlc[i].m_Id;
		}
	}

	return maxId;

}
int CDataProvider::GetMaxVideoId()
{
	int maxId = 0;
	for (int i = 0; i < m_vectVideo.size(); i++)
	{

		if (maxId < m_vectVideo[i].m_Id)
		{
			maxId = m_vectVideo[i].m_Id;
		}
	}

	return maxId;
}


//2015-10-31添加参数历史记录功能//
void CDataProvider::ReadRecordTbIndex()
{
	CString strsql;
	strsql.Format(_T("select * from tbParaRecordIndex order by Id"));

	CtbParaRecordIndex tbRecordIndex; 
	try{
		if (tbRecordIndex.IsOpen())
			tbRecordIndex.Close();
		if (!tbRecordIndex.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbRecordIndex.IsBOF())
	{
		return;
	}


	m_vectParaRecordTbIndex.clear();

	CParaRecordIndex tempParaRecordTbIndex;
	tbRecordIndex.MoveFirst();
	while (!tbRecordIndex.IsEOF()){	
		tempParaRecordTbIndex.m_Id = tbRecordIndex.m_Id;
		tempParaRecordTbIndex.m_strCreatedTime = tbRecordIndex.m_CreatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		tempParaRecordTbIndex.m_strLastUpdateTime = tbRecordIndex.m_LastUpdatedDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		tempParaRecordTbIndex.m_strTbRecordName = tbRecordIndex.m_tbRecordName; //参数对应的表名//

		m_vectParaRecordTbIndex.push_back(tempParaRecordTbIndex);
		tbRecordIndex.MoveNext();
	}

	tbRecordIndex.Close();
}
void CDataProvider::AddRecordTbIndex(CParaRecordIndex& tempRecordIndex)
{
	CtbParaRecordIndex tbRecordIndex;
	try{
		if (tbRecordIndex.IsOpen())
			tbRecordIndex.Close();
		if (!tbRecordIndex.Open(CRecordset::dynaset)){
			AfxMessageBox(_T("打开数据库失败！"));
			return;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	int length = m_vectParaRecordTbIndex.size();
	if (length == 0)//设置第一个模块ID
	{
		tempRecordIndex.m_Id = 1;
	}
	else{
		tempRecordIndex.m_Id = m_vectParaRecordTbIndex[length - 1].m_Id + 1;
	}

	//创建的表名tbParaRecord+待记录参数的ID//
	tempRecordIndex.m_strTbRecordName.Format(_T("tbParaRecord%d"), tempRecordIndex.m_ProParaId);

	m_vectParaRecordTbIndex.push_back(tempRecordIndex);

	if (tbRecordIndex.CanUpdate()){
		tbRecordIndex.AddNew();
		CTime time = CTime::GetCurrentTime();
		tbRecordIndex.m_Id = tempRecordIndex.m_Id;
		tbRecordIndex.m_CreatedDateTime = time;
		tbRecordIndex.m_LastUpdatedDateTime = time;
		tbRecordIndex.m_ProParaId = tempRecordIndex.m_ProParaId;
		tbRecordIndex.m_tbRecordName = tempRecordIndex.m_strTbRecordName;
		tbRecordIndex.Update();
	}
	tbRecordIndex.Close();

	CreateParaRecordTb(tempRecordIndex); //创建记录参数对应的表//
}


void CDataProvider::DeleteRecordTbIndex(CParaRecordIndex& tempRecordIndex)
{
	CString strsql;
	strsql.Format(_T("DELETE FROM tbParaRecordIndex WHERE Id=%d"), tempRecordIndex.m_Id); 
	ExecutionSQL(strsql); //删除索引表的记录//

	DeleteParaRecordTb(tempRecordIndex); //删除表//

}
void CDataProvider::UpdateRecordTbIndex(CParaRecordIndex& tempRecordIndex)
{


}

//创建每条待记录参数对应的表//
BOOL CDataProvider::CreateParaRecordTb(CParaRecordIndex &RecordIndex)
{
	CString strsql;

	strsql.Format(_T("CREATE TABLE %s(Id int,strCreatedTime varchar(100),ProParaId int,ParaValue float)"), RecordIndex.m_strTbRecordName);
	if (ExecutionSQL(strsql))
	{
		return FALSE;
	}
	return TRUE; //执行成功//
}
BOOL CDataProvider::DeleteParaRecordTb(CParaRecordIndex &RecordIndex)
{
	CString strsql;
	strsql.Format(_T("DROP TABLE  %s"), RecordIndex.m_strTbRecordName);
	if (ExecutionSQL(strsql))
	{
		return FALSE;
	}
	return TRUE;

}
BOOL CDataProvider::AddParaReordToTb(CString& RecordTbName, CParaRecord& ParaRecord)
{

	CString strsql;
	
	strsql.Format(_T("INSERT INTO %s(Id, CreatedTime,ProParaId,ParaValue) VALUES('%s',getdate(),'%d')"), RecordTbName, ParaRecord.m_Id, ParaRecord.m_ProParaId,ParaRecord.m_fParaValue);

	ExecutionSQL(strsql);
	return TRUE;
}


void CDataProvider::ReadParaRecords(CString& tbRecordName)
{
	CtbParaRecord tbParaRecord;
	CString strsql;
	strsql.Format(_T("select * from %s order by Id"),tbRecordName);

	try{
		if (tbParaRecord.IsOpen())
			tbParaRecord.Close();
		if (!tbParaRecord.Open(CRecordset::dynaset, strsql)){
			AfxMessageBox(_T("打开数据库失败！"));
			return ;
		}
	}
	catch (CDBException *e){
		e->ReportError();
	}

	if (tbParaRecord.IsBOF())
	{
		return;
	}

	m_vParaRecordes.clear();

	CParaRecord tempParaRecord;
	tbParaRecord.MoveFirst();
	while (!tbParaRecord.IsEOF()){	
		tempParaRecord.m_Id = tbParaRecord.m_Id;
		tempParaRecord.m_CreateTime = tbParaRecord.m_CreatedTime;
		tempParaRecord.m_ProParaId = tbParaRecord.m_ProParaId;
		tempParaRecord.m_fParaValue = tbParaRecord.m_ParaValue;

		m_vParaRecordes.push_back(tempParaRecord);
		tbParaRecord.MoveNext();
	}

	tbParaRecord.Close();
}

