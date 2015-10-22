// DeviceChiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "DeviceChiDlg.h"
#include "afxdialogex.h"



// CDeviceChiDlg 对话框

IMPLEMENT_DYNAMIC(CDeviceChiDlg, CDialog)

CDeviceChiDlg::CDeviceChiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceChiDlg::IDD, pParent)
{
	m_nCurSelModuleId = 0;//初始化变量，导航栏当前选中模块ID//
	m_bHaveInit = false;
	m_bHaveRemenberOldSize = false;
	m_bHaveEditCreate = false;
}

CDeviceChiDlg::~CDeviceChiDlg()
{
}

void CDeviceChiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE_CHIDLG, m_treeDevice);
	DDX_Control(pDX, IDC_LI1_MODULEPARA, m_listModulePara);
	DDX_Control(pDX, IDC_LI2_DEVICEPARA, m_listDevicePara);
	DDX_Control(pDX, IDC_ST_CURPOSITION, m_textCurPosition);
	DDX_Control(pDX, IDC_REALPLAYXCTRL1, m_video);
}


BEGIN_MESSAGE_MAP(CDeviceChiDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DEVICE_CHIDLG, &CDeviceChiDlg::OnTvnSelchangedTreeDeviceChidlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_LI2_DEVICEPARA, &CDeviceChiDlg::OnNMDblclkLi2Devicepara)
	ON_NOTIFY(NM_DBLCLK, IDC_LI1_MODULEPARA, &CDeviceChiDlg::OnNMDblclkLi1Modulepara)
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID, &CDeviceChiDlg::OnEnKillfocusEditCreateid)
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE_DEVICE_CHIDLG, &CDeviceChiDlg::OnTvnSelchangingTreeDeviceChidlg)
END_MESSAGE_MAP()


// CDeviceChiDlg 消息处理程序

//窗体大小发生变化的响应函数//
void CDeviceChiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		if (m_bHaveInit&&m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			CRect rectList1;                        //重新设定列表的列宽//
			m_listModulePara.GetWindowRect(&rectList1);
			m_listModulePara.SetColumnWidth(1, rectList1.Width() / 27.0*2.0);
			m_listModulePara.SetColumnWidth(2, rectList1.Width() / 27.0 *10.85);
			m_listModulePara.SetColumnWidth(3, rectList1.Width() / 27.0 * 4.8);
			m_listModulePara.SetColumnWidth(4, rectList1.Width() / 27.0 * 4.8);
			m_listModulePara.SetColumnWidth(5, rectList1.Width() / 27.0 * 4.5);
			CRect rectList2;
			m_listDevicePara.GetWindowRect(&rectList2);
			m_listDevicePara.SetColumnWidth(1, rectList2.Width() / 7.0*1.16);
			m_listDevicePara.SetColumnWidth(2, rectList2.Width() / 7.0 * 3.90);
			m_listDevicePara.SetColumnWidth(3, rectList2.Width() / 7.0 * 1.88);
			Invalidate();
		}
}


