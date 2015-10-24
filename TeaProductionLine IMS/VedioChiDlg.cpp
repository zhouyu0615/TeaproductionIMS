// VedioChiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "VedioChiDlg.h"
#include "afxdialogex.h"


// CVedioChiDlg 对话框

IMPLEMENT_DYNAMIC(CVedioChiDlg, CDialog)

CVedioChiDlg::CVedioChiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVedioChiDlg::IDD, pParent)
{
	m_nPicStyle = 4;  //初始化，4画面风格//
	m_nNumOfPage = 1; //当前显示的页码//
	m_bIsVedioPlay = false;  //初始化摄像头是否已经在播放标识符//
	m_bHaveInit = false;
	m_bHaveRemenberOldSize = false;
	bool bIsPlay = false;
	for (int n = 0; n < 18;n++)
	{
		m_vectIsVideoPlay.push_back(bIsPlay);
	}
}

CVedioChiDlg::~CVedioChiDlg()
{
}

void CVedioChiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REALPLAYXCTRL1, m_vedio1);
	DDX_Control(pDX, IDC_REALPLAYXCTRL2, m_vedio2);
	DDX_Control(pDX, IDC_REALPLAYXCTRL3, m_vedio3);
	DDX_Control(pDX, IDC_REALPLAYXCTRL4, m_vedio4);
	DDX_Control(pDX, IDC_REALPLAYXCTRL5, m_vedio5);
	DDX_Control(pDX, IDC_REALPLAYXCTRL6, m_vedio6);
	DDX_Control(pDX, IDC_REALPLAYXCTRL7, m_vedio7);
	DDX_Control(pDX, IDC_REALPLAYXCTRL8, m_vedio8);
	DDX_Control(pDX, IDC_REALPLAYXCTRL9, m_vedio9);
	DDX_Control(pDX, IDC_REALPLAYXCTRL10, m_vedio10);
	DDX_Control(pDX, IDC_REALPLAYXCTRL11, m_vedio11);
	DDX_Control(pDX, IDC_REALPLAYXCTRL12, m_vedio12);
	DDX_Control(pDX, IDC_REALPLAYXCTRL13, m_vedio13);
	DDX_Control(pDX, IDC_REALPLAYXCTRL14, m_vedio14);
	DDX_Control(pDX, IDC_REALPLAYXCTRL15, m_vedio15);
	DDX_Control(pDX, IDC_REALPLAYXCTRL16, m_vedio16);
	DDX_Control(pDX, IDC_REALPLAYXCTRL17, m_vedio17);
	DDX_Control(pDX, IDC_REALPLAYXCTRL18, m_vedio18);
}


BEGIN_MESSAGE_MAP(CVedioChiDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_PLAY, &CVedioChiDlg::OnBnClickedBtPlay)
	ON_BN_CLICKED(IDC_BT_STOP, &CVedioChiDlg::OnBnClickedBtStop)
	ON_BN_CLICKED(IDC_BUTTON3, &CVedioChiDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_BT_SINGLEPIC, &CVedioChiDlg::OnBnClickedBtSinglepic)
	ON_BN_CLICKED(IDC_BT_FOURPIC, &CVedioChiDlg::OnBnClickedBtFourpic)
	ON_BN_CLICKED(IDC_BT_NINEPIC, &CVedioChiDlg::OnBnClickedBtNinepic)
	ON_BN_CLICKED(IDC_BT_16PIC, &CVedioChiDlg::OnBnClickedBt16pic)
	ON_BN_CLICKED(IDC_BT_NEXT, &CVedioChiDlg::OnBnClickedBtNext)
	ON_BN_CLICKED(IDC_BT_LAST, &CVedioChiDlg::OnBnClickedBtLast)
END_MESSAGE_MAP()


// CVedioChiDlg 消息处理程序

