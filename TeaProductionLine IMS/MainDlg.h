
// MainDlg.h : 头文件
//

#pragma once
#include "UserManageDlg.h"
#include "InitDlg.h"
#include "EditPlcParaDlg.h"
#include "VedioChiDlg.h"
#include "DeviceChiDlg.h"
#include "DataProvider.h"
#include "ConfigChiDlg.h"
#include "afxwin.h"
#include "Weather.h"
#include "PlcStatusDlg.h"
#include "Modbus.h"
#include "AlarmDlg.h"
#include "AlarmClass.h"
#include "afxcmn.h"

class CTeaProductionLineIMSDlgAutoProxy;
#define ID_VEDIODLG 1
#define ID_DEVICEDLG 2
#define ID_CONFIGDLG 3
#define ID_TOTALPROCESSLINE 4

// CMainDlg 对话框
class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg);
	friend class CTeaProductionLineIMSDlgAutoProxy;

// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_MAINDLG };       //IDD_TEAPRODUCTIONLINEIMS_DIALOG

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	CTeaProductionLineIMSDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedBtUser();
	CUserManageDlg m_UserManageDlg;                     //用户管理界面//
	afx_msg void OnSysteminit();    
	afx_msg void OnPlcpara();       
	CInitDlg m_InitDlg;                                 //初始化界面//
	CEditPlcParaDlg m_EditPlcParaDlg;                   //PLC参数配置界面//
	
	CStatusBarCtrl m_StatBar;                           //状态栏//
	CVedioChiDlg m_VedioChiDlg;                         //视频监控子界面//
	CDeviceChiDlg m_DeviceChiDlg;                       //设备监控子界面//
	CConfigChiDlg m_ConfigChiDlg;                       //配方子界面//
	afx_msg void OnBnClickedBtVedio();
	int m_nCurChiDlg;                                    //标识当前主窗体内显示的子窗体，1标识视频监控，2表示设备监控，3配方，4生产总图//
	afx_msg void OnBnClickedBtDevice();
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();      //数据源//
	afx_msg void OnBnClickedBtConfig();
	afx_msg void OnDestroy();
	CComboBox m_CityComboBox;
	afx_msg void OnBnClickedUpdateWeather();
	CStatic m_textWeather;
	CWeather m_weather;	
	CLoginDlg m_LoginDlg;							//登录界面///
	CPlcStatusDlg m_PLCStatusDlg;                   //PLC状态监视界面//
	CModbus m_Modbus;                               //modbus连接变量
	CAlarmDlg m_AlarmDlg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnLogout();
	afx_msg void OnPlcstatus();
	
	CStatic m_pic;                     //加载LOGO的控件//
	bool m_bHaveInit;                  //是否已经初始化//
	bool m_bHaveRemenberOldSize;       //是否已经记住初始尺寸//
	std::vector<CRect> m_vectOldSize;  //存放各个控件的初始尺寸//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);
	POINT m_OldSize;                                    //窗体最初的尺寸//
	afx_msg void OnSize(UINT nType, int cx, int cy);    //窗体大小发生变化时的响应函数//
	afx_msg void OnAbout();
	afx_msg void OnBnClickedConnectPLC();
	afx_msg void OnBnClickedDisconnectPlc();
	bool m_bIsAlarmDlgShow;                             //警报界面是否正处于弹出状态//
private:	
	static UINT WeatherForcastThreadFunc(LPVOID lparam);               //天气预报线程函数//
public:
	afx_msg void OnBnClickedBtAlarm();
	void UpdateAlarm();  //警报//
	int m_nStateParaIndex;    //状态参数在数据源容器的下标//
	CListCtrl m_listState;
	void UpdateStatePara();   //更新状态参数//
	bool m_bIsWarningDlgShow;  //报警警告窗口是否显示//
	afx_msg void OnBnClickedBtHistroy();
};
