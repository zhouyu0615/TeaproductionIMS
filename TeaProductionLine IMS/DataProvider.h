#pragma once
#include <vector>
#include <map>

#include "UserClass.h"
#include "ProcessModuleClass.h"
#include "ProductionLineClass.h"
#include "PlcClass.h"
#include "DeviceClass.h"
#include "VideoClass.h"
#include "LoginUser.h"
#include "StatePara.h"
#include "FaultPara.h"
#include "DevicePara.h"
#include "ProcessModulePara.h"
#include "FormulaClass.h"
#include "tbFormula.h"
#include "atlimage.h"
#include "AlarmClass.h"
#include "RequestQueue.h"


class CDataProvider
{

private:
	CDataProvider();
	CDataProvider(const CDataProvider&);
	CDataProvider& operator=(const CDataProvider &);

	static CDataProvider *Instance;
	
	int ExecutionSQL(CString strSql);

public:
	
	static CDataProvider* getInstance();
	
	~CDataProvider();
	enum enumDBTABLE {	tbUser, tbProductionLine, tbProcessModule,
		tbDevice, tbPLc, tbVideo, tbLoginUser, tbFaultPara,
		tbProcessPara, tbStatePara, tbDevicePara, tbFormula	};
	std::vector<CLoginUser> m_vectLoginUser;
	std::vector<CAlarmClass> m_vectAlarmClass;         //警报信息//
	std::vector<CUserClass> m_vectUser;
	std::vector<CProductionLineClass> m_vectProductionLine;
	std::vector<CProcessModuleClass> m_vectProcessModule;
	std::vector<CDeviceClass>  m_vectDevice;
	std::vector<CPlcClass>  m_vectPlc;
	std::vector<CVideoClass> m_vectVideo;
	std::vector<CStatePara> m_vectStatePara;
	std::vector<CFaultPara> m_vectFaultPara;
	std::vector<CProcessPara> m_vectProModulePara;
	std::vector<CDevicePara>  m_vectDevicePara;
	std::vector<CFormulaClass> m_vectFormula;          //配方表//
	std::vector<CProcessPara> m_vectTempProModulePara; //临时工艺参数表//
	CRequestQueue m_wRequestQueue;                     //写入到PLC的请求队列
	std::vector<CProductionLineClass>::iterator pProlineIter;
	std::vector<CProcessModuleClass>::iterator  pModuleIter;
	std::vector<CDeviceClass>::iterator        pDeviceIter;
	std::vector<CPlcClass>::iterator            pPlcIter;
	std::vector<CVideoClass>::iterator		pVideoIter;	
	std::vector<CLoginUser>::iterator pLoginUserIter;
	std::vector<CStatePara>::iterator pStateParaIter;
	std::vector<CProcessPara>::iterator pProcessParaIter;
	std::vector<CFaultPara>::iterator pFaultParaIter;
	std::vector<CDevicePara>::iterator pDeviceParaIter;
	std::vector<CFormulaClass>::iterator pFormulaIter;
	void  InitDataProvider();
	void ReadLoginUserFormDatabase();
	void AddLoginUserToDatabase(CLoginUser tempLoginUser);
	void UpdateUserLoginTime(int LoginUserId);
	void AddUserToDatabase(CUserClass tempUser);
	void ReadUserFromDatabase();
	void AddProLineToDatabase(CProductionLineClass tempProLine);
	void ReadProLineFromDatabase();
	void AddProModuleToDatabase(CProcessModuleClass tempProModule);
	void ReadProModuleFromDatabase();
	void ReadDeviceFromDatabase();
	void AddDeviceToDatabase(CDeviceClass tempDevice);
	void ReadPlcFromDatabase();
	void AddPlcToDatabase(CPlcClass tempPlc);
	void ReadVideoFromDatabase();
	void AddVideoToDatabase(CVideoClass tempVideo);
	void ReadDeviceParaFromDatabase();
	void AddDeviceParaToDatabase(CDevicePara tempDevice);

	void ReadFaultParaFromDatabase();
	void AddFaultParaToDatabase(CFaultPara tempFaultPara);
	
	void ReadProcessParaFromDatabase();
	void AddProcessParaToDatabase(CProcessPara tempProcessPara);

	void ReadStateParaFromDatabase();
	void AddStateParaToDatabase(CStatePara tempStatePara);

	void ReadFormulaFormDatabase();
	void AddFormulaToDatabase(CFormulaClass tempFormula);


	int UpdateTableItem(enumDBTABLE dbTable, int Id);

	int DeleteDbTable(enumDBTABLE dbTable);

    int DeleteDbTableItem(enumDBTABLE dbTable, int Id);
	int DeleteFormulaRelatedToLine(int ProductionLineID);
	
	int DeleteFormula(int FormulaId);
	
	int DeleteModule(CString ProductionLineName);
	int DeleteDevice(CString ProductionLineName, CString ModuleName=_T(""));
	int DeletePlc(CString ProductionLineName);
	int DeleteVideo(CString ProductionLineName, CString ModuleName=_T(""));
	int DeleteParaRelatedToLine(int ProductionLineId);
	int DeleteParaRelatedToModule(int ModuleId);
	int DeleteParaRelatedToDevice(int DeviceId);
	int DeleteParaRelatedToPlc(int PlcId);

	//根据提供的生产线名，返回找到生产线的ID,查找失败则返回0
	int FindProLineId(CString ProducitonLineName);
	//根据提供的生产线名，模块名，返回找到的所属工艺模块的ID,查找失败则返回0
	int FindProModuleId(CString ProductionLineName, CString ModuleName);

	int FindPlcId(CString PlcName);
	int FindDeviceId(CString ProductionLineName, CString ModuleName, CString DeviceName);

	int FindFormulaId(CString FormulaName);
	int FindProcessParaId(CString ProcessPara);
	CString FindProcessParaName(int ProcessParaId);

	//作为Search Device,PlcPara,Video 方法临时存储的中间容器
	std::vector<CDeviceClass> m_vTempDevice;
	std::vector<CVideoClass> m_vTempVideo;

	int SearchDevice(CString ProductionLineName, CString ModuleName);
	int SearchVideo(CString ProducitonLinaName, CString ModuleName);


	int UpdateRelatedToLine(int LineId, CString modifyLineName);
	int UpdateRelatedToModule(int ModuleId, CString modifyModuleName);
	int UpdateRelatedToPlc(int PlcId, CString modifyPlcName);
	int UpdataRelatedToDevice(int DeviceId, CString modifyDeviceName);

	void SetDefaultConfig(int LineID,int ConfigID);       //为指定生产线设置默认配方//
	void SetCurConfig(int LineID, int ConfigID);          //为指定生产线设置正在使用的配方//
	bool ShowPicture(CString imgPath, CStatic &staticdlg, CImage &image);//显示图片//


	//从PLCMEMORY中，取出数据分发到各条工艺参数，设备参数等中//
	void DispatchParaValue(int PlcIndex);

	void DispatchProPara(int PlcIndex);
	void DispatchDevicePara(int PlcIndex);
	void DispatchFaultPara(int PlcIndex);
	void DispatchStatePara(int PlcIndex);
	int searchModuleParaType(int nModuleParaId);  //根据工艺参数的ID确认他的值类型，并返回//

	//从容器中找出当前最大ID//
	int GetMaxProlineId();
	int GetMaxModuleId();
	int GetMaxDeviceId();
	int GetMaxPlcId();
	int GetMaxVideoId();

};