//窗体大小变化时，使控件自适应//
void CVedioChiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		if (m_bHaveInit&&m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			switch (m_nPicStyle)
			{
			case 1:
				getVideoRect(1);
				for (int n = 0; n < m_vectVedio.size();n++)
					m_vectVedio[n]->MoveWindow(m_vectVideoRect[0]);
				break;
			case 4:
				getVideoRect(4);
				for (int n = 0; n < 4;n++)
					for (int m = 0; m < m_vectVideoRect.size();m++)
						m_vectVedio[m + 4 * n]->MoveWindow(m_vectVideoRect[m]);
				break;
			case 9:
				getVideoRect(9);
				for (int n = 0; n < 2; n++)
					for (int m = 0; m < m_vectVideoRect.size(); m++)
						m_vectVedio[m + 9 * n]->MoveWindow(m_vectVideoRect[m]);
				break;
			case 16:
				getVideoRect(16);
				for (int n = 0; n < m_vectVideoRect.size();n++)
					m_vectVedio[n]->MoveWindow(m_vectVideoRect[n]);
				break;
			default:
				break;
			}
			Invalidate();
		}
}


BOOL CVedioChiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bHaveInit = true;
	// TODO:  在此添加额外的初始化

	//将18个摄像头显示控件放入容器//
	m_vectVedio.push_back(&m_vedio1);
	m_vectVedio.push_back(&m_vedio2);
	m_vectVedio.push_back(&m_vedio3);
	m_vectVedio.push_back(&m_vedio4);
	m_vectVedio.push_back(&m_vedio5);
	m_vectVedio.push_back(&m_vedio6);
	m_vectVedio.push_back(&m_vedio7);
	m_vectVedio.push_back(&m_vedio8);
	m_vectVedio.push_back(&m_vedio9);
	m_vectVedio.push_back(&m_vedio10);
	m_vectVedio.push_back(&m_vedio11);
	m_vectVedio.push_back(&m_vedio12);
	m_vectVedio.push_back(&m_vedio13);
	m_vectVedio.push_back(&m_vedio14);
	m_vectVedio.push_back(&m_vedio15);
	m_vectVedio.push_back(&m_vedio16);
	m_vectVedio.push_back(&m_vedio17);
	m_vectVedio.push_back(&m_vedio18);

	CRect rectTemp;                //记住原始尺寸//
	if (!m_bHaveRemenberOldSize)
	{
		GetClientRect(&rectTemp);
		m_OldSize.x = rectTemp.right - rectTemp.left;
		m_OldSize.y = rectTemp.bottom - rectTemp.top;
		RememberOldSize(m_vectOldSize);
		m_bHaveRemenberOldSize = true;
	}

	ShowFourVedio(m_vedio1, m_vedio2, m_vedio3, m_vedio4);    //默认显示四窗格//
	GetDlgItem(IDC_ST_PAGE)->SetWindowText(_T("1"));

	//读取数据库，注册相机//
	m_nNumOfVedio = m_pDataProvider->m_vectVideo.size();      //数据库中相机的个数//
	if (m_nNumOfVedio > 4)                                    //如果摄像头个数大于4，则使能下一页按钮//
		GetDlgItem(IDC_BT_NEXT)->EnableWindow(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


//开始所有按钮响应函数//
void CVedioChiDlg::OnBnClickedBtPlay()
{
	AfxBeginThread(ThreadFunc, this);

	IsVideoPlay = true;
}

//停止所有响应函数//
void CVedioChiDlg::OnBnClickedBtStop()
{
	StopAllVideoPlay();
	IsVideoPlay = false;
}

void CVedioChiDlg::StopAllVideoPlay()
{
	for (int n = 0; n < m_nNumOfVedio; n++)
	{
		m_vectVedio[n]->StopPlay();
		m_vectIsVideoPlay[n] = false;
	}
}

//刷新按钮的响应函数//
void CVedioChiDlg::OnBnClickedRefresh()
{
	// TODO:  在此添加控件通知处理程序代码
	AfxBeginThread(ThreadConnectVideo, this);
}

//连接摄像头，并播放//
UINT CVedioChiDlg::ThreadConnectVideo(LPVOID lpParam)
{
	unsigned char ispalying, isconnected;
	((CVedioChiDlg*)lpParam)->m_nNumOfVedio = ((CVedioChiDlg*)lpParam)->m_pDataProvider->m_vectVideo.size();
	for (int n = 0; n < ((CVedioChiDlg*)lpParam)->m_nNumOfVedio; n++)
	{
		isconnected = ((CVedioChiDlg*)lpParam)->m_vectVedio[n]->SetDeviceInfo(((CVedioChiDlg*)lpParam)->m_pDataProvider->m_vectVideo[n].m_strIPAddr,
			((CVedioChiDlg*)lpParam)->m_pDataProvider->m_vectVideo[n].m_port, 0, _T("admin"), _T("admin"));
		ispalying = ((CVedioChiDlg*)lpParam)->m_vectVedio[n]->StartPlay();
		if (isconnected == -1 && ispalying == -1)
			((CVedioChiDlg*)lpParam)->m_vectIsVideoPlay[n] = true;
		else
			((CVedioChiDlg*)lpParam)->m_vectIsVideoPlay[n] = false;
	}
	return 1;
}

//单画面按钮响应函数//
void CVedioChiDlg::OnBnClickedBtSinglepic()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nPicStyle != 1)
	{
		ShowOneVedio(m_vedio1);
		m_nNumOfPage = 1;
		GetDlgItem(IDC_ST_PAGE)->SetWindowText(_T("1"));
		GetDlgItem(IDC_BT_LAST)->EnableWindow(FALSE);
		if (m_nNumOfVedio > 1)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
	}

}