BOOL CDeviceChiDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	m_bHaveInit = true;

	// TODO:  在此添加额外的初始化
	HICON hIcon[3];      // 图标句柄数组//
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICONTREE);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON4);
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON3);
	// 创建图像序列CImageList对象   
	m_imageList.Create(24, 24, ILC_COLOR32, 3, 3);
	m_imageList.Add(hIcon[0]);
	m_imageList.Add(hIcon[1]);
	m_imageList.Add(hIcon[2]);
	InitTreeContrl();   //初始化导航栏//

	SetTimer(3, 1001, NULL); //设置更新界面的定时器
	
	//初始化2个列表框//
	m_bHaveEditCreate = false;            //初始化标志位，表示还没有创建编辑框//
	CRect rectList1;
	m_listModulePara.GetWindowRect(&rectList1);
	m_listModulePara.SetExtendedStyle(m_listModulePara.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE );
	m_listModulePara.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listModulePara.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList1.Width() / 27.0*2.0, -1);
	m_listModulePara.InsertColumn(2, _T("工艺参数"), LVCFMT_CENTER, rectList1.Width() / 27.0 *10.9, -1);
	m_listModulePara.InsertColumn(3, _T("当前值"), LVCFMT_CENTER, rectList1.Width() / 27.0 * 4.8, -1);
	m_listModulePara.InsertColumn(4, _T("设定值"), LVCFMT_CENTER, rectList1.Width() / 27.0 * 4.8, -1);
	m_listModulePara.InsertColumn(5, _T("单位"), LVCFMT_CENTER, rectList1.Width() / 27.0 * 4.5, -1);

	CRect rectList2;
	m_listDevicePara.GetWindowRect(&rectList2);
	m_listDevicePara.SetExtendedStyle(m_listDevicePara.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SINGLESEL | LBS_OWNERDRAWVARIABLE);
	m_listDevicePara.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
	m_listDevicePara.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList2.Width() / 7.0*1.2, -1);
	m_listDevicePara.InsertColumn(2, _T("设备"), LVCFMT_CENTER, rectList2.Width() / 7.0 * 3.92, -1);
	m_listDevicePara.InsertColumn(3, _T("当前状态"), LVCFMT_CENTER, rectList2.Width() / 7.0 * 1.88, -1);


	CRect rectTemp;                //记住原始尺寸//
	if (!m_bHaveRemenberOldSize)
	{
		GetClientRect(&rectTemp);
		m_OldSize.x = rectTemp.right - rectTemp.left;
		m_OldSize.y = rectTemp.bottom - rectTemp.top;
		RememberOldSize(m_vectOldSize);
		m_bHaveRemenberOldSize = true;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//重新初始化树形控件//
void CDeviceChiDlg::InitTreeContrl()
{
	m_treeDevice.DeleteAllItems();
	HTREEITEM hRoot;     // 树的根节点的句柄//   
	HTREEITEM hFirItem;  // 可表示任一一级节点的句柄//
	// 为树形控件设置图像序列//  
	m_treeDevice.SetImageList(&m_imageList, TVSIL_NORMAL);
	for (int nLine = 0; nLine < m_pDataProvider->m_vectProductionLine.size();nLine++)
	{
		hRoot = m_treeDevice.InsertItem(m_pDataProvider->m_vectProductionLine[nLine].m_strLineName, 0, 0, TVI_ROOT, TVI_LAST);
		m_treeDevice.SetItemData(hRoot, m_pDataProvider->m_vectProductionLine[nLine].m_Id);
		for (int nModule = 0; nModule < m_pDataProvider->m_vectProcessModule.size();nModule++)
		{
			if (m_pDataProvider->m_vectProcessModule[nModule].m_ProcessLineId == m_pDataProvider->m_vectProductionLine[nLine].m_Id)
			{
				hFirItem = m_treeDevice.InsertItem(m_pDataProvider->m_vectProcessModule[nModule].m_strProcessModuleName, 1, 2, hRoot, TVI_LAST);
				m_treeDevice.SetItemData(hFirItem, m_pDataProvider->m_vectProcessModule[nModule].m_Id);
			}
		}
	}
}

//当树形控件选中条目发生变化时的响应函数//
void CDeviceChiDlg::OnTvnSelchangedTreeDeviceChidlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	//删除两个列表框控件的内容//
	m_listDevicePara.DeleteAllItems();
	m_listModulePara.DeleteAllItems();

	m_video.StopPlay();   //视频播放停止//

	HTREEITEM hCurSelItem = m_treeDevice.GetSelectedItem();
	UINT TempID = m_treeDevice.GetItemData(hCurSelItem);
	HTREEITEM hParentItem = m_treeDevice.GetParentItem(hCurSelItem);
	CString strCurPosition;
	if (hParentItem!=NULL)                              //如果选中的是工艺模块节点//
	{
		m_nCurSelModuleId = TempID;                                                                 //当前选中模块的ID//
		CString strLineName = m_treeDevice.GetItemText(hParentItem);    //所属生产线名称//
		strCurPosition = _T("当前位置： ") + strLineName + _T(" >> ") + m_treeDevice.GetItemText(hCurSelItem)+_T(" >> ");  //位置栏更新//
		m_textCurPosition.SetWindowText(strCurPosition);  
		
		SearchFromDataProvider(m_nCurSelModuleId);    //将当前选中模块下的摄像头、工艺参数、设备参数存到临时容器//
		WriteToDeviceList();                          //填写两个列表框//
		WriteToModuleList();
		if (!m_vectTempVideo.empty())                 //如果该模块有视频监控，则显示//
			AfxBeginThread(ThreadFunc, this);
	}
	else                                             //如果选中的是生产线节点//
	{
		strCurPosition = _T("当前位置： ") + m_treeDevice.GetItemText(hCurSelItem) + _T(" >> ");
		m_textCurPosition.SetWindowText(strCurPosition);
		m_listModulePara.DeleteAllItems();
		m_listDevicePara.DeleteAllItems();
		m_vectTempDevicePara.clear();
		m_vectTempModulePara.clear();
		m_vectTempVideo.clear();
	}

}

