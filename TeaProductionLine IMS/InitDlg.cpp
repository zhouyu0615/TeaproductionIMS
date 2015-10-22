// InitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "InitDlg.h"
#include "afxdialogex.h"
#include  "Resource.h"


// CInitDlg 对话框

IMPLEMENT_DYNAMIC(CInitDlg, CDialog)

CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
: CDialog(CInitDlg::IDD, pParent)
{
	m_InitTag = OWER_EDIT_TAG;
	//m_pDataProvider->InitDataProvider();
}

CInitDlg::~CInitDlg()
{
}

void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED1_INITDLG, m_EditText1);
	DDX_Control(pDX, IDC_ED2_INITDLG, m_EditText2);
	DDX_Control(pDX, IDC_ED3_INITDLG, m_EditText3);
	DDX_Control(pDX, IDC_ED4_INITDLG, m_EditText4);
	DDX_Control(pDX, IDC_ED5_INITDLG, m_EditText5);
	DDX_Control(pDX, IDC_ST_ADDCONTENT_INITDLG, m_GroupBox);
	DDX_Control(pDX, IDC_LI_INITDLG, m_list_init);
	DDX_Control(pDX, IDC_CO_LINE_INITDLG, m_LineComboBox);
	DDX_Control(pDX, IDC_CO_MODULE_INITDLG, m_ModuleComboBox);
	DDX_Control(pDX, IDC_ST1_INITDLG, m_StaticText1);
	DDX_Control(pDX, IDC_ST2_INITDLG, m_StaticText2);
	DDX_Control(pDX, IDC_ST3_INITDLG, m_StaticText3);
	DDX_Control(pDX, IDC_ST4_INITDLG, m_StaticText4);
	DDX_Control(pDX, IDC_ST5_INITDLG, m_StaticText5);

}


BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_PLCPARA_INITDLG, &CInitDlg::OnBnClickedBtPlcparaInitdlg)
	ON_BN_CLICKED(IDC_BT_OWNER_INITDLG, &CInitDlg::OnBnClickedBtOwnerInitdlg)
	ON_BN_CLICKED(IDC_BT_LINE_INITDLG, &CInitDlg::OnBnClickedBtLineInitdlg)
	ON_BN_CLICKED(IDC_BT_MODULE_INITDLG, &CInitDlg::OnBnClickedBtModuleInitdlg)
	ON_BN_CLICKED(IDC_BT_DEVICE_INITDLG, &CInitDlg::OnBnClickedBtDeviceInitdlg)
	ON_BN_CLICKED(IDC_BT_PLC_INITDLG, &CInitDlg::OnBnClickedBtPlcInitdlg)
	ON_BN_CLICKED(IDC_BT_VEDIO_INITDLG, &CInitDlg::OnBnClickedBtVedioInitdlg)
	ON_BN_CLICKED(IDC_ADD_INITDLG, &CInitDlg::OnBnClickedAddInitdlg)
	ON_BN_CLICKED(IDC_BT_GOBACK_INITDLG, &CInitDlg::OnBnClickedBtGobackInitdlg)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_CO_LINE_INITDLG, &CInitDlg::OnCbnSelchangeCoLineInitdlg)
	ON_NOTIFY(NM_RCLICK, IDC_LI_INITDLG, &CInitDlg::OnNMRClickLiInitdlg)
END_MESSAGE_MAP()


// CInitDlg 消息处理程序

//“PLC变量”按钮响应函数//
void CInitDlg::OnBnClickedBtPlcparaInitdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	m_EditPlcParaDlg.DoModal();
}


void CInitDlg::OnBnClickedBtOwnerInitdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_InitTag != OWER_EDIT_TAG)
	{
		ClearEditCtrl();
	}
	m_InitTag = OWER_EDIT_TAG;
	MyOnPaint();

}


void CInitDlg::OnBnClickedBtLineInitdlg()
{
	if (m_pDataProvider->m_vectUser.empty())
	{
		AfxMessageBox(_T("尚未创建用户，请先添加用户！"));
		return;
	}
	if (m_InitTag != PRODUCTION_LINE_EDIT_TAG)
	{
		ClearEditCtrl();
	}
	m_InitTag = PRODUCTION_LINE_EDIT_TAG;
	MyOnPaint();

}


void CInitDlg::OnBnClickedBtModuleInitdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pDataProvider->m_vectUser.empty())
	{
		AfxMessageBox(_T("尚未创建用户，请先添加用户！"));
		return;
	}
	if (m_pDataProvider->m_vectProductionLine.empty())
	{
		AfxMessageBox(_T("请先添加生产线！"));
		return;
	}
	if (m_InitTag != MODULE_EDIT_TAG)
	{
		ClearEditCtrl();
	}
	m_InitTag = MODULE_EDIT_TAG;
	MyOnPaint();

}


void CInitDlg::OnBnClickedBtDeviceInitdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pDataProvider->m_vectUser.empty())
	{
		AfxMessageBox(_T("尚未创建所有者，请先创建！"));
		return;
	}
	if (m_pDataProvider->m_vectProductionLine.empty())
	{
		AfxMessageBox(_T("请先添加生产线！"));
		return;
	}
	if (m_pDataProvider->m_vectProcessModule.empty())
	{
		AfxMessageBox(_T("请先添加工艺模块！"));
		return;
	}
	if (m_InitTag != DEVICE_EDIT_TAG)
	{
		ClearEditCtrl();
	}
	m_InitTag = DEVICE_EDIT_TAG;
	MyOnPaint();

}


void CInitDlg::OnBnClickedBtPlcInitdlg()
{
	if (m_pDataProvider->m_vectUser.empty())
	{
		AfxMessageBox(_T("尚未创建用户，请先添加用户！"));
		return;
	}
	if (m_pDataProvider->m_vectProductionLine.empty())
	{
		AfxMessageBox(_T("请先添加生产线！"));
		return;
	}
	if (m_InitTag != PLC_EDIT_TAG)
	{
		this->ClearEditCtrl();
	}
	m_InitTag = PLC_EDIT_TAG;
	MyOnPaint();
}