//四画面按钮响应函数//
void CVedioChiDlg::OnBnClickedBtFourpic()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nPicStyle != 4)
	{
		ShowFourVedio(m_vedio1, m_vedio2, m_vedio3, m_vedio4);
		m_nNumOfPage = 1;
		GetDlgItem(IDC_ST_PAGE)->SetWindowText(_T("1"));
		GetDlgItem(IDC_BT_LAST)->EnableWindow(FALSE);
		if (m_nNumOfVedio > 4)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
		Invalidate();
	}
}

//九画面按钮响应函数//
void CVedioChiDlg::OnBnClickedBtNinepic()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nPicStyle != 9)
	{
		ShowNineVedio(1);
		GetDlgItem(IDC_ST_PAGE)->SetWindowText(_T("1"));
		GetDlgItem(IDC_BT_LAST)->EnableWindow(FALSE);
		if (m_nNumOfVedio > 9)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
		Invalidate();
	}
}

//16画面按钮的响应函数//
void CVedioChiDlg::OnBnClickedBt16pic()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nPicStyle != 16)
	{
		getVideoRect(16);
		for (int n = 0; n < m_vectVideoRect.size();n++)
		{
			m_vectVedio[n]->MoveWindow(m_vectVideoRect[n]);
		}

		for (int n = 0; n < 16; n++)
			m_vectVedio[n]->ShowWindow(SW_SHOW);
		m_vedio17.ShowWindow(SW_HIDE);
		m_vedio18.ShowWindow(SW_HIDE);
		
		m_nPicStyle = 16;
		m_nNumOfPage = 1;
		GetDlgItem(IDC_ST_PAGE)->SetWindowText(_T("1"));
		GetDlgItem(IDC_BT_LAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
		Invalidate();
	}

}

//显示指定单个摄像头，并修改画面风格标志//
void CVedioChiDlg::ShowOneVedio(CRealplayxctrl1 &vedio)
{
	getVideoRect(1);
	vedio.MoveWindow(m_vectVideoRect[0]);
	for (int n = 0; n < 18; n++)
		m_vectVedio[n]->ShowWindow(SW_HIDE);
	vedio.ShowWindow(SW_SHOW);
	m_nPicStyle = 1;
}