//将当前选中模块下的摄像头、工艺参数、设备参数存到临时容器//
void CDeviceChiDlg::SearchFromDataProvider(UINT nCerSelModuleId)
{
	m_vectTempDevicePara.clear();
	m_vectTempModulePara.clear();
	m_vectTempVideo.clear();
	for (int n = 0; n < m_pDataProvider->m_vectDevicePara.size();n++)  //设备参数//
	{
		if (m_pDataProvider->m_vectDevicePara[n].m_ProcessModuleId==nCerSelModuleId)
		{
			m_vectTempDevicePara.push_back(m_pDataProvider->m_vectDevicePara[n]);
		}
	}
	for (int i = 0; i < m_pDataProvider->m_vectVideo.size();i++)      //摄像头//
    {
		if (m_pDataProvider->m_vectVideo[i].m_ModuleId==nCerSelModuleId)
		{
			m_vectTempVideo.push_back(m_pDataProvider->m_vectVideo[i]);
			break;
		}
    }
	for (int j = 0; j < m_pDataProvider->m_vectProModulePara.size();j++)    //工艺参数//
	{
		if (m_pDataProvider->m_vectProModulePara[j].m_ProcessModuleId==nCerSelModuleId && m_pDataProvider->m_vectProModulePara[j].m_IsVisible)
		{
			m_vectTempModulePara.push_back(m_pDataProvider->m_vectProModulePara[j]);
		}
	}
}

//填写工艺参数列表框//
void CDeviceChiDlg::WriteToModuleList()
{
	m_listModulePara.DeleteAllItems();
	CString strItem;                                               //模块下的工艺参数显示在list1//
	for (int n = 0; n < m_vectTempModulePara.size(); n++)
	{
		strItem.Format(_T("%d"), n + 1);
		m_listModulePara.InsertItem(n, _T(""));
		m_listModulePara.SetItemText(n, 1, strItem);
		m_listModulePara.SetItemText(n, 2, m_vectTempModulePara[n].m_strParaName);
		m_listModulePara.SetItemText(n, 5, m_vectTempModulePara[n].m_strUnit);
	}  
}   

//填写设备参数列表框//
void CDeviceChiDlg::WriteToDeviceList()                          
{
	m_listDevicePara.DeleteAllItems();
	CString strItem;
	for (int n = 0; n < m_vectTempDevicePara.size(); n++)
	{
		m_listDevicePara.InsertItem(n, _T(""));
		strItem.Format(_T("%d"), n + 1);
		m_listDevicePara.SetItemText(n, 1, strItem);
		m_listDevicePara.SetItemText(n, 2, m_vectTempDevicePara[n].m_strDeviceName);
	}
}

void CDeviceChiDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	//CRect rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect, RGB(0, 0, 250));   //设置为绿色背景
	// 不为绘图消息调用 CDialog::OnPaint()
}


HBRUSH CDeviceChiDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//双击设备参数列表的响应函数//
void CDeviceChiDlg::OnNMDblclkLi2Devicepara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	m_nDClkLineItem = pNMItemActivate->iItem;
	if (m_nDClkLineItem == -1)
		return;
	if (!IsPlcConnected(m_vectTempDevicePara[m_nDClkLineItem].m_PlcId))
	{
		AfxMessageBox(_T("请先连接设备！"));
		return;
	}
	CDeviceContrlPopDlg DeviceContrlPopDlg;          //设备控制台弹出窗口//
	DeviceContrlPopDlg.m_vDevicePara.push_back(m_vectTempDevicePara[m_nDClkLineItem]);        //将选中设备参数传到弹出界面//
	DeviceContrlPopDlg.DoModal();

	*pResult = 0;
}

