
// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "MainDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CMainDlg �Ի���


IMPLEMENT_DYNAMIC(CMainDlg, CDialog);

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_nCurChiDlg = 1;                                 //��ʼ����ǰ��ʾ�Ӵ���ı�ʶ��//
	m_bHaveInit = false;
	m_bIsAlarmDlgShow = false;
	m_bHaveRemenberOldSize=false;       //�Ƿ��Ѿ���ס��ʼ�ߴ�//
	m_bIsWarningDlgShow = false;
	//��ȡ���ݿ⵽����Դ//
	m_pDataProvider->InitDataProvider();
}

CMainDlg::~CMainDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)  //��������ߵĵ�ǰʹ���䷽//
	{
		m_pDataProvider->SetCurConfig(m_pDataProvider->m_vectProductionLine[n].m_Id, 0);
	}
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CityComboBox);
	DDX_Control(pDX, IDC_STATIC_MAINDLG, m_pic);
	DDX_Control(pDX, IDC_STATIC_WEATHER, m_textWeather);
	DDX_Control(pDX, IDC_LISTSTATE, m_listState);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_USER, &CMainDlg::OnBnClickedBtUser)
	ON_COMMAND(IDM_SYSTEMINIT, &CMainDlg::OnSysteminit)
	ON_COMMAND(IDM_PLCPARA, &CMainDlg::OnPlcpara)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_VEDIO, &CMainDlg::OnBnClickedBtVedio)
	ON_BN_CLICKED(IDC_BT_DEVICE, &CMainDlg::OnBnClickedBtDevice)
	ON_BN_CLICKED(IDC_BT_CONFIG, &CMainDlg::OnBnClickedBtConfig)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedUpdateWeather)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDM_LOGOUT, &CMainDlg::OnLogout)
	ON_COMMAND(IDM_PLCSTATUS, &CMainDlg::OnPlcstatus)
	ON_COMMAND(IDM_ABOUT, &CMainDlg::OnAbout)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainDlg::OnBnClickedConnectPLC)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedDisconnectPlc)

	ON_BN_CLICKED(IDC_BT_ALARM, &CMainDlg::OnBnClickedBtAlarm)
	ON_BN_CLICKED(IDC_BT_HISTROY, &CMainDlg::OnBnClickedBtHistroy)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_COMMAND(IDM_UPLOAD, &CMainDlg::OnUploadData)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
{
	//�ж��Ƿ����û�ѡ�����Զ���¼������ֱ�ӵ�¼//
	bool bIsAnyAutoLoginUser = false;    //�Ƿ����û�ѡ���Զ���¼//
	for (int n = 0; n < m_pDataProvider->m_vectLoginUser.size(); n++)   //�����û�ѡ�����Զ���¼��������ֵ����ǰ�û�//
	{
		if (m_pDataProvider->m_vectLoginUser[n].m_IsAutoLogin)
		{
			CLoginDlg::LoginUser_CurrentId = m_pDataProvider->m_vectLoginUser[n].m_Id;
			CLoginDlg::LoginUser_CurrentPermissionLevel = m_pDataProvider->m_vectLoginUser[n].m_PermissionLevel;
			CLoginDlg::LoginUser_Name = m_pDataProvider->m_vectLoginUser[n].m_strLoginName;
			bIsAnyAutoLoginUser = true;
			break;
		}
	}
	if (!bIsAnyAutoLoginUser)
		m_LoginDlg.DoModal();				//���û���Զ���¼�˻����򵯳���¼����//

	//���״̬��//	
	CRect TempRect;
	GetClientRect(&TempRect);                                                    //ȡ�ͻ�����С//    
	TempRect.top = TempRect.bottom / 75;                                         //����״̬���ľ�������//
	m_StatBar.Create(WS_BORDER | WS_VISIBLE | CBRS_BOTTOM, TempRect, this, 3);
	int nParts[4] = { TempRect.right / 4.0, TempRect.right / 2.0, TempRect.right *0.75, -1 };                                       //�ָ�ߴ�//
	m_StatBar.SetParts(4, nParts);                                              //�ָ�״̬��//
	CString strLoginName;
	strLoginName.Format(_T("  ��ǰ�û��� %s"), CLoginDlg::LoginUser_Name);
	m_StatBar.SetText(strLoginName, 0, 0);                                   //��һ����������"״̬����"//
	m_StatBar.SetText(_T("  ����"), 1, 0);
	m_StatBar.ShowWindow(SW_SHOW);

	CRect ChiDlgRect;
	GetDlgItem(IDC_PIC_POSITION)->GetWindowRect(&ChiDlgRect);
	ScreenToClient(&ChiDlgRect);
	m_DeviceChiDlg.Create(IDD_DEVICE_CHIDLG, this);
	m_DeviceChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.Create(IDD_VEDIO_CHIDLG, this);
	m_VedioChiDlg.ShowWindow(SW_SHOW);
	m_ConfigChiDlg.Create(IDD_CONFIG_CHIDLG, this);
	m_ConfigChiDlg.ShowWindow(SW_HIDE);

	m_HistoryChiDlg.Create(IDD_HISTORY_CHIDLG, this);
	m_HistoryChiDlg.ShowWindow(SW_HIDE);

	CDialog::OnInitDialog();
	//����Ԥ��//
	m_CityComboBox.AddString(_T("��ɳ"));
	m_CityComboBox.AddString(_T("����"));
	m_CityComboBox.AddString(_T("����"));
	m_CityComboBox.SetCurSel(0);
	OnBnClickedUpdateWeather();        //ˢ������//
	//�༭�б��//
	CRect rectList11;
	m_listState.GetWindowRect(&rectList11);
	m_listState.SetExtendedStyle(m_listState.GetExtendedStyle()  | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE);
	m_listState.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listState.InsertColumn(1, _T("����"), LVCFMT_CENTER, rectList11.Width() / 8.0*5.0, -1);
	m_listState.InsertColumn(2, _T("״̬"), LVCFMT_CENTER, rectList11.Width() / 8.0*2.92, -1);
	for (int n = 0; n < m_pDataProvider->m_vectStatePara.size();n++)
	{
		m_listState.InsertItem(n, _T(""));
		m_listState.SetItemText(n, 1, m_pDataProvider->m_vectStatePara[n].m_strParaName);
		m_listState.SetItemText(n, 2, _T("δ����"));
	}

	m_bHaveInit = true;               //�Ƿ��Ѿ���ʼ��//
	if (!m_bHaveRemenberOldSize)
	{
		RememberOldSize(m_vectOldSize);
		CRect TempRect;
		GetClientRect(&TempRect);                                                    //ȡ�ͻ�����С//    
		m_OldSize.x = TempRect.right - TempRect.left;
		m_OldSize.y = TempRect.bottom - TempRect.top;
		m_bHaveRemenberOldSize = true;
	}

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	ShowWindow(SW_MAXIMIZE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	SetTimer(1, 1000, NULL);			//����ʱ����ʾ��ʱ�Ӻ���///
	SetTimer(0, 900000, NULL);          //����Ԥ��ˢ�µĶ�ʱ��//
	m_PLCStatusDlg.Create(IDD_PLCSTATUS, this);
	m_PLCStatusDlg.ShowWindow(SW_HIDE);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void CMainDlg::OnClose()
{
	OnBnClickedDisconnectPlc();

	if (CanExit())
		CDialog::OnClose();

	
}

void CMainDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CMainDlg::OnCancel()
{
	OnBnClickedDisconnectPlc();
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CMainDlg::CanExit()
{
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}
	return TRUE;
}


//���û�������ť��Ӧ����//
void CMainDlg::OnBnClickedBtUser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strLoginName;
	if (m_UserManageDlg.DoModal() == IDOK)           //�����û��������//
	{
		ShowWindow(SW_HIDE);
		CLoginDlg::LoginUser_CurrentPermissionLevel = 0;
		CLoginDlg::LoginUser_CurrentId = 0;
		CLoginDlg::LoginUser_Name = _T("");
		if (m_LoginDlg.DoModal() == IDOK)
		{
			strLoginName.Format(_T("  ��ǰ�û��� %s"), CLoginDlg::LoginUser_Name);
			m_StatBar.SetText(strLoginName, 0, 0);
			ShowWindow(SW_SHOW);
		}
	}
}


//�˵�����ϵͳ���á���ť��Ӧ����//
void CMainDlg::OnSysteminit()
{
	// TODO:  �ڴ���������������
	if (CLoginDlg::LoginUser_CurrentPermissionLevel == 1)
		m_InitDlg.DoModal();
	else
		AfxMessageBox(_T("��û��Ȩ�޽��иò�����"));
}


//PLC�������ð�ť��Ӧ����//
void CMainDlg::OnPlcpara()
{
	// TODO:  �ڴ���������������
	if (CLoginDlg::LoginUser_CurrentPermissionLevel == 1)
		m_EditPlcParaDlg.DoModal();
	else
		AfxMessageBox(_T("��û��Ȩ�޽��иò�����"));
}

//���巢����С�仯ʱ����Ӧ����//
void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		//ReSize();
		if (m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			CRect rectList11;
			m_listState.GetWindowRect(&rectList11);
			m_listState.SetColumnWidth(1, rectList11.Width() / 8.0*4.98);
			m_listState.SetColumnWidth(2, rectList11.Width() / 8.0*2.92);
			Invalidate();
		}
		if (m_bHaveInit)
		{
			CRect ChiDlgRect;
			GetDlgItem(IDC_PIC_POSITION)->GetWindowRect(ChiDlgRect);
			ScreenToClient(ChiDlgRect);
			m_DeviceChiDlg.SetWindowPos(&wndBottom, ChiDlgRect.left, ChiDlgRect.top, ChiDlgRect.right - ChiDlgRect.left, ChiDlgRect.bottom - ChiDlgRect.top, SWP_NOZORDER);
			m_VedioChiDlg.SetWindowPos(&wndBottom, ChiDlgRect.left, ChiDlgRect.top, ChiDlgRect.right - ChiDlgRect.left, ChiDlgRect.bottom - ChiDlgRect.top, SWP_NOZORDER);
			m_ConfigChiDlg.SetWindowPos(&wndBottom, ChiDlgRect.left, ChiDlgRect.top, ChiDlgRect.right - ChiDlgRect.left, ChiDlgRect.bottom - ChiDlgRect.top, SWP_NOZORDER);

			m_HistoryChiDlg.SetWindowPos(&wndBottom, ChiDlgRect.left, ChiDlgRect.top, ChiDlgRect.right - ChiDlgRect.left, ChiDlgRect.bottom - ChiDlgRect.top, SWP_NOZORDER);
		}
	}
}

