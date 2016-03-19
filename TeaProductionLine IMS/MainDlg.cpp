
// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "MainDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMainDlg 对话框


IMPLEMENT_DYNAMIC(CMainDlg, CDialog);

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_nCurChiDlg = 1;                                 //初始化当前显示子窗体的标识符//
	m_bHaveInit = false;
	m_bIsAlarmDlgShow = false;
	m_bHaveRemenberOldSize=false;       //是否已经记住初始尺寸//
	m_bIsWarningDlgShow = false;
	//读取数据库到数据源//
	m_pDataProvider->InitDataProvider();
}

CMainDlg::~CMainDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)  //清空生产线的当前使用配方//
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


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	//判断是否有用户选择了自动登录，有则直接登录//
	bool bIsAnyAutoLoginUser = false;    //是否有用户选择自动登录//
	for (int n = 0; n < m_pDataProvider->m_vectLoginUser.size(); n++)   //若有用户选择了自动登录，则将它赋值给当前用户//
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
		m_LoginDlg.DoModal();				//如果没有自动登录账户，则弹出登录界面//

	//添加状态栏//	
	CRect TempRect;
	GetClientRect(&TempRect);                                                    //取客户区大小//    
	TempRect.top = TempRect.bottom / 75;                                         //设置状态栏的矩形区域//
	m_StatBar.Create(WS_BORDER | WS_VISIBLE | CBRS_BOTTOM, TempRect, this, 3);
	int nParts[4] = { TempRect.right / 4.0, TempRect.right / 2.0, TempRect.right *0.75, -1 };                                       //分割尺寸//
	m_StatBar.SetParts(4, nParts);                                              //分割状态栏//
	CString strLoginName;
	strLoginName.Format(_T("  当前用户： %s"), CLoginDlg::LoginUser_Name);
	m_StatBar.SetText(strLoginName, 0, 0);                                   //第一个分栏加入"状态参数"//
	m_StatBar.SetText(_T("  正常"), 1, 0);
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
	//天气预报//
	m_CityComboBox.AddString(_T("长沙"));
	m_CityComboBox.AddString(_T("北京"));
	m_CityComboBox.AddString(_T("深圳"));
	m_CityComboBox.SetCurSel(0);
	OnBnClickedUpdateWeather();        //刷新天气//
	//编辑列表框//
	CRect rectList11;
	m_listState.GetWindowRect(&rectList11);
	m_listState.SetExtendedStyle(m_listState.GetExtendedStyle()  | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE);
	m_listState.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listState.InsertColumn(1, _T("名称"), LVCFMT_CENTER, rectList11.Width() / 8.0*5.0, -1);
	m_listState.InsertColumn(2, _T("状态"), LVCFMT_CENTER, rectList11.Width() / 8.0*2.92, -1);
	for (int n = 0; n < m_pDataProvider->m_vectStatePara.size();n++)
	{
		m_listState.InsertItem(n, _T(""));
		m_listState.SetItemText(n, 1, m_pDataProvider->m_vectStatePara[n].m_strParaName);
		m_listState.SetItemText(n, 2, _T("未连接"));
	}

	m_bHaveInit = true;               //是否已经初始化//
	if (!m_bHaveRemenberOldSize)
	{
		RememberOldSize(m_vectOldSize);
		CRect TempRect;
		GetClientRect(&TempRect);                                                    //取客户区大小//    
		m_OldSize.x = TempRect.right - TempRect.left;
		m_OldSize.y = TempRect.bottom - TempRect.top;
		m_bHaveRemenberOldSize = true;
	}

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	ShowWindow(SW_MAXIMIZE);

	// TODO:  在此添加额外的初始化代码

	SetTimer(1, 1000, NULL);			//设置时间显示的时钟函数///
	SetTimer(0, 900000, NULL);          //天气预报刷新的定时器//
	m_PLCStatusDlg.Create(IDD_PLCSTATUS, this);
	m_PLCStatusDlg.ShowWindow(SW_HIDE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

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
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}
	return TRUE;
}


//“用户管理”按钮响应函数//
void CMainDlg::OnBnClickedBtUser()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strLoginName;
	if (m_UserManageDlg.DoModal() == IDOK)           //弹出用户管理界面//
	{
		ShowWindow(SW_HIDE);
		CLoginDlg::LoginUser_CurrentPermissionLevel = 0;
		CLoginDlg::LoginUser_CurrentId = 0;
		CLoginDlg::LoginUser_Name = _T("");
		if (m_LoginDlg.DoModal() == IDOK)
		{
			strLoginName.Format(_T("  当前用户： %s"), CLoginDlg::LoginUser_Name);
			m_StatBar.SetText(strLoginName, 0, 0);
			ShowWindow(SW_SHOW);
		}
	}
}