void CInitDlg::OnBnClickedBtVedioInitdlg()
{
	// TODO:  在此添加控件通知处理程序代码


	if (m_pDataProvider->m_vectUser.empty())
	{
		AfxMessageBox(_T("尚未创建所有者，请先添加所有者！"));
		return;
	}
	if (m_pDataProvider->m_vectProductionLine.empty())
	{
		AfxMessageBox(_T("请先添加生产线！"));
		return;
	}


	m_InitTag = VIDEO_EDIT_TAG;
	MyOnPaint();
}


void CInitDlg::OnBnClickedAddInitdlg()
{
	CString text1, text2, text3, text4, text5, text6;
	CUserClass tempUser;
	CProductionLineClass tempProLine;
	CProcessModuleClass tempProModule;
	CDeviceClass tempDevice;
	CPlcClass tempPlc;
	CVideoClass tempVideo;


	m_EditText1.GetWindowText(text1);
	m_EditText2.GetWindowText(text2);
	m_EditText3.GetWindowText(text3);
	m_EditText4.GetWindowText(text4);
	m_EditText5.GetWindowText(text5);


	switch (m_InitTag){
	case OWER_EDIT_TAG:
		if (!m_pDataProvider->m_vectUser.empty())            //判断是否已添加过用户
		{
			AfxMessageBox(_T("非法操作，用户已创建！"));
			return;
		}

		if (text1.IsEmpty() || text2.IsEmpty() || text3.IsEmpty())
		{
			AfxMessageBox(_T("信息不完善，无法添加！"));
			return;
		}

		tempUser.m_strUserName = text1;
		tempUser.m_strUserPasswd = text2;
		tempUser.m_strUserCode = text3;
		tempUser.m_strNote = text4;

		m_pDataProvider->AddUserToDatabase(tempUser);

		break;
	case PRODUCTION_LINE_EDIT_TAG:
		if (!m_pDataProvider->m_vectProductionLine.empty())                 //检测该生产线名称是否已经存在//
			for (int q = 0; q < m_pDataProvider->m_vectProductionLine.size(); q++)
				if (text1 == m_pDataProvider->m_vectProductionLine[q].m_strLineName)
				{
					AfxMessageBox(_T("非法操作，该生产线名称已存在！"));
					return;
				}

		if (text1.IsEmpty() || text2.IsEmpty())
		{
			AfxMessageBox(_T("信息不完善，无法添加！"));
			return;
		}

		tempProLine.m_strLineName = text1;
		tempProLine.m_strCapacity = text2;
		tempProLine.m_strDescription = text3;
		 
		m_pDataProvider->AddProLineToDatabase(tempProLine);


		break;
	case MODULE_EDIT_TAG:

		m_LineComboBox.GetWindowText(text1); //text1所属生产线，text2模块名称

		if (text1.IsEmpty() || text2.IsEmpty())                   //判断所录入工艺模块信息是否完全//
		{
			AfxMessageBox(_T("信息不完善，无法添加！"));
			return;
		}

		//检测该生产线下工艺模块名称是否已经存在//
		for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)
			if (text2 == m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName)
			{
				if (text1 == m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName)
				{
					AfxMessageBox(_T("非法操作，该生产线下的同名工艺模块已存在！"));
					return;
				}

			}


		tempProModule.m_strProductionLineName = text1;
		tempProModule.m_strProcessModuleName = text2;
		tempProModule.m_strDescription = text3;

		m_pDataProvider->AddProModuleToDatabase(tempProModule);
		break;
	case DEVICE_EDIT_TAG:

		m_LineComboBox.GetWindowText(text1);
		m_ModuleComboBox.GetWindowText(text2);

		if (text1.IsEmpty() || text2.IsEmpty() || text3.IsEmpty() || text4.IsEmpty())  //判断录入信息是否完整//
		{
			AfxMessageBox(_T("信息不完善，无法添加！"));
			return;
		}

		tempDevice.m_strProductionLineName = text1;
		tempDevice.m_strProcessModuleName = text2;
		tempDevice.m_strDeviceName = text3;
		tempDevice.m_strDeviceType = text4;


		m_pDataProvider->AddDeviceToDatabase(tempDevice);

		break;
	case PLC_EDIT_TAG:

		if (text1.IsEmpty() || text2.IsEmpty() || text3.IsEmpty()
			|| text4.IsEmpty())  //判断录入信息是否完整//
		{
			AfxMessageBox(_T("信息不完善，无法添加！"));
			return;
		}

		tempPlc.m_strPlcName = text1;
		tempPlc.m_strIPAddr = text2;
		tempPlc.m_ReadStartAddr = _ttoi(text3);
		tempPlc.m_ReadLength = _ttoi(text4);
		//tempPlc.m_WriteStartAddr = _ttoi(text5);
		//tempPlc.m_WriteLength = _ttoi(text6);


		//将添加的信息录入到容器//

		m_pDataProvider->AddPlcToDatabase(tempPlc);
		break;

	case VIDEO_EDIT_TAG:

		m_LineComboBox.GetWindowText(text1);
		m_ModuleComboBox.GetWindowText(text2);

		if (text1.IsEmpty() || text2.IsEmpty() || text3.IsEmpty() || text4.IsEmpty())  //判断录入信息是否完整//
		{
			AfxMessageBox(_T("信息不完善，无法添加！"));
			return;
		}
		tempVideo.m_strProductionLineName = text1;
		tempVideo.m_strProcessModuleName = text2;
		tempVideo.m_strVideoName = text3;
		tempVideo.m_strIPAddr = text4;

		tempVideo.m_port = _ttoi(text5);

		//将添加的信息录入到容器//


		m_pDataProvider->AddVideoToDatabase(tempVideo);
		break;

	default:
		break;
	}
	ClearEditCtrl();

	ListOnPaint();

}