//��������Ƶ��ء���ť����Ӧ����//
void CMainDlg::OnBnClickedBtVedio()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nCurChiDlg == ID_VEDIODLG)
		return;
	m_DeviceChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.ShowWindow(SW_SHOW);
	m_ConfigChiDlg.ShowWindow(SW_HIDE);

	m_HistoryChiDlg.ShowWindow(SW_HIDE);

	m_nCurChiDlg = ID_VEDIODLG;               //��ӦΪ��ǰ��ʾ�ӽ���ı�ʶ����ֵ//
	if (m_VedioChiDlg.IsVideoPlay==true)
	{
		m_VedioChiDlg.OnBnClickedBtPlay();
	}
}

//������豸��ء���ť����Ӧ����//
void CMainDlg::OnBnClickedBtDevice()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nCurChiDlg == ID_DEVICEDLG)  //�����ǰ��ʾ���ӽ����Ѿ����豸��ؽ��棬�����κ���Ӧ//
		return;	
	if (m_ConfigChiDlg.m_OldConfigGraChiDlg.m_bIsCurConfigChange)
	{
		m_ConfigChiDlg.m_OldConfigGraChiDlg.m_bIsCurConfigChange = false;
	}
	m_VedioChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.StopAllVideoPlay(); //�ر���Ƶ����//

	m_HistoryChiDlg.ShowWindow(SW_HIDE);

	m_DeviceChiDlg.ShowWindow(SW_SHOW);
	m_ConfigChiDlg.ShowWindow(SW_HIDE);
	m_nCurChiDlg = ID_DEVICEDLG;         //��ӦΪ��ǰ��ʾ�ӽ���ı�ʶ����ֵ//
}