//双击工艺参数列表的响应函数//
void CDeviceChiDlg::OnNMDblclkLi1Modulepara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO:  在此添加控件通知处理程序代码//
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pEditCtrl->iItem == -1)                                                //点击到非工作区//
	{
		if (m_bHaveEditCreate == true)                                            //如果之前创建了编辑框就销毁掉//
		{
			distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);        //销毁单元格编辑框对象//
			m_bHaveEditCreate = false;
		}
	}
	else
	{
		if (m_bHaveEditCreate == true)
		{
			if (!(m_OldItem == pEditCtrl->iItem && m_OldSubItem == pEditCtrl->iSubItem))    //如果点中的单元格不是之前创建好的//
			{
				distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);
				m_bHaveEditCreate = false;
				createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);      //创建编辑框//
			}
			else                                                                       //点中的单元格是之前创建好的//
			{
				m_Edit.SetFocus();                                                     //设置为焦点//
			}
		}
		else
		{
			if (!IsPlcConnected(m_vectTempModulePara[pEditCtrl->iItem].m_PlcId))      //如果该参数所属PLC未连接//
			{
				AfxMessageBox(_T("请先连接设备！"));
				return;
			}
			m_OldItem = pEditCtrl->iItem;                                                 //将点中的单元格的行赋值给“刚编辑过的行”以便后期处理//
			m_OldSubItem = pEditCtrl->iSubItem;                                           //将点中的单元格的行赋值给“刚编辑过的列”以便后期处理//
			createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);         //创建编辑框//
		}
	}
	*pResult = 0;
}

//创建单元格编辑框函数//
void CDeviceChiDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)
//pEditCtrl为列表对象指针，createdit为编辑框指针对象//
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准//
{
	Item = pEditCtrl->iItem;                     //将点中的单元格的行赋值给“刚编辑过的行”以便后期处理//
	SubItem = pEditCtrl->iSubItem;               //将点中的单元格的行赋值给“刚编辑过的列”以便后期处理//
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_CENTER | ES_WANTRETURN,CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);  //创建编辑框对象,IDC_EDIT_CREATEID为控件ID号//
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);                   //设置字体//
	createdit->SetParent(&m_listModulePara);                      //将listcontrol设置为父窗口//
	CRect  EditRect;
	m_listModulePara.GetSubItemRect(m_OldItem, 4, LVIR_LABEL, EditRect);      //获取单元格的空间位置信息//
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_listModulePara.GetColumnWidth(4) - 1, EditRect.bottom - 1);//+1和-1让编辑框不至于挡住列表框中的网格线
	CString strItem = m_listModulePara.GetItemText(m_OldItem, 4);             //获得当前设定值//
	createdit->SetWindowText(strItem);                                     //将单元格字符显示在编辑框上//
	createdit->MoveWindow(&EditRect);                                      //将编辑框位置放在相应单元格上//
	createdit->ShowWindow(SW_SHOW);                                        //显示编辑框在单元格上面//
	createdit->SetFocus();                                                 //设置为焦点//
	createdit->SetSel(-1);                                                 //设置光标在文本框文字的最后//
}