void CInitDlg::OnBnClickedBtGobackInitdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


int CInitDlg::LineComboboxOnPaint()
{
	//如果已经添加了生产线信息而又没有填写到下拉框，则导入//
	if (!m_pDataProvider->m_vectProductionLine.empty())  {
		m_LineComboBox.ResetContent();
		for (int k = 0; k < m_pDataProvider->m_vectProductionLine.size(); k++)
		{
			m_LineComboBox.AddString(m_pDataProvider->m_vectProductionLine[k].m_strLineName);
			
		}
	}
	m_LineComboBox.SetCurSel(0);
	return 0;
}

int CInitDlg::ListOnPaint()
{
	//设置列表控件风格//
	CRect rect1;
	m_list_init.GetWindowRect(&rect1);
	m_list_init.SetExtendedStyle(m_list_init.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LBS_OWNERDRAWVARIABLE | LVS_EX_GRIDLINES);

	//清空列表//
	m_list_init.DeleteAllItems();
	CHeaderCtrl *pmyHeaderCtrl = m_list_init.GetHeaderCtrl();
	int nCount = pmyHeaderCtrl->GetItemCount();
	for (int i = nCount - 1; i >= 0; i--)
		m_list_init.DeleteColumn(i);

	if (!m_pDataProvider->m_vectUser.empty()
		&& !m_pDataProvider->m_vectProductionLine.empty()
		&& !m_pDataProvider->m_vectProcessModule.empty()) //如果已经添加了用户、生产线、模块，则使“添加摄像头”按钮可用//
		GetDlgItem(IDC_BT_VEDIO_INITDLG)->EnableWindow(TRUE);

	int temp = 0;
	LV_ITEM litem;
	litem.mask = LVIF_TEXT;
	litem.iSubItem = 0;
	litem.pszText = _T("");

	switch (m_InitTag){
	case OWER_EDIT_TAG:
		//初始化编辑区//
		m_GroupBox.SetWindowText(_T("添加所有者"));
		m_StaticText1.SetWindowText(_T("用 户 名："));
		m_StaticText2.SetWindowText(_T("登录密码："));
		m_StaticText3.SetWindowText(_T("客户编码："));
		m_StaticText4.SetWindowText(_T("备注说明："));

		m_StaticText1.ShowWindow(SW_SHOW);
		m_StaticText2.ShowWindow(SW_SHOW);
		m_StaticText3.ShowWindow(SW_SHOW);
		m_StaticText4.ShowWindow(SW_SHOW);
		m_StaticText5.ShowWindow(SW_HIDE);
	
		m_EditText1.ShowWindow(SW_SHOW);
		m_EditText2.ShowWindow(SW_SHOW);
		m_EditText3.ShowWindow(SW_SHOW);
		m_EditText4.ShowWindow(SW_SHOW);
		m_EditText5.ShowWindow(SW_HIDE);
		
		m_LineComboBox.ShowWindow(SW_HIDE);
		m_ModuleComboBox.ShowWindow(SW_HIDE);
		//初始化列表区//
		//列标编辑//
		m_list_init.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);
		m_list_init.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 14, -1);
		m_list_init.InsertColumn(2, _T("用户名"), LVCFMT_CENTER, rect1.Width() / 14 * 3, -1);
		m_list_init.InsertColumn(3, _T("登录密码"), LVCFMT_CENTER, rect1.Width() / 14 * 3, -1);
		m_list_init.InsertColumn(4, _T("客户编码"), LVCFMT_CENTER, rect1.Width() / 14 * 2, -1);
		m_list_init.InsertColumn(5, _T("备注说明"), LVCFMT_CENTER, rect1.Width() / 14 * 5, -1);
		//填写表单内容//

		temp = m_pDataProvider->m_vectUser.size();
		for (int i = 0; i < temp; i++)
		{
			litem.iItem = i;
			CString str;
			str.Format(_T("%d"), i + 1);
			m_list_init.InsertItem(&litem);
			m_list_init.SetItemText(i, 1, str);
			m_list_init.SetItemText(i, 2, m_pDataProvider->m_vectUser[i].m_strUserName);
			m_list_init.SetItemText(i, 3, m_pDataProvider->m_vectUser[i].m_strUserPasswd);
			m_list_init.SetItemText(i, 4, m_pDataProvider->m_vectUser[i].m_strUserCode);
			m_list_init.SetItemText(i, 5, m_pDataProvider->m_vectUser[i].m_strNote);
		}
		break;


	case PRODUCTION_LINE_EDIT_TAG:
		//初始化编辑区//

		m_GroupBox.SetWindowText(_T("添加生产线"));
		m_StaticText1.SetWindowText(_T("生产线名称："));
		m_StaticText2.SetWindowText(_T("产能  产量："));
		m_StaticText3.SetWindowText(_T("备注  说明："));


		m_StaticText1.ShowWindow(SW_SHOW);
		m_StaticText2.ShowWindow(SW_SHOW);
		m_StaticText3.ShowWindow(SW_SHOW);
		m_StaticText4.ShowWindow(SW_HIDE);
		m_StaticText5.ShowWindow(SW_HIDE);
		
		m_EditText1.ShowWindow(SW_SHOW);
		m_EditText2.ShowWindow(SW_SHOW);
		m_EditText3.ShowWindow(SW_SHOW);
		m_EditText4.ShowWindow(SW_HIDE);
		m_EditText5.ShowWindow(SW_HIDE);
		
		m_LineComboBox.ShowWindow(SW_HIDE);
		m_ModuleComboBox.ShowWindow(SW_HIDE);

		//初始化列表区//
		m_list_init.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);       //列标编辑//
		m_list_init.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 10, -1);
		m_list_init.InsertColumn(2, _T("生产线名称"), LVCFMT_CENTER, rect1.Width() / 10 * 4, -1);
		m_list_init.InsertColumn(3, _T("产能产量"), LVCFMT_CENTER, rect1.Width() / 10 * 2, -1);
		m_list_init.InsertColumn(4, _T("备注说明"), LVCFMT_CENTER, rect1.Width() / 10 * 3, -1);
		//填写表单内容//

		temp = m_pDataProvider->m_vectProductionLine.size();
		for (int i = 0; i < temp; i++)
		{
			litem.iItem = i;
			CString str;
			str.Format(_T("%d"), i + 1);
			m_list_init.InsertItem(&litem);
			m_list_init.SetItemText(i, 1, str); //序号
			m_list_init.SetItemText(i, 2, m_pDataProvider->m_vectProductionLine[i].m_strLineName);
			m_list_init.SetItemText(i, 3, m_pDataProvider->m_vectProductionLine[i].m_strCapacity);
			m_list_init.SetItemText(i, 4, m_pDataProvider->m_vectProductionLine[i].m_strDescription);
		}
		break;


	case MODULE_EDIT_TAG:
		//初始化编辑区//
		m_GroupBox.SetWindowText(_T("添加工艺模块"));
		m_StaticText1.SetWindowText(_T("所属  生产线："));
		m_StaticText2.SetWindowText(_T("工艺模块名称："));
		m_StaticText3.SetWindowText(_T("备 注  说 明："));


		m_StaticText1.ShowWindow(SW_SHOW);
		m_StaticText2.ShowWindow(SW_SHOW);
		m_StaticText3.ShowWindow(SW_SHOW);
		m_StaticText4.ShowWindow(SW_HIDE);
		m_StaticText5.ShowWindow(SW_HIDE);
		

		m_EditText1.ShowWindow(SW_HIDE);
		m_EditText2.ShowWindow(SW_SHOW);
		m_EditText3.ShowWindow(SW_SHOW);
		m_EditText4.ShowWindow(SW_HIDE);
		m_EditText5.ShowWindow(SW_HIDE);
		

		m_LineComboBox.ShowWindow(SW_SHOW);
		m_ModuleComboBox.ShowWindow(SW_HIDE);



		//初始化列表区//
		m_list_init.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);       //列标编辑//
		m_list_init.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 10, -1);
		m_list_init.InsertColumn(2, _T("所属生产线"), LVCFMT_CENTER, rect1.Width() / 10 * 3, -1);
		m_list_init.InsertColumn(3, _T("工艺模块名称"), LVCFMT_CENTER, rect1.Width() / 10 * 3, -1);
		m_list_init.InsertColumn(4, _T("备注 说明"), LVCFMT_CENTER, rect1.Width() / 10 * 3, -1);

		//填写表单内容//
		temp = m_pDataProvider->m_vectProcessModule.size();
		for (int i = 0; i < temp; i++)
		{
			litem.iItem = i;
			CString str;
			str.Format(_T("%d"), i + 1);
			m_list_init.InsertItem(&litem);
			m_list_init.SetItemText(i, 1, str); //序号
			m_list_init.SetItemText(i, 2, m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName);
			m_list_init.SetItemText(i, 3, m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
			m_list_init.SetItemText(i, 4, m_pDataProvider->m_vectProcessModule[i].m_strDescription);
		}
		break;


	case DEVICE_EDIT_TAG:
		//初始化编辑区//
		m_GroupBox.SetWindowText(_T("添加设备"));
		m_StaticText1.SetWindowText(_T("所属  生产线："));
		m_StaticText2.SetWindowText(_T("所属工艺模块："));
		m_StaticText3.SetWindowText(_T("设 备  名 称："));
		m_StaticText4.SetWindowText(_T("设 备  类 型："));

		m_StaticText1.ShowWindow(SW_SHOW);
		m_StaticText2.ShowWindow(SW_SHOW);
		m_StaticText3.ShowWindow(SW_SHOW);
		m_StaticText4.ShowWindow(SW_SHOW);
		m_StaticText5.ShowWindow(SW_HIDE);
		

		m_EditText1.ShowWindow(SW_HIDE);
		m_EditText2.ShowWindow(SW_HIDE);
		m_EditText3.ShowWindow(SW_SHOW);
		m_EditText4.ShowWindow(SW_SHOW);
		m_EditText5.ShowWindow(SW_HIDE);
		

		m_LineComboBox.ShowWindow(SW_SHOW);
		m_ModuleComboBox.ShowWindow(SW_SHOW);

		if (m_LineComboBox.GetCount() > 0)
		{
			if (m_ModuleComboBox.GetCount() > 0)    //如果COMBO2已有内容，先清空//
				m_ModuleComboBox.ResetContent();

			temp = m_LineComboBox.GetCurSel();
			CString str1;
			m_LineComboBox.GetLBText(temp, str1);//得到COMBO1当前选中条目//
			for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)			           //对应填写COMBO2内容//
				if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == str1)
				{
					m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
					m_ModuleComboBox.SetCurSel(0);
				}
		}
		m_ModuleComboBox.ShowWindow(SW_SHOW);


		//初始化列表区//
		m_list_init.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);       //编辑表头//
		m_list_init.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 11, -1);
		m_list_init.InsertColumn(2, _T("所属生产线"), LVCFMT_CENTER, rect1.Width() / 11 * 3, -1);
		m_list_init.InsertColumn(3, _T("所属工艺模块"), LVCFMT_CENTER, rect1.Width() / 11 * 3, -1);
		m_list_init.InsertColumn(4, _T("设备名称"), LVCFMT_CENTER, rect1.Width() / 11 * 2, -1);
		m_list_init.InsertColumn(5, _T("设备类型"), LVCFMT_CENTER, rect1.Width() / 11 * 2, -1);



		//填写表单内容//
		temp = m_pDataProvider->m_vectDevice.size();
		for (int i = 0; i < temp; i++)
		{
			litem.iItem = i;
			CString str;
			str.Format(_T("%d"), i + 1);
			m_list_init.InsertItem(&litem);
			m_list_init.SetItemText(i, 1, str); //序号
			m_list_init.SetItemText(i, 2, m_pDataProvider->m_vectDevice[i].m_strProductionLineName);
			m_list_init.SetItemText(i, 3, m_pDataProvider->m_vectDevice[i].m_strProcessModuleName);
			m_list_init.SetItemText(i, 4, m_pDataProvider->m_vectDevice[i].m_strDeviceName);
			m_list_init.SetItemText(i, 5, m_pDataProvider->m_vectDevice[i].m_strDeviceType);
		}
		break;

	case PLC_EDIT_TAG:
		//初始化编辑区//
		m_GroupBox.SetWindowText(_T("添加PLC"));
		m_StaticText1.SetWindowText(_T("PLC 名称："));
		m_StaticText2.SetWindowText(_T("IP  地址："));
		m_StaticText3.SetWindowText(_T("起始地址："));
		m_StaticText4.SetWindowText(_T("数据长度："));
		//m_StaticText5.SetWindowText(_T("写起始地址："));
		//m_StaticText6.SetWindowText(_T("写数据长度："));

		m_StaticText1.ShowWindow(SW_SHOW);
		m_StaticText2.ShowWindow(SW_SHOW);
		m_StaticText3.ShowWindow(SW_SHOW);
		m_StaticText4.ShowWindow(SW_SHOW);
		m_StaticText5.ShowWindow(SW_HIDE);
		
		m_EditText1.ShowWindow(SW_SHOW);
		m_EditText2.ShowWindow(SW_SHOW);
		m_EditText3.ShowWindow(SW_SHOW);
		m_EditText4.ShowWindow(SW_SHOW);
		m_EditText5.ShowWindow(SW_HIDE);
		

		m_LineComboBox.ShowWindow(SW_HIDE);
		m_ModuleComboBox.ShowWindow(SW_HIDE);



		//初始化列表区//
		m_list_init.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);       //列标编辑//
		m_list_init.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 11, -1);
		m_list_init.InsertColumn(2, _T("PLC名称"), LVCFMT_CENTER, rect1.Width() / 11 * 3, -1);
		m_list_init.InsertColumn(3, _T("IP地址"), LVCFMT_CENTER, rect1.Width() / 11 * 3, -1);
		m_list_init.InsertColumn(4, _T("起始地址"), LVCFMT_CENTER, rect1.Width() / 11 * 2, -1);
		m_list_init.InsertColumn(5, _T("数据长度"), LVCFMT_CENTER, rect1.Width() / 11 * 2, -1);
		//m_list_init.InsertColumn(6, _T("写起始地址"), LVCFMT_CENTER, rect1.Width() / 15 * 2, -1);
		//m_list_init.InsertColumn(7, _T("写数据长度"), LVCFMT_CENTER, rect1.Width() / 15 * 2, -1);

		//填写表单内容//
		temp = m_pDataProvider->m_vectPlc.size();
		for (int i = 0; i < temp; i++)
		{
			litem.iItem = i;
			CString str;
			str.Format(_T("%d"), i + 1);
			m_list_init.InsertItem(&litem);
			m_list_init.SetItemText(i, 1, str); //序号
			m_list_init.SetItemText(i, 2, m_pDataProvider->m_vectPlc[i].m_strPlcName);
			m_list_init.SetItemText(i, 3, m_pDataProvider->m_vectPlc[i].m_strIPAddr);

			str.Format(_T("%d"), m_pDataProvider->m_vectPlc[i].m_ReadStartAddr);
			m_list_init.SetItemText(i, 4, str);
			str.Format(_T("%d"), m_pDataProvider->m_vectPlc[i].m_ReadLength);
			m_list_init.SetItemText(i, 5, str);
			//str.Format(_T("%d"), m_pDataProvider->m_vectPlc[i].m_WriteStartAddr);
			//m_list_init.SetItemText(i, 6, str);
			//str.Format(_T("%d"), m_pDataProvider->m_vectPlc[i].m_WriteLength);
			//m_list_init.SetItemText(i, 7, str);
		}
		break;

	case VIDEO_EDIT_TAG:
		//初始化编辑区//
		m_GroupBox.SetWindowText(_T("添加摄像头"));
		m_StaticText1.SetWindowText(_T("所属  生产线："));
		m_StaticText2.SetWindowText(_T("所属工艺模块："));
		m_StaticText3.SetWindowText(_T("摄像头  名称："));
		m_StaticText4.SetWindowText(_T("IP   地   址："));
		m_StaticText5.SetWindowText(_T("端        口："));


		m_StaticText1.ShowWindow(SW_SHOW);
		m_StaticText2.ShowWindow(SW_SHOW);
		m_StaticText3.ShowWindow(SW_SHOW);
		m_StaticText4.ShowWindow(SW_SHOW);
		m_StaticText5.ShowWindow(SW_SHOW);
		

		m_EditText1.ShowWindow(SW_HIDE);
		m_EditText2.ShowWindow(SW_HIDE);
		m_EditText3.ShowWindow(SW_SHOW);
		m_EditText4.ShowWindow(SW_SHOW);
		m_EditText5.ShowWindow(SW_SHOW);
	


		m_LineComboBox.ShowWindow(SW_SHOW);
		m_ModuleComboBox.ShowWindow(SW_SHOW);

		if (m_LineComboBox.GetCount() > 0)
		{
			if (m_ModuleComboBox.GetCount() > 0)    //如果COMBO2已有内容，先清空//
				m_ModuleComboBox.ResetContent();
			temp = m_LineComboBox.GetCurSel();
			CString str1;
			m_LineComboBox.GetLBText(temp, str1);//得到COMBO1当前选中条目// 
			for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)			           //对应填写COMBO2内容//
				if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == str1)
				{
					m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
					m_ModuleComboBox.SetCurSel(0);
				}
		}

		//初始化列表区//
		m_list_init.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, -1);       //编辑表头//
		m_list_init.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rect1.Width() / 10, -1);
		m_list_init.InsertColumn(2, _T("所属生产线"), LVCFMT_CENTER, rect1.Width() / 10 * 2, -1);
		m_list_init.InsertColumn(3, _T("所属工艺模块"), LVCFMT_CENTER, rect1.Width() / 10 * 2, -1);
		m_list_init.InsertColumn(4, _T("摄像头名称"), LVCFMT_CENTER, rect1.Width() / 10 * 2, -1);
		m_list_init.InsertColumn(5, _T("IP地址"), LVCFMT_CENTER, rect1.Width() / 10 * 2, -1);
		m_list_init.InsertColumn(6, _T("端口"), LVCFMT_CENTER, rect1.Width() / 10 * 1, -1);
		//填写表单内容//
		temp = m_pDataProvider->m_vectVideo.size();

		for (int i = 0; i < temp; i++)
		{
			CString str, strPort;
			litem.iItem = i;
			str.Format(_T("%d"), i + 1);
			m_list_init.InsertItem(&litem);
			m_list_init.SetItemText(i, 1, str); //序号
			m_list_init.SetItemText(i, 2, m_pDataProvider->m_vectVideo[i].m_strProductionLineName);
			m_list_init.SetItemText(i, 3, m_pDataProvider->m_vectVideo[i].m_strProcessModuleName);
			m_list_init.SetItemText(i, 4, m_pDataProvider->m_vectVideo[i].m_strVideoName);
			m_list_init.SetItemText(i, 5, m_pDataProvider->m_vectVideo[i].m_strIPAddr);

			strPort.Format(_T("%d"), m_pDataProvider->m_vectVideo[i].m_port);
			m_list_init.SetItemText(i, 6, strPort);
		}
		break;

	default:
		AfxMessageBox(_T("系统错误！"));
		CDialog::OnCancel();
		break;
	}

	return 0;
}