//显示指定的四个摄像头，并修改画面风格标志//
void CVedioChiDlg::ShowFourVedio(CRealplayxctrl1 &vedio1, CRealplayxctrl1 &vedio2, CRealplayxctrl1 &vedio3, CRealplayxctrl1 &vedio4)
{
	getVideoRect(4);

	if (m_vectVideoRect.size() != 4)
	{
		AfxMessageBox(_T("程序运行出错！"));
		return;
	}
	vedio1.MoveWindow(m_vectVideoRect[0]);
	vedio2.MoveWindow(m_vectVideoRect[1]);
	vedio3.MoveWindow(m_vectVideoRect[2]);
	vedio4.MoveWindow(m_vectVideoRect[3]);
	for (int n = 0; n < 18; n++)
		m_vectVedio[n]->ShowWindow(SW_HIDE);

	vedio1.ShowWindow(SW_SHOW);
	vedio2.ShowWindow(SW_SHOW);
	vedio3.ShowWindow(SW_SHOW);
	vedio4.ShowWindow(SW_SHOW);

	m_nPicStyle = 4;
}

//显示九画面,并修改页码标志和画面风格标志//
void CVedioChiDlg::ShowNineVedio(int nPage)
{
	if (nPage != 1 && nPage != 2)
		return;

	getVideoRect(9);

	for (int n = 0; n < m_vectVideoRect.size();n++)
		m_vectVedio[n]->MoveWindow(m_vectVideoRect[n]);

	for (int n = 0; n < m_vectVideoRect.size(); n++)
		m_vectVedio[n+9]->MoveWindow(m_vectVideoRect[n]);

	switch(nPage)
    {
	case 1:
		for (int n = 0; n < 9;n++)
			m_vectVedio[n]->ShowWindow(SW_SHOW);

		for (int n = 9; n < 18; n++)
			m_vectVedio[n]->ShowWindow(SW_HIDE);

		m_nPicStyle = 9;
		m_nNumOfPage = 1;
		break;
	case 2:
		for (int n = 0; n < 9; n++)
			m_vectVedio[n]->ShowWindow(SW_HIDE);

		for (int n = 9; n < 18; n++)
			m_vectVedio[n]->ShowWindow(SW_SHOW);

		m_nNumOfPage = 2;
		m_nPicStyle = 9;
		break;
	default:
		break;
    } 
}

//下一页按钮响应函数//
void CVedioChiDlg::OnBnClickedBtNext()
{
	// TODO:  在此添加控件通知处理程序代码

	switch (m_nPicStyle)
	{
	case 1:
		ShowOneVedio(*(m_vectVedio[m_nNumOfPage]));
		if (m_nNumOfPage >= m_pDataProvider->m_vectVideo.size() - 1)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
		m_nNumOfPage++;
		break;
	case 4:
		ShowFourVedio(*(m_vectVedio[m_nNumOfPage * 4]), *(m_vectVedio[m_nNumOfPage * 4 + 1]), *(m_vectVedio[m_nNumOfPage * 4 + 2]), *(m_vectVedio[m_nNumOfPage * 4 + 3]));
		if (m_nNumOfPage >= (m_pDataProvider->m_vectVideo.size()-1)/4)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
		m_nNumOfPage++;
		break;
	case 9:
		ShowNineVedio(2);
		GetDlgItem(IDC_BT_NEXT)->EnableWindow(FALSE);
		break;
	default:
		break;
	}
	if (m_nNumOfPage > 1)
		GetDlgItem(IDC_BT_LAST)->EnableWindow(TRUE);
	CString strPage;
	strPage.Format(_T("%d"), m_nNumOfPage);
	GetDlgItem(IDC_ST_PAGE)->SetWindowText(strPage);
	Invalidate();
}