//�䷽��ť����Ӧ����//
void CMainDlg::OnBnClickedBtConfig()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nCurChiDlg == ID_CONFIGDLG)
		return;
	m_ConfigChiDlg.ShowWindow(SW_SHOW);
	m_DeviceChiDlg.ShowWindow(SW_HIDE);

	m_VedioChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.StopAllVideoPlay();

	m_HistoryChiDlg.ShowWindow(SW_HIDE);
	m_nCurChiDlg = ID_CONFIGDLG;
}

//����������//
void CMainDlg::OnDestroy()
{
	OnBnClickedDisconnectPlc();

	m_VedioChiDlg.StopAllVideoPlay(); //�Ͽ���Ƶ����//
	CDialog::OnDestroy();
	// TODO:  �ڴ˴������Ϣ����������
}


void CMainDlg::OnBnClickedUpdateWeather()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWinThread *weatherThread;
	weatherThread = AfxBeginThread(WeatherForcastThreadFunc, this);
}

 UINT  CMainDlg::WeatherForcastThreadFunc(LPVOID lparam)
{
	CMainDlg *pmd = (CMainDlg*)lparam;

	CString strCity;
	pmd->m_CityComboBox.GetWindowText(strCity);
	pmd->m_weather.UpdateWeather(strCity);

	pmd->m_textWeather.SetWindowText(pmd->m_weather.m_strWeatherInfo);
	return 0;

}