void CInitDlg::MyOnPaint()
{
	LineComboboxOnPaint();
	ListOnPaint();
}

void CInitDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

}




void CInitDlg::ClearEditCtrl()
{
	m_EditText1.Clear();
	m_EditText2.Clear();
	m_EditText3.Clear();
	m_EditText4.Clear();
	m_EditText5.Clear();
	

	m_EditText1.SetWindowText(_T(""));
	m_EditText2.SetWindowText(_T(""));
	m_EditText3.SetWindowText(_T(""));
	m_EditText4.SetWindowText(_T(""));
	m_EditText5.SetWindowText(_T(""));
	
}





void CInitDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO:  在此处添加消息处理程序代码

}


int CInitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


BOOL CInitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	MyOnPaint();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CInitDlg::OnCbnSelchangeCoLineInitdlg()
{
	if (m_ModuleComboBox.GetCount() > 0)    //如果COMBO2已有内容，先清空//
		m_ModuleComboBox.ResetContent();
	CString str1;
	m_LineComboBox.GetWindowText(str1); // 得到COMBO1当前选中条目//
	for (int i = 0; i < m_pDataProvider->m_vectProcessModule.size(); i++)			           //对应填写COMBO2内容//
		if (m_pDataProvider->m_vectProcessModule[i].m_strProductionLineName == str1)
		{
			m_ModuleComboBox.AddString(m_pDataProvider->m_vectProcessModule[i].m_strProcessModuleName);
			m_ModuleComboBox.SetCurSel(0);
		}

}


void CInitDlg::OnNMRClickLiInitdlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	
	m_nSelectedItem = -1;
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		m_nSelectedItem = lpNMItemActivate->iItem;
	}
	if (m_nSelectedItem == -1)
	{
		return;
	}
	m_list_init.SetItemState(m_nSelectedItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	CMenu menu, *pSubMenu;
	menu.LoadMenu(IDR_INITDLG_POP_MENU);
	pSubMenu = menu.GetSubMenu(0);
	CPoint point1;        //存储鼠标位置的临时变量//
	GetCursorPos(&point1);//得到光标处//
	UINT nItem1 = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_TOPALIGN, point1.x, point1.y, GetParent());//确保右键点击在哪菜单出现在哪，并返回所选的菜单项//

	switch (nItem1)
	{
	case ID_AA_MODIFY:  //右键菜单：修改//
		ModifyListItem(); 
		break;
	case ID_AA_DELETE:
		DeleteListItem(m_nSelectedItem);
		break;
	case  ID_AA_MOVEUP:
		MoveUpItem();
		break;
	case ID_AA_MOVEDOWN:
		MoveDownItem();
		break;
	default:
		break;
	}
	MyOnPaint();
}


int CInitDlg::ModifyListItem()
{
	switch (m_InitTag)
	{
	case OWER_EDIT_TAG:
		m_EditOwerPopDlg.m_nSelectedItem = m_nSelectedItem;
		m_EditOwerPopDlg.DoModal();
		break;
	case PRODUCTION_LINE_EDIT_TAG:
		m_EditLinePopDlg.m_nSelectedItem = m_nSelectedItem;
		m_EditLinePopDlg.DoModal();
		break;
	case MODULE_EDIT_TAG:
		m_EditModulePopDlg.m_nSelectedItem = m_nSelectedItem;
		m_EditModulePopDlg.DoModal();
		break;
	case DEVICE_EDIT_TAG:
		m_EditDevicePopDlg.m_nSelectedItem = m_nSelectedItem;
		m_EditDevicePopDlg.DoModal();
		break;
	case PLC_EDIT_TAG:
		m_EditPlcPopDlg.m_nSelectedItem = m_nSelectedItem;
		m_EditPlcPopDlg.DoModal();
		break;
	case VIDEO_EDIT_TAG:
		m_EditVideoPopDlg.m_nSelectedItem = m_nSelectedItem;
		m_EditVideoPopDlg.DoModal();
		break;
	default:
		break;
	}

	return 0;
}