//上一页按钮响应函数//
void CVedioChiDlg::OnBnClickedBtLast()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nNumOfPage > 1)
		m_nNumOfPage--;
	else
		return;
	switch (m_nPicStyle)
	{
	case 1:
		ShowOneVedio(*(m_vectVedio[m_nNumOfPage-1]));
		if (GetDlgItem(IDC_BT_NEXT)->IsWindowEnabled() == false)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(true);
		break;
	case 4:
		ShowFourVedio(*(m_vectVedio[m_nNumOfPage * 4-4]), *(m_vectVedio[m_nNumOfPage * 4 -3]), *(m_vectVedio[m_nNumOfPage * 4 - 2]), *(m_vectVedio[m_nNumOfPage * 4 -1]));
		if (GetDlgItem(IDC_BT_NEXT)->IsWindowEnabled() == false)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(true);
		break;
	case 9:
		ShowNineVedio(1);
		if (GetDlgItem(IDC_BT_NEXT)->IsWindowEnabled() == false)
			GetDlgItem(IDC_BT_NEXT)->EnableWindow(true);
		break;
	default:
		break;
	}
	if (m_nNumOfPage <= 1)
		GetDlgItem(IDC_BT_LAST)->EnableWindow(false);
	CString strPage;
	strPage.Format(_T("%d"), m_nNumOfPage);
	GetDlgItem(IDC_ST_PAGE)->SetWindowText(strPage);
	Invalidate();
}

//重载虚函数//
void CVedioChiDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

}


void CVedioChiDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
{
	vectOldSize.clear();
	CRect Rect;
	int woc;
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件//    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);              //取得ID//  
		/*if (woc != IDC_REALPLAYXCTRL2 && woc != IDC_REALPLAYXCTRL1 && woc != IDC_REALPLAYXCTRL3 && woc != IDC_REALPLAYXCTRL4 && woc != IDC_REALPLAYXCTRL5 && woc != IDC_REALPLAYXCTRL6 &&
			woc != IDC_REALPLAYXCTRL7 && woc != IDC_REALPLAYXCTRL8 && woc != IDC_REALPLAYXCTRL9 && woc != IDC_REALPLAYXCTRL10 && woc != IDC_REALPLAYXCTRL11
			&& woc != IDC_REALPLAYXCTRL12 && woc != IDC_REALPLAYXCTRL13 && woc != IDC_REALPLAYXCTRL14 && woc != IDC_REALPLAYXCTRL15 && woc != IDC_REALPLAYXCTRL16 && woc != IDC_REALPLAYXCTRL17 && woc != IDC_REALPLAYXCTRL18)*/
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		vectOldSize.push_back(Rect);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

void CVedioChiDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
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

void  CVedioChiDlg::getVideoRect(int nPicStyle)   //获得当前窗口下video控件的位置大小//
{
	m_vectVideoRect.clear();
	CRect RectOfChiDlg, rect1, rect2, rect3, rect4, rect5, rect6, rect7, rect8, rect9, rect10, rect11, rect12, rect13, rect14, rect15, rect16;
	GetWindowRect(RectOfChiDlg);
	ScreenToClient(RectOfChiDlg);
	switch (nPicStyle)
	{
	case 1:
		rect1.left = RectOfChiDlg.left;
		rect1.top = RectOfChiDlg.top;
		rect1.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		rect1.right = RectOfChiDlg.right;
		m_vectVideoRect.push_back(rect1);
		break;
	case 4:
		rect1.left = RectOfChiDlg.left;
		rect1.top = RectOfChiDlg.top;
		rect1.bottom = (double)RectOfChiDlg.bottom / 16 * 7.3;
		rect1.right = (double)RectOfChiDlg.right / 2.01;
		m_vectVideoRect.push_back(rect1);

		rect2.left = (double)RectOfChiDlg.right / 1.99;
		rect2.bottom = (double)RectOfChiDlg.bottom / 16 * 7.3;
		rect2.top = RectOfChiDlg.top;
		rect2.right = RectOfChiDlg.right;
		m_vectVideoRect.push_back(rect2);

		rect3.left = RectOfChiDlg.left;
		rect3.top = (double)RectOfChiDlg.bottom / 16 * 7.4;
		rect3.right = (double)RectOfChiDlg.right / 2.01;
		rect3.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect3);

		rect4.left = (double)RectOfChiDlg.right / 1.99;
		rect4.top = (double)RectOfChiDlg.bottom / 16 * 7.4;
		rect4.right = RectOfChiDlg.right;
		rect4.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect4);
		break;
	case 9:
		rect1.left = RectOfChiDlg.left;
		rect1.top = RectOfChiDlg.top;
		rect1.bottom = (double)RectOfChiDlg.bottom / 16 * 4.8;
		rect1.right = (double)RectOfChiDlg.right / 240 * 79;
		m_vectVideoRect.push_back(rect1);

		rect2.left = (double)RectOfChiDlg.right / 3;
		rect2.bottom = (double)RectOfChiDlg.bottom / 16 * 4.8;
		rect2.top = RectOfChiDlg.top;
		rect2.right = (double)RectOfChiDlg.right / 240 * 159;
		m_vectVideoRect.push_back(rect2);

		rect3.left = (double)RectOfChiDlg.right / 3 * 2;
		rect3.top = RectOfChiDlg.top;
		rect3.right = RectOfChiDlg.right;
		rect3.bottom = (double)RectOfChiDlg.bottom / 16 * 4.8;
		m_vectVideoRect.push_back(rect3);

		rect4.left = RectOfChiDlg.left;
		rect4.top = (double)RectOfChiDlg.bottom / 16 * 4.9;
		rect4.right = (double)RectOfChiDlg.right / 240 * 79;
		rect4.bottom = (double)RectOfChiDlg.bottom / 16 * 9.7;
		m_vectVideoRect.push_back(rect4);

		rect5.left = RectOfChiDlg.right / 3;
		rect5.top = (double)RectOfChiDlg.bottom / 16 * 4.9;
		rect5.right = (double)RectOfChiDlg.right / 240 * 159;
		rect5.bottom = (double)RectOfChiDlg.bottom / 16 * 9.7;
		m_vectVideoRect.push_back(rect5);

		rect6.left = (double)RectOfChiDlg.right / 3 * 2;
		rect6.top = (double)RectOfChiDlg.bottom / 16 * 4.9;
		rect6.right = RectOfChiDlg.right;
		rect6.bottom = (double)RectOfChiDlg.bottom / 16 * 9.7;
		m_vectVideoRect.push_back(rect6);

		rect7.left = RectOfChiDlg.left;
		rect7.top = (double)RectOfChiDlg.bottom / 16 * 9.8;
		rect7.right = (double)RectOfChiDlg.right / 240 * 79;
		rect7.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect7);

		rect8.left = (double)RectOfChiDlg.right / 3;
		rect8.top = (double)RectOfChiDlg.bottom / 16 * 9.8;
		rect8.right = (double)RectOfChiDlg.right / 240 * 159;
		rect8.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect8);

		rect9.left = (double)RectOfChiDlg.right / 3 * 2;
		rect9.top = (double)RectOfChiDlg.bottom / 16 * 9.8;
		rect9.right = RectOfChiDlg.right;
		rect9.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect9);
		break;
	case 16:
		rect1.left = RectOfChiDlg.left;
		rect1.top = RectOfChiDlg.top;
		rect1.bottom = (double)RectOfChiDlg.bottom / 16 * 3.6;
		rect1.right = (double)RectOfChiDlg.right / 240 * 59;
		m_vectVideoRect.push_back(rect1);

		rect2.left = (double)RectOfChiDlg.right / 4;
		rect2.bottom = (double)RectOfChiDlg.bottom / 16 * 3.6;
		rect2.top = RectOfChiDlg.top;
		rect2.right = (double)RectOfChiDlg.right / 240 * 119;
		m_vectVideoRect.push_back(rect2);

		rect3.left = (double)RectOfChiDlg.right / 2;
		rect3.top = RectOfChiDlg.top;
		rect3.right = (double)RectOfChiDlg.right / 240 * 179;
		rect3.bottom = (double)RectOfChiDlg.bottom / 16 * 3.6;
		m_vectVideoRect.push_back(rect3);

		rect4.left = (double)RectOfChiDlg.right / 4 * 3;
		rect4.top = (double)RectOfChiDlg.top;
		rect4.right = (double)RectOfChiDlg.right;
		rect4.bottom = (double)RectOfChiDlg.bottom / 16 * 3.6;
		m_vectVideoRect.push_back(rect4);

		rect5.left = (double)RectOfChiDlg.left;
		rect5.top = (double)RectOfChiDlg.bottom / 16 * 3.7;
		rect5.right = (double)RectOfChiDlg.right / 240 * 59;
		rect5.bottom = (double)RectOfChiDlg.bottom / 16 * 7.3;
		m_vectVideoRect.push_back(rect5);

		rect6.left = (double)RectOfChiDlg.right / 4;
		rect6.top = (double)RectOfChiDlg.bottom / 16 * 3.7;
		rect6.right = (double)RectOfChiDlg.right / 240 * 119;
		rect6.bottom = (double)RectOfChiDlg.bottom / 16 * 7.3;
		m_vectVideoRect.push_back(rect6);

		rect7.left = (double)RectOfChiDlg.right / 2;
		rect7.top = (double)RectOfChiDlg.bottom / 16 * 3.7;
		rect7.right = (double)RectOfChiDlg.right / 240 * 179;
		rect7.bottom = (double)RectOfChiDlg.bottom / 16 * 7.3;
		m_vectVideoRect.push_back(rect7);

		rect8.left = (double)RectOfChiDlg.right / 4 * 3;
		rect8.top = (double)RectOfChiDlg.bottom / 16 * 3.7;
		rect8.right = (double)RectOfChiDlg.right;
		rect8.bottom = (double)RectOfChiDlg.bottom / 16 * 7.3;
		m_vectVideoRect.push_back(rect8);

		rect9.left = (double)RectOfChiDlg.left;
		rect9.top = (double)RectOfChiDlg.bottom / 16 * 7.4;
		rect9.right = (double)RectOfChiDlg.right / 240 * 59;
		rect9.bottom = (double)RectOfChiDlg.bottom / 16 * 11;
		m_vectVideoRect.push_back(rect9);

		rect10.left = (double)RectOfChiDlg.right / 4;
		rect10.top = (double)RectOfChiDlg.bottom / 16 * 7.4;
		rect10.right = (double)RectOfChiDlg.right / 240 * 119;
		rect10.bottom = (double)RectOfChiDlg.bottom / 16 * 11;
		m_vectVideoRect.push_back(rect10);

		rect11.left = (double)RectOfChiDlg.right / 2;
		rect11.top = (double)RectOfChiDlg.bottom / 16 * 7.4;
		rect11.right = (double)RectOfChiDlg.right / 240 * 179;
		rect11.bottom = (double)RectOfChiDlg.bottom / 16 * 11;
		m_vectVideoRect.push_back(rect11);

		rect12.left = (double)RectOfChiDlg.right / 4 * 3;
		rect12.top = (double)RectOfChiDlg.bottom / 16 * 7.4;
		rect12.right = (double)RectOfChiDlg.right;
		rect12.bottom = (double)RectOfChiDlg.bottom / 16 * 11;
		m_vectVideoRect.push_back(rect12);

		rect13.left = RectOfChiDlg.left;
		rect13.top = (double)RectOfChiDlg.bottom / 16 * 11.1;
		rect13.right = (double)RectOfChiDlg.right / 240 * 59;
		rect13.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect13);

		rect14.left = (double)RectOfChiDlg.right / 4;
		rect14.top = (double)RectOfChiDlg.bottom / 16 * 11.1;
		rect14.right = (double)RectOfChiDlg.right / 240 * 119;
		rect14.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect14);

		rect15.left = (double)RectOfChiDlg.right / 2;
		rect15.top = (double)RectOfChiDlg.bottom / 16 * 11.1;
		rect15.right = (double)RectOfChiDlg.right / 240 * 179;
		rect15.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect15);

		rect16.left = (double)RectOfChiDlg.right / 4 * 3;
		rect16.top = (double)RectOfChiDlg.bottom / 16 * 11.1;
		rect16.right = RectOfChiDlg.right;
		rect16.bottom = (double)RectOfChiDlg.bottom / 16 * 14.7;
		m_vectVideoRect.push_back(rect16);
		break;
	default:
		break;
	}
}
BEGIN_EVENTSINK_MAP(CVedioChiDlg, CDialog)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL1, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl1, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL1, DISPID_CLICK, CVedioChiDlg::ClickRealplayxctrl1, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL2, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl2, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL3, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl3, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL4, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl4, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL5, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl5, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL6, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl6, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL7, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl7, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL8, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl8, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL9, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl9, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL10, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl10, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL11, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl11, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL12, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl12, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL13, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl13, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL14, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl14, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL15, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl15, VTS_NONE)
	ON_EVENT(CVedioChiDlg, IDC_REALPLAYXCTRL16, DISPID_DBLCLICK, CVedioChiDlg::DblClickRealplayxctrl16, VTS_NONE)