void CDeviceChiDlg::distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem)
{
	CString strEditData;
	distroyedit->GetWindowText(strEditData);
	strEditData = strEditData.Trim();
	if (strEditData.IsEmpty())             //如果输入为空，则销毁编辑框//
	{
		AfxMessageBox(_T("非法操作，输入不能为空！"));
		distroyedit->DestroyWindow();
		return;
	}
	if (m_vectTempModulePara[Item].m_ValueType == CProcessPara::VALUETYPE_BOOL && (strEditData != _T("0") && strEditData != _T("1")))
	{
		AfxMessageBox(_T("非法操作，该参数值只能为“0”或“1”！"));
		distroyedit->DestroyWindow();                          //销毁对象//
		return;
	}
	if (strEditData == strEditData.SpanIncluding(_T("0123456789.")))
	{
		if(IDYES==MessageBox(_T("是否保存该修改？"), _T("提示"), MB_ICONQUESTION | MB_YESNO))
		{
			//list->SetItemText(Item, 4, strEditData);      //将修改写入列表框//
			//待添加，将修改写入队列//
			CPlcWriteRequest tempRequest;
			switch (m_vectTempModulePara[Item].m_ValueType)   //根据数据类型赋值//
			{
			case 0:
				if (strEditData==_T("0"))
					tempRequest.m_SetValue = 0;
				else
					tempRequest.m_SetValue = 1;
				break;
			case 1:
			case 2:
			case 3:
				tempRequest.m_SetValue = (float)_wtof(strEditData);
				break;
			case 4:
			case 5:
				tempRequest.m_fSetValue = (float)_wtof(strEditData);
				break;
			default:
				break;
			}
			tempRequest.m_PlcId = m_vectTempModulePara[Item].m_PlcId;
			tempRequest.m_AddrType = m_vectTempModulePara[Item].GetAddrTypeInEmType();
			tempRequest.m_ByteOffset = m_vectTempModulePara[Item].GetWriteBitOffSet();
			tempRequest.m_WriteByteAddrIndex = m_vectTempModulePara[Item].GetWriteAddrByteIndex();
			m_pDataProvider->m_wRequestQueue.PushBack(tempRequest);
		}
	}             
	else
		AfxMessageBox(_T("非法操作，请输入数字！"));
	distroyedit->DestroyWindow();                          //销毁对象//
}

//重载虚函数//
void CDeviceChiDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	m_listModulePara.SetFocus();
}

//编辑框失去焦点//
void CDeviceChiDlg::OnEnKillfocusEditCreateid()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bHaveEditCreate == true)                                               //如果之前创建了编辑框就销毁掉//
	{
		distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);           //销毁单元格编辑框对象//
		m_bHaveEditCreate = false;
	}
}

void CDeviceChiDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
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

void CDeviceChiDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
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

UINT CDeviceChiDlg::ThreadFunc(LPVOID lpParam)
{
	((CDeviceChiDlg*)lpParam)->m_video.SetDeviceInfo(((CDeviceChiDlg*)lpParam)->m_vectTempVideo[0].m_strIPAddr, ((CDeviceChiDlg*)lpParam)->m_vectTempVideo[0].m_port, 0, _T("admin"), _T("admin"));
	((CDeviceChiDlg*)lpParam)->m_video.StartPlay();
	return 1;
}

void CDeviceChiDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 3)
	{
		if (!m_vectTempModulePara.empty())
		    UpdateModuleParaValueShow();            //更新工艺参数列表框//
		if (!m_vectTempDevicePara.empty())
			UpdateDeviceParaValueShow();           //更新设备参数列表框//
	}  

	CDialog::OnTimer(nIDEvent);
}