int CInitDlg::MoveUpItem()
{
	switch (m_InitTag)
	{
	case OWER_EDIT_TAG:

		break;
	case PRODUCTION_LINE_EDIT_TAG:
		if (m_nSelectedItem >= 1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectProductionLine[m_nSelectedItem - 1], m_pDataProvider->m_vectProductionLine[m_nSelectedItem]);
			//把生产线的SortIndex交换//
			std::swap(m_pDataProvider->m_vectProductionLine[m_nSelectedItem - 1].m_SortIndex, m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProductionLine, m_pDataProvider->m_vectProductionLine[m_nSelectedItem - 1].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProductionLine, m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_Id);
		}
		break;
	case MODULE_EDIT_TAG:
		if (m_nSelectedItem >= 1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectProcessModule[m_nSelectedItem - 1], m_pDataProvider->m_vectProcessModule[m_nSelectedItem]);
			//把SortIndex交换//
			std::swap(m_pDataProvider->m_vectProcessModule[m_nSelectedItem - 1].m_SortIndex, m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProcessModule, m_pDataProvider->m_vectProcessModule[m_nSelectedItem - 1].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProcessModule, m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_Id);
		}
		break;
	case DEVICE_EDIT_TAG:
		if (m_nSelectedItem >= 1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectDevice[m_nSelectedItem - 1], m_pDataProvider->m_vectDevice[m_nSelectedItem]);
			//把SortIndex交换//
			std::swap(m_pDataProvider->m_vectDevice[m_nSelectedItem - 1].m_SortIndex, m_pDataProvider->m_vectDevice[m_nSelectedItem].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbDevice, m_pDataProvider->m_vectDevice[m_nSelectedItem - 1].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbDevice, m_pDataProvider->m_vectDevice[m_nSelectedItem].m_Id);
		}
		break;
	case PLC_EDIT_TAG:
		if (m_nSelectedItem >= 1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectPlc[m_nSelectedItem - 1], m_pDataProvider->m_vectPlc[m_nSelectedItem]);
			//把SortIndex交换//
			std::swap(m_pDataProvider->m_vectPlc[m_nSelectedItem - 1].m_SortIndex, m_pDataProvider->m_vectPlc[m_nSelectedItem].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbPLc, m_pDataProvider->m_vectPlc[m_nSelectedItem - 1].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbPLc, m_pDataProvider->m_vectPlc[m_nSelectedItem].m_Id);
		}
		break;
	case VIDEO_EDIT_TAG:
		if (m_nSelectedItem >= 1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectVideo[m_nSelectedItem - 1], m_pDataProvider->m_vectVideo[m_nSelectedItem]);
			//SortIndex交换//
			std::swap(m_pDataProvider->m_vectVideo[m_nSelectedItem - 1].m_SortIndex, m_pDataProvider->m_vectVideo[m_nSelectedItem].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbVideo, m_pDataProvider->m_vectVideo[m_nSelectedItem - 1].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbVideo, m_pDataProvider->m_vectVideo[m_nSelectedItem].m_Id);
		}
		break;
	default:
		break;
	}

	return 0;
}


