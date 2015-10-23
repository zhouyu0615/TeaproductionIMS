
// MainDlg.h : ͷ�ļ�
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

// CMainDlg �Ի���
class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg);
	friend class CTeaProductionLineIMSDlgAutoProxy;

// ����
public:
	CMainDlg(CWnd* pParent = NULL);	   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_MAINDLG };       //IDD_TEAPRODUCTIONLINEIMS_DIALOG

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	CTeaProductionLineIMSDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
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
	CUserManageDlg m_UserManageDlg;                     //�û��������//
	afx_msg void OnSysteminit();    
	afx_msg void OnPlcpara();       
	CInitDlg m_InitDlg;                                 //��ʼ������//
	CEditPlcParaDlg m_EditPlcParaDlg;                   //PLC�������ý���//
	
	CStatusBarCtrl m_StatBar;                           //״̬��//
	CVedioChiDlg m_VedioChiDlg;                         //��Ƶ����ӽ���//
	CDeviceChiDlg m_DeviceChiDlg;                       //�豸����ӽ���//
	CConfigChiDlg m_ConfigChiDlg;                       //�䷽�ӽ���//
	afx_msg void OnBnClickedBtVedio();
	int m_nCurChiDlg;                                    //��ʶ��ǰ����������ʾ���Ӵ��壬1��ʶ��Ƶ��أ�2��ʾ�豸��أ�3�䷽��4������ͼ//
	afx_msg void OnBnClickedBtDevice();
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();      //����Դ//
	afx_msg void OnBnClickedBtConfig();
	afx_msg void OnDestroy();
	CComboBox m_CityComboBox;
	afx_msg void OnBnClickedUpdateWeather();
	CStatic m_textWeather;
	CWeather m_weather;	
	CLoginDlg m_LoginDlg;							//��¼����///
	CPlcStatusDlg m_PLCStatusDlg;                   //PLC״̬���ӽ���//
	CModbus m_Modbus;                               //modbus���ӱ���
	CAlarmDlg m_AlarmDlg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnLogout();
	afx_msg void OnPlcstatus();
	
	CStatic m_pic;                     //����LOGO�Ŀؼ�//
	bool m_bHaveInit;                  //�Ƿ��Ѿ���ʼ��//
	bool m_bHaveRemenberOldSize;       //�Ƿ��Ѿ���ס��ʼ�ߴ�//
	std::vector<CRect> m_vectOldSize;  //��Ÿ����ؼ��ĳ�ʼ�ߴ�//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);
	POINT m_OldSize;                                    //��������ĳߴ�//
	afx_msg void OnSize(UINT nType, int cx, int cy);    //�����С�����仯ʱ����Ӧ����//
	afx_msg void OnAbout();
	afx_msg void OnBnClickedConnectPLC();
	afx_msg void OnBnClickedDisconnectPlc();
	bool m_bIsAlarmDlgShow;                             //���������Ƿ������ڵ���״̬//
private:	
	static UINT WeatherForcastThreadFunc(LPVOID lparam);               //����Ԥ���̺߳���//
public:
	afx_msg void OnBnClickedBtAlarm();
	void UpdateAlarm();  //����//
	int m_nStateParaIndex;    //״̬����������Դ�������±�//
	CListCtrl m_listState;
	void UpdateStatePara();   //����״̬����//
	bool m_bIsWarningDlgShow;  //�������洰���Ƿ���ʾ//
	afx_msg void OnBnClickedBtHistroy();
};