//��ʱ������///
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		CString StrDate = CTime::GetCurrentTime().Format("%Y��%m��%d��");	//StrDateΪ��ǰ����///
		GetDlgItem(IDC_STATIC_DATE)->SetWindowText(StrDate);
		CString StrTime = CTime::GetCurrentTime().Format("%H:%M:%S");		//StrTimeΪ��ǰʱ��///
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(StrTime);
		if (!m_bIsAlarmDlgShow && !m_bIsWarningDlgShow)           //����������治����ʾ״̬//
		    UpdateAlarm();                      //��ر���//
		UpdateStatePara();                //����״̬��ʾ//
	}
	if (nIDEvent==0)
	{
		OnBnClickedUpdateWeather();   //ˢ��������15����һ��//
	}
	CDialog::OnTimer(nIDEvent);
}

//ʱ����ʾ�����ɫ����///
HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//�˵���ע����ť//
void CMainDlg::OnLogout()
{
	// TODO:  �ڴ���������������
	ShowWindow(SW_HIDE);
	CLoginDlg::LoginUser_CurrentPermissionLevel = 0;
	CLoginDlg::LoginUser_Name = _T("");
	CLoginDlg::LoginUser_CurrentId = 0;
	CString strLoginName;
	if (m_LoginDlg.DoModal() == IDCANCEL)
		PostQuitMessage(0);
	else
	{
		strLoginName.Format(_T("  ��ǰ�û��� %s"), CLoginDlg::LoginUser_Name);
		m_StatBar.SetText(strLoginName, 0, 0);
		ShowWindow(SW_SHOW);
	}
}

//�˵�������PLC״̬����//
void CMainDlg::OnPlcstatus()
{
	// TODO:  �ڴ���������������	
    m_PLCStatusDlg.ShowWindow(SW_SHOW);
	CRect WinRect,dlgRect;
	GetWindowRect(WinRect);
	m_PLCStatusDlg.GetWindowRect(dlgRect);
	ScreenToClient(WinRect);
	ScreenToClient(dlgRect);	
	WinRect.left = WinRect.right / 2 - (dlgRect.right-dlgRect.left)/2;
	WinRect.top = WinRect.bottom / 2-(dlgRect.bottom-dlgRect.top)/2;
	WinRect.right = WinRect.right / 2 + (dlgRect.right - dlgRect.left) / 2;
	WinRect.bottom = WinRect.bottom / 2 + (dlgRect.bottom - dlgRect.top) / 2;
	m_PLCStatusDlg.MoveWindow(WinRect);	
}


void CMainDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
{
	vectOldSize.clear();
	CRect Rect;
	int woc;
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�//    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //ȡ��ID//  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		vectOldSize.push_back(Rect);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

void CMainDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
{
	CRect Rect;
	int woc;
	CPoint  BRPoint, TLPoint;
	double fsp[2];                                    //���ű���//
	POINT recNewSize;                                 //��ȡ���ڶԻ���Ĵ�С//  
	CRect TempRect;
	GetClientRect(&TempRect);                         //ȡ�ͻ�����С//    
	recNewSize.x = TempRect.right - TempRect.left;
	recNewSize.y = TempRect.bottom - TempRect.top;
	fsp[0] = (double)recNewSize.x / OldSizeOfClient.x;
	fsp[1] = (double)recNewSize.y / OldSizeOfClient.y;

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�//    
	for (int n = 0; n < vectOldSize.size(); n++)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //ȡ��ID//  
		TLPoint.x = double(vectOldSize[n].left*fsp[0]);
		TLPoint.y = double(vectOldSize[n].top*fsp[1]);
		BRPoint.x = double(vectOldSize[n].right *fsp[0]);
		BRPoint.y = double(vectOldSize[n].bottom*fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}


void CMainDlg::OnAbout()
{
	// TODO:  �ڴ���������������
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
}


void CMainDlg::OnBnClickedConnectPLC()
{
	//����modbus.CycleStart()����PLC��ѯ����
	if (m_Modbus.m_bIsCycling == TRUE)
		return;
	m_Modbus.CycleStart();
	int defaultConfigId;
	CString strtemp;
	for (int a = 0; a < m_pDataProvider->m_vectProductionLine.size(); a++)  //Ϊÿ�������������䷽//
	{
		defaultConfigId = -1;
		for (int b = 0; b < m_pDataProvider->m_vectFormula.size();b++)
			if (m_pDataProvider->m_vectFormula[b].m_ProductionLineId == m_pDataProvider->m_vectProductionLine[a].m_Id && m_pDataProvider->m_vectFormula[b].m_IsDefaultFormula!=0)
			{
				defaultConfigId = m_pDataProvider->m_vectFormula[b].m_FormulaId;
				break;
			}
		if (defaultConfigId != -1)
		{
			//if (!m_ConfigChiDlg.m_OldConfigGraChiDlg.IsPlcConnected(defaultConfigId))
			//	continue;
			m_pDataProvider->SetCurConfig(m_pDataProvider->m_vectProductionLine[a].m_Id, 1);
			strtemp.Format(_T("%s��ʹ��Ĭ���䷽��"), m_pDataProvider->m_vectProductionLine[a].m_strLineName);
			AfxMessageBox(strtemp);
		}
	}
	m_ConfigChiDlg.m_OldConfigGraChiDlg.reInitList1();
}


void CMainDlg::OnBnClickedDisconnectPlc()
{
	if (m_Modbus.m_bIsCycling == TRUE)
		m_Modbus.CycleDisconnect();
}

void CMainDlg::OnBnClickedBtAlarm() //������ť//
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bIsAlarmDlgShow = true;
	m_AlarmDlg.DoModal();
	m_bIsAlarmDlgShow = false;
}

void CMainDlg::UpdateAlarm()  //��������//
{
	CString strErrorParaName;
	CString strTime;
	CAlarmClass tempAlarmClass;
	for (int x = 0; x < m_pDataProvider->m_vectFaultPara.size();x++)
	{
		if (m_pDataProvider->m_vectFaultPara[x].m_ParaValue != 0 && m_DeviceChiDlg.IsPlcConnected(m_pDataProvider->m_vectFaultPara[x].m_PLCId))
		{
			if (m_StatBar.GetText(1, 0) != _T("  ��δ����ı�����"))
				m_StatBar.SetText(_T("  ��δ����ı�����"), 1, 0);
			if (m_pDataProvider->m_vectFaultPara[x].m_nIndexInAlarmVect == -1) //����ò�����δ���뱨���б�//
			{
				tempAlarmClass.m_nErrorParaId = m_pDataProvider->m_vectFaultPara[x].m_Id;
				tempAlarmClass.m_nPlcId = m_pDataProvider->m_vectFaultPara[x].m_PLCId;
				tempAlarmClass.m_strAdressIndex = m_pDataProvider->m_vectFaultPara[x].m_strAddressIndex;
				tempAlarmClass.m_strAlarmName = m_pDataProvider->m_vectFaultPara[x].m_strParaName;
				strTime = CTime::GetCurrentTime().Format("%m.%d  %H:%M:%S");
				tempAlarmClass.m_strAlarmTime = strTime;
				tempAlarmClass.m_strNote = m_pDataProvider->m_vectFaultPara[x].m_strDescription;
				tempAlarmClass.m_nState = 0;
				tempAlarmClass.m_nIndexInErrorParaVect = x;
				m_pDataProvider->m_vectAlarmClass.push_back(tempAlarmClass);
				m_pDataProvider->m_vectFaultPara[x].m_nIndexInAlarmVect = m_pDataProvider->m_vectAlarmClass.size() - 1;
			}
			if (!m_pDataProvider->m_vectFaultPara[x].m_bIsIgnore)
			{
				strErrorParaName.Format(_T("������%s!�Ƿ����ϴ���������񡱽����Ըñ�����"), m_pDataProvider->m_vectFaultPara[x].m_strParaName);
				m_bIsAlarmDlgShow = true;
				if (MessageBox(strErrorParaName, _T("����"), MB_ICONWARNING | MB_YESNO) == IDYES)
				{
					m_bIsAlarmDlgShow = true;
					m_AlarmDlg.DoModal();
					m_bIsAlarmDlgShow = false;
				}
				else
				{
					m_pDataProvider->m_vectFaultPara[x].m_bIsIgnore = true;
					m_pDataProvider->m_vectAlarmClass[m_pDataProvider->m_vectFaultPara[x].m_nIndexInAlarmVect].m_nState = 1;
				}
				m_bIsAlarmDlgShow = false;
			}
		}
		else
			if (m_StatBar.GetText(1, 0) != _T("  ����"))
				m_StatBar.SetText(_T("  ����"), 1, 0);
	}
}

void CMainDlg::UpdateStatePara()  //����״̬����//
{
	for (int y = 0; y < m_pDataProvider->m_vectStatePara.size();y++)
	{
		if (m_DeviceChiDlg.IsPlcConnected(m_pDataProvider->m_vectStatePara[y].m_PlcId))
		{
			switch (m_pDataProvider->m_vectStatePara[y].m_ParaValue)
			{
			case 0:
				if (m_listState.GetItemText(y, 2) != _T("ֹͣ"))
					m_listState.SetItemText(y, 2, _T("ֹͣ"));
				break;
			case 1:
				if (m_listState.GetItemText(y, 2) != _T("��������"))
					m_listState.SetItemText(y, 2, _T("��������"));
				break;
			default:
				break;
			}
		} 
		else
		{
			if (m_listState.GetItemText(y, 2) != _T("δ����"))
			    m_listState.SetItemText(y, 2, _T("δ����"));
		}
	}
}

void CMainDlg::OnBnClickedBtHistroy()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_nCurChiDlg == ID_HISTORYDLG)
		return;

	m_HistoryChiDlg.ShowWindow(SW_SHOW);

	m_ConfigChiDlg.ShowWindow(SW_HIDE);
	m_DeviceChiDlg.ShowWindow(SW_HIDE);

	m_VedioChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.StopAllVideoPlay();

	m_nCurChiDlg = ID_HISTORYDLG;


}


void CMainDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	m_HistoryChiDlg.StopRecordThread();

	CDialog::OnCancel();
}




//�����˵����ϴ�����ѡ��ص�����//
void CMainDlg::OnUploadData()
{
	
}