int CInitDlg::MoveDownItem()
{
	switch (m_InitTag)
	{
	case OWER_EDIT_TAG:
		break;
	case PRODUCTION_LINE_EDIT_TAG:
		if (m_nSelectedItem<m_pDataProvider->m_vectProductionLine.size()-1)
		{
			//与容器中的下一个元素进行交换//
			std::swap(m_pDataProvider->m_vectProductionLine[m_nSelectedItem], m_pDataProvider->m_vectProductionLine[m_nSelectedItem+1]);
			//把生产线的SortIndex交换//
			std::swap(m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_SortIndex, m_pDataProvider->m_vectProductionLine[m_nSelectedItem+1].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProductionLine, m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProductionLine, m_pDataProvider->m_vectProductionLine[m_nSelectedItem+1].m_Id);

		}

	case MODULE_EDIT_TAG:
		if (m_nSelectedItem<m_pDataProvider->m_vectProcessModule.size()-1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectProcessModule[m_nSelectedItem], m_pDataProvider->m_vectProcessModule[m_nSelectedItem+1]);
			//把SortIndex交换//
			std::swap(m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_SortIndex, m_pDataProvider->m_vectProcessModule[m_nSelectedItem+1].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProcessModule, m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbProcessModule, m_pDataProvider->m_vectProcessModule[m_nSelectedItem+1].m_Id);
		}
		break;
	case DEVICE_EDIT_TAG:
		if (m_nSelectedItem <m_pDataProvider->m_vectDevice.size()-1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectDevice[m_nSelectedItem], m_pDataProvider->m_vectDevice[m_nSelectedItem+1]);
			//把SortIndex交换//
			std::swap(m_pDataProvider->m_vectDevice[m_nSelectedItem].m_SortIndex, m_pDataProvider->m_vectDevice[m_nSelectedItem+1].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbDevice, m_pDataProvider->m_vectDevice[m_nSelectedItem].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbDevice, m_pDataProvider->m_vectDevice[m_nSelectedItem+1].m_Id);
		}
		break;
	case PLC_EDIT_TAG:
		if (m_nSelectedItem <m_pDataProvider->m_vectPlc.size()-1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectPlc[m_nSelectedItem], m_pDataProvider->m_vectPlc[m_nSelectedItem+1]);
			//把SortIndex交换//
			std::swap(m_pDataProvider->m_vectPlc[m_nSelectedItem].m_SortIndex, m_pDataProvider->m_vectPlc[m_nSelectedItem+1].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbPLc, m_pDataProvider->m_vectPlc[m_nSelectedItem].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbPLc, m_pDataProvider->m_vectPlc[m_nSelectedItem+1].m_Id);
		}
		break;
	case VIDEO_EDIT_TAG:
		if (m_nSelectedItem <m_pDataProvider->m_vectVideo.size()-1)
		{
			//与容器中的上一个元素进行交换//
			std::swap(m_pDataProvider->m_vectVideo[m_nSelectedItem], m_pDataProvider->m_vectVideo[m_nSelectedItem+1]);
			//SortIndex交换//
			std::swap(m_pDataProvider->m_vectVideo[m_nSelectedItem].m_SortIndex, m_pDataProvider->m_vectVideo[m_nSelectedItem+1].m_SortIndex);

			//把改动存入数据库//
			m_pDataProvider->UpdateTableItem(CDataProvider::tbVideo, m_pDataProvider->m_vectVideo[m_nSelectedItem].m_Id);
			m_pDataProvider->UpdateTableItem(CDataProvider::tbVideo, m_pDataProvider->m_vectVideo[m_nSelectedItem+1].m_Id);
		}
		break;
	default:
		break;
	}


	return 0;
}