//菜单栏“系统配置”按钮响应函数//
void CMainDlg::OnSysteminit()
{
	// TODO:  在此添加命令处理程序代码
	if (CLoginDlg::LoginUser_CurrentPermissionLevel == 1)
		m_InitDlg.DoModal();
	else
		AfxMessageBox(_T("您没有权限进行该操作！"));
}


//PLC参数配置按钮响应函数//
void CMainDlg::OnPlcpara()
{
	// TODO:  在此添加命令处理程序代码
	if (CLoginDlg::LoginUser_CurrentPermissionLevel == 1)
		m_EditPlcParaDlg.DoModal();
	else
		AfxMessageBox(_T("您没有权限进行该操作！"));
}

//窗体发生大小变化时的响应函数//
void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
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

//单击“视频监控”按钮的响应函数//
void CMainDlg::OnBnClickedBtVedio()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nCurChiDlg == ID_VEDIODLG)
		return;
	m_DeviceChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.ShowWindow(SW_SHOW);
	m_ConfigChiDlg.ShowWindow(SW_HIDE);

	m_HistoryChiDlg.ShowWindow(SW_HIDE);

	m_nCurChiDlg = ID_VEDIODLG;               //相应为当前显示子界面的标识符赋值//
	if (m_VedioChiDlg.IsVideoPlay==true)
	{
		m_VedioChiDlg.OnBnClickedBtPlay();
	}
}

//点击“设备监控”按钮的响应函数//
void CMainDlg::OnBnClickedBtDevice()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nCurChiDlg == ID_DEVICEDLG)  //如果当前显示的子界面已经是设备监控界面，则不做任何响应//
		return;	
	if (m_ConfigChiDlg.m_OldConfigGraChiDlg.m_bIsCurConfigChange)
	{
		m_ConfigChiDlg.m_OldConfigGraChiDlg.m_bIsCurConfigChange = false;
	}
	m_VedioChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.StopAllVideoPlay(); //关闭视频连接//

	m_HistoryChiDlg.ShowWindow(SW_HIDE);

	m_DeviceChiDlg.ShowWindow(SW_SHOW);
	m_ConfigChiDlg.ShowWindow(SW_HIDE);
	m_nCurChiDlg = ID_DEVICEDLG;         //相应为当前显示子界面的标识符赋值//
}

//配方按钮的响应函数//
void CMainDlg::OnBnClickedBtConfig()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nCurChiDlg == ID_CONFIGDLG)
		return;
	m_ConfigChiDlg.ShowWindow(SW_SHOW);
	m_DeviceChiDlg.ShowWindow(SW_HIDE);

	m_VedioChiDlg.ShowWindow(SW_HIDE);
	m_VedioChiDlg.StopAllVideoPlay();

	m_HistoryChiDlg.ShowWindow(SW_HIDE);
	m_nCurChiDlg = ID_CONFIGDLG;
}

//销毁主窗体//
void CMainDlg::OnDestroy()
{
	OnBnClickedDisconnectPlc();

	m_VedioChiDlg.StopAllVideoPlay(); //断开视频连接//
	CDialog::OnDestroy();
	// TODO:  在此处添加消息处理程序代码
}


void CMainDlg::OnBnClickedUpdateWeather()
{
	// TODO:  在此添加控件通知处理程序代码
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

//定时器函数///
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		CString StrDate = CTime::GetCurrentTime().Format("%Y年%m月%d日");	//StrDate为当前日期///
		GetDlgItem(IDC_STATIC_DATE)->SetWindowText(StrDate);
		CString StrTime = CTime::GetCurrentTime().Format("%H:%M:%S");		//StrTime为当前时间///
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(StrTime);
		if (!m_bIsAlarmDlgShow && !m_bIsWarningDlgShow)           //如果报警画面不在显示状态//
		    UpdateAlarm();                      //监控报警//
		UpdateStatePara();                //更新状态显示//
	}
	if (nIDEvent==0)
	{
		OnBnClickedUpdateWeather();   //刷新天气，15分钟一次//
	}
	CDialog::OnTimer(nIDEvent);
}