void CDeviceChiDlg::UpdateModuleParaValueShow() //更新工艺参数的当前值和设定值//
{
	//m_listModulePara.SetRedraw(FALSE);
	CString strTempSetValue, strTempStateValue,strCurSetValue,strCurStateValue;
	int nTempSetValue, nTempStateValue;
	for (int n = 0; n < m_vectTempModulePara.size(); n++)
	{
		for (int i = 0; i < m_pDataProvider->m_vectProModulePara.size(); i++)
		{
			if (m_vectTempModulePara[n].m_Id == m_pDataProvider->m_vectProModulePara[i].m_Id)
			{
				strCurSetValue = m_listModulePara.GetItemText(n, 4);
				strCurStateValue = m_listModulePara.GetItemText(n, 3);
				if (IsPlcConnected(m_pDataProvider->m_vectProModulePara[i].m_PlcId))   //判断PLC是否连接//
                {
					switch (m_pDataProvider->m_vectProModulePara[i].m_ValueType)     //根据值类型填写列表框//
					{
					case 0:
						if (m_pDataProvider->m_vectProModulePara[i].m_ParaValue == 0)
						{
							if (strCurStateValue != _T("0"))
								m_listModulePara.SetItemText(n, 3, _T("0"));
						}
						else
							if (strCurStateValue != _T("1"))
							    m_listModulePara.SetItemText(n, 3, _T("1"));

						if (m_pDataProvider->m_vectProModulePara[i].m_fSetValue == 0)
						{
							if (strCurSetValue != _T("0"))
								m_listModulePara.SetItemText(n, 4, _T("0"));
						}
						else
							if (strCurSetValue != _T("1"))
							    m_listModulePara.SetItemText(n, 4, _T("1"));

						break;
					case 1:
					case 2:
					case 3:
						nTempSetValue = m_pDataProvider->m_vectProModulePara[i].m_fSetValue;
						nTempStateValue = m_pDataProvider->m_vectProModulePara[i].m_ParaValue;
						strTempSetValue.Format(_T("%d"), nTempSetValue);
						strTempStateValue.Format(_T("%d"), nTempStateValue);
						if (strCurStateValue!=strTempStateValue)
						    m_listModulePara.SetItemText(n, 3, strTempStateValue);
						if (strCurSetValue!=strTempSetValue)
						    m_listModulePara.SetItemText(n, 4, strTempSetValue);
						break;
					case 4:
					case 5:
						strTempSetValue.Format(_T("%.2f"), m_pDataProvider->m_vectProModulePara[i].m_fSetValue);
						strTempStateValue.Format(_T("%.2f"), m_pDataProvider->m_vectProModulePara[i].m_ParaValue);
						if (strCurStateValue != strTempStateValue)
						    m_listModulePara.SetItemText(n, 3, strTempStateValue);
						if (strCurSetValue != strTempSetValue)
						    m_listModulePara.SetItemText(n, 4, strTempSetValue);
						break;
					default:
						break;
					}
                }
				else
				{
					if (strCurStateValue != _T("未连接"))
					    m_listModulePara.SetItemText(n, 3, _T("未连接"));
					if (strCurSetValue != _T("未连接"))
					    m_listModulePara.SetItemText(n, 4, _T("未连接"));
				}
				break;
			}
		}
	}
	//m_listModulePara.SetRedraw(TRUE);
//	m_listModulePara.UpdateWindow();
}

bool CDeviceChiDlg::IsPlcConnected(int nPlcId)    //判断PLC是否连接//
{
	for (int n = 0; n < m_pDataProvider->m_vectPlc.size();n++)
		if (m_pDataProvider->m_vectPlc[n].m_Id==nPlcId)
			if (m_pDataProvider->m_vectPlc[n].m_bIsConnected)
				return true;
		    else
				return false;
	return false;
}

void CDeviceChiDlg::UpdateDeviceParaValueShow()    //更新设备启停状态//
{
	//m_listDevicePara.SetRedraw(FALSE);
	CString strCurValue;
	for (int n = 0; n < m_vectTempDevicePara.size();n++)
		for (int i = 0; i < m_pDataProvider->m_vectDevicePara.size();i++)
			if (m_vectTempDevicePara[n].m_Id == m_pDataProvider->m_vectDevicePara[i].m_Id)
			{
				strCurValue = m_listDevicePara.GetItemText(n, 3);
				if (IsPlcConnected(m_vectTempDevicePara[n].m_PlcId))          //判断该设备是否连接//
				{
					if (m_pDataProvider->m_vectDevicePara[i].m_StateValue)
					{
						if (strCurValue != _T("正常运行"))
							m_listDevicePara.SetItemText(n, 3, _T("正常运行"));
					}
					else
					{
						if (strCurValue != _T("停止"))
							m_listDevicePara.SetItemText(n, 3, _T("停止"));
					}
				}
				else
					if (strCurValue != _T("未连接"))
					    m_listDevicePara.SetItemText(n, 3, _T("未连接"));
				break;
			}
	//m_listDevicePara.SetRedraw(TRUE);
	//m_listDevicePara.UpdateWindow();
}

//导航栏选中条目即将发生变化//
void CDeviceChiDlg::OnTvnSelchangingTreeDeviceChidlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (m_bHaveEditCreate == true)                                               //如果之前创建了编辑框就销毁掉//
	{
		distroyEdit(&m_listModulePara, &m_Edit, m_OldItem, m_OldSubItem);           //销毁单元格编辑框对象//
		m_bHaveEditCreate = false;
	}
	*pResult = 0;
}