END_EVENTSINK_MAP()


void CVedioChiDlg::DblClickRealplayxctrl1() //双击VIDEO1控件//
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size()>0 && m_vectIsVideoPlay[0])
	{
	    CFullScreenDlg FullScreenDlg;
	    FullScreenDlg.m_nItem = 0;
	    FullScreenDlg.DoModal();
	}
}


UINT CVedioChiDlg::ThreadFunc(LPVOID lpParam)   //线程函数，连接所有摄像头//
{
	signed char IsConnected,IsPlaying;

	for (int n = 0; n < ((CVedioChiDlg*)lpParam)->m_nNumOfVedio; n++)
	{
		IsConnected = ((CVedioChiDlg*)lpParam)->m_vectVedio[n]->SetDeviceInfo(((CVedioChiDlg*)lpParam)->m_pDataProvider->m_vectVideo[n].m_strIPAddr, 
			((CVedioChiDlg*)lpParam)->m_pDataProvider->m_vectVideo[n].m_port, 0, _T("admin"), _T("admin"));
		IsPlaying = ((CVedioChiDlg*)lpParam)->m_vectVedio[n]->StartPlay();
		if (IsPlaying==-1&&IsConnected==-1)
			((CVedioChiDlg*)lpParam)->m_vectIsVideoPlay[n] = true;
		else
			((CVedioChiDlg*)lpParam)->m_vectIsVideoPlay[n] = false;
	}
	return 1;
}