//时间显示框的颜色设置///
HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//菜单栏注销按钮//
void CMainDlg::OnLogout()
{
	// TODO:  在此添加命令处理程序代码
	ShowWindow(SW_HIDE);
	CLoginDlg::LoginUser_CurrentPermissionLevel = 0;
	CLoginDlg::LoginUser_Name = _T("");
	CLoginDlg::LoginUser_CurrentId = 0;
	CString strLoginName;
	if (m_LoginDlg.DoModal() == IDCANCEL)
		PostQuitMessage(0);
	else
	{
		strLoginName.Format(_T("  当前用户： %s"), CLoginDlg::LoginUser_Name);
		m_StatBar.SetText(strLoginName, 0, 0);
		ShowWindow(SW_SHOW);
	}
}

//菜单栏弹出PLC状态监视//
void CMainDlg::OnPlcstatus()
{
	// TODO:  在此添加命令处理程序代码	
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
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件//    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //取得ID//  
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
	double fsp[2];                                    //缩放比例//
	POINT recNewSize;                                 //获取现在对话框的大小//  
	CRect TempRect;
	GetClientRect(&TempRect);                         //取客户区大小//    
	recNewSize.x = TempRect.right - TempRect.left;
	recNewSize.y = TempRect.bottom - TempRect.top;
	fsp[0] = (double)recNewSize.x / OldSizeOfClient.x;
	fsp[1] = (double)recNewSize.y / OldSizeOfClient.y;

	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件//    
	for (int n = 0; n < vectOldSize.size(); n++)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //取得ID//  
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
	// TODO:  在此添加命令处理程序代码
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
}


void CMainDlg::OnBnClickedConnectPLC()
{
	//调用modbus.CycleStart()启动PLC轮询机制
	if (m_Modbus.m_bIsCycling == TRUE)
		return;
	m_Modbus.CycleStart();
	int defaultConfigId;
	CString strtemp;
	for (int a = 0; a < m_pDataProvider->m_vectProductionLine.size(); a++)  //为每条生产线设置配方//
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
			strtemp.Format(_T("%s已使用默认配方！"), m_pDataProvider->m_vectProductionLine[a].m_strLineName);
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

void CMainDlg::OnBnClickedBtAlarm() //报警按钮//
{
	// TODO:  在此添加控件通知处理程序代码
	m_bIsAlarmDlgShow = true;
	m_AlarmDlg.DoModal();
	m_bIsAlarmDlgShow = false;
}

void CMainDlg::UpdateAlarm()  //报警函数//
{
	CString strErrorParaName;
	CString strTime;
	CAlarmClass tempAlarmClass;
	for (int x = 0; x < m_pDataProvider->m_vectFaultPara.size();x++)
	{
		if (m_pDataProvider->m_vectFaultPara[x].m_ParaValue != 0 && m_DeviceChiDlg.IsPlcConnected(m_pDataProvider->m_vectFaultPara[x].m_PLCId))
		{
			if (m_StatBar.GetText(1, 0) != _T("  有未处理的报警！"))
				m_StatBar.SetText(_T("  有未处理的报警！"), 1, 0);
			if (m_pDataProvider->m_vectFaultPara[x].m_nIndexInAlarmVect == -1) //如果该参数尚未进入报警列表//
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
				strErrorParaName.Format(_T("警报：%s!是否马上处理？点击“否”将忽略该报警。"), m_pDataProvider->m_vectFaultPara[x].m_strParaName);
				m_bIsAlarmDlgShow = true;
				if (MessageBox(strErrorParaName, _T("警告"), MB_ICONWARNING | MB_YESNO) == IDYES)
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
			if (m_StatBar.GetText(1, 0) != _T("  正常"))
				m_StatBar.SetText(_T("  正常"), 1, 0);
	}
}

void CMainDlg::UpdateStatePara()  //监听状态参数//
{
	for (int y = 0; y < m_pDataProvider->m_vectStatePara.size();y++)
	{
		if (m_DeviceChiDlg.IsPlcConnected(m_pDataProvider->m_vectStatePara[y].m_PlcId))
		{
			switch (m_pDataProvider->m_vectStatePara[y].m_ParaValue)
			{
			case 0:
				if (m_listState.GetItemText(y, 2) != _T("停止"))
					m_listState.SetItemText(y, 2, _T("停止"));
				break;
			case 1:
				if (m_listState.GetItemText(y, 2) != _T("正常运行"))
					m_listState.SetItemText(y, 2, _T("正常运行"));
				break;
			default:
				break;
			}
		} 
		else
		{
			if (m_listState.GetItemText(y, 2) != _T("未连接"))
			    m_listState.SetItemText(y, 2, _T("未连接"));
		}
	}
}

void CMainDlg::OnBnClickedBtHistroy()
{
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码

	m_HistoryChiDlg.StopRecordThread();

	CDialog::OnCancel();
}




//下拉菜单中上传数据选项回调函数//
void CMainDlg::OnUploadData()
{
	
}