int CInitDlg::DeleteListItem(int nItem)
{
	int nResult;
	std::vector<CProductionLineClass>::iterator pProlineIter;
	std::vector<CProcessModuleClass>::iterator  pModuleIter;
	std::vector<CDeviceClass>::iterator        pDeviceIter;
	std::vector<CPlcClass>::iterator            pPlcIter;
	std::vector<CVideoClass>::iterator		pVideoIter;
	CString tempProLineName, tempModuleName;
	switch (m_InitTag)
	{
	case OWER_EDIT_TAG:
		nResult = MessageBox(_T("该操作将删除用户及其名下所有设备，是否继续当前操作？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
		if (nResult == IDYES)
		{
			m_pDataProvider->DeleteDbTable(CDataProvider::tbUser);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbProductionLine);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbProcessModule);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbDevice);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbPLc);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbVideo);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbProcessPara);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbStatePara);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbDevicePara);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbFaultPara);
			m_pDataProvider->DeleteDbTable(CDataProvider::tbFormula);

		}
		break;
	case PRODUCTION_LINE_EDIT_TAG:
		nResult = MessageBox(_T("该操作将删除该生产线所有工艺模块，设备，摄像头等设备，是否继续当前操作？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
		if (nResult == IDYES)
		{
			//删除数据库中的生产线//
			m_pDataProvider->DeleteDbTableItem(CDataProvider::tbProductionLine,
				m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_Id);

			//删除包含该生产线的工艺模块，设备，PLC,摄像头//
			tempProLineName = m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_strLineName;
			m_pDataProvider->DeleteModule(tempProLineName);
			m_pDataProvider->DeleteDevice(tempProLineName);
			m_pDataProvider->DeleteVideo(tempProLineName);

			//删除相关的设备参数，工艺参数，故障参数//
			m_pDataProvider->DeleteParaRelatedToLine(m_pDataProvider->m_vectProductionLine[m_nSelectedItem].m_Id); 
			

			//删除该容器中的数据
			pProlineIter = m_pDataProvider->m_vectProductionLine.begin();
			m_pDataProvider->m_vectProductionLine.erase(pProlineIter + m_nSelectedItem);
		}

		break;

	case MODULE_EDIT_TAG:
		nResult = MessageBox(_T("该操作将删除该工艺模块下所有相关设备等数据，是否继续当前操作？"), _T("警告"), MB_ICONEXCLAMATION | MB_YESNO);//警告//
		if (nResult == IDYES)
		{
			//删除数据库中的数据//
			m_pDataProvider->DeleteDbTableItem(CDataProvider::tbProcessModule,
				m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_Id);

			//删除包含该生产线的工艺模块，设备，PLC,摄像头//
			tempProLineName = m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_strProductionLineName;
			tempModuleName = m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_strProcessModuleName;
			m_pDataProvider->DeleteDevice(tempProLineName, tempModuleName);
			m_pDataProvider->DeleteVideo(tempProLineName, tempModuleName);

			//删除与模块相关的工艺参数，设备参数，故障参数，状态参数//
			m_pDataProvider->DeleteParaRelatedToModule(m_pDataProvider->m_vectProcessModule[m_nSelectedItem].m_Id);

			//删除该容器中的数据//
			pModuleIter = m_pDataProvider->m_vectProcessModule.begin();
			m_pDataProvider->m_vectProcessModule.erase(pModuleIter + m_nSelectedItem);
		}

		break;
	case DEVICE_EDIT_TAG:
		m_pDataProvider->DeleteDbTableItem(CDataProvider::tbDevice,
			m_pDataProvider->m_vectDevice[m_nSelectedItem].m_Id);

		m_pDataProvider->DeleteParaRelatedToDevice(m_pDataProvider->m_vectDevice[m_nSelectedItem].m_Id);

		pDeviceIter = m_pDataProvider->m_vectDevice.begin();
		m_pDataProvider->m_vectDevice.erase(pDeviceIter + m_nSelectedItem);


		break;
	case PLC_EDIT_TAG:
		m_pDataProvider->DeleteDbTableItem(CDataProvider::tbPLc,
			m_pDataProvider->m_vectPlc[m_nSelectedItem].m_Id);

		m_pDataProvider->DeleteParaRelatedToPlc(m_pDataProvider->m_vectPlc[m_nSelectedItem].m_Id);

		pPlcIter = m_pDataProvider->m_vectPlc.begin();
		m_pDataProvider->m_vectPlc.erase(pPlcIter + m_nSelectedItem);

		break;
	case VIDEO_EDIT_TAG:

		m_pDataProvider->DeleteDbTableItem(CDataProvider::tbVideo,
			m_pDataProvider->m_vectVideo[m_nSelectedItem].m_Id);

		pVideoIter = m_pDataProvider->m_vectVideo.begin();
		m_pDataProvider->m_vectVideo.erase(pVideoIter + m_nSelectedItem);

		break;
	default:
		break;
	}
	return 0;
}

void CInitDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