void CVedioChiDlg::ClickRealplayxctrl1() //video1单击响应//
{
	// TODO:  在此处添加消息处理程序代码
}


void CVedioChiDlg::DblClickRealplayxctrl2()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 1 && m_vectIsVideoPlay[1])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 1;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl3()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 2 && m_vectIsVideoPlay[2])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 2;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl4()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 3 && m_vectIsVideoPlay[3])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 3;
		FullScreenDlg.DoModal();
	}
}



void CVedioChiDlg::DblClickRealplayxctrl5()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 4 && m_vectIsVideoPlay[4])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 4;
		FullScreenDlg.DoModal();
	}
}



void CVedioChiDlg::DblClickRealplayxctrl6()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 5 && m_vectIsVideoPlay[5])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 5;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl7()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 6 && m_vectIsVideoPlay[6])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 6;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl8()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 7 && m_vectIsVideoPlay[7])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 7;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl9()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 8 && m_vectIsVideoPlay[8])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 8;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl10()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 9 && m_vectIsVideoPlay[9])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 9;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl11()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 10 && m_vectIsVideoPlay[10])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 10;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl12()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 11 && m_vectIsVideoPlay[11])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 11;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl13()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 12 && m_vectIsVideoPlay[12])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 12;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl14()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 13 && m_vectIsVideoPlay[13])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 13;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl15()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 14 && m_vectIsVideoPlay[14])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 14;
		FullScreenDlg.DoModal();
	}
}


void CVedioChiDlg::DblClickRealplayxctrl16()
{
	// TODO:  在此处添加消息处理程序代码
	if (m_pDataProvider->m_vectVideo.size() > 15 && m_vectIsVideoPlay[15])
	{
		CFullScreenDlg FullScreenDlg;
		FullScreenDlg.m_nItem = 15;
		FullScreenDlg.DoModal();
	}
}


BOOL CVedioChiDlg::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类

	StopAllVideoPlay();


	return CDialog::DestroyWindow();
}
