// NewConfigGraChiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeaProductionLine IMS.h"
#include "NewConfigGraChiDlg.h"
#include "afxdialogex.h"


// CNewConfigGraChiDlg 对话框
bool CNewConfigGraChiDlg::m_bIsAddConfig = false;
IMPLEMENT_DYNAMIC(CNewConfigGraChiDlg, CDialogEx)

CNewConfigGraChiDlg::CNewConfigGraChiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewConfigGraChiDlg::IDD, pParent)
{
	m_nLastSelLineItem = -1;
	m_bHaveEditCreate = false;
	m_bHaveInit = false;
	m_bHaveRemenberOldSize = false;
}

CNewConfigGraChiDlg::~CNewConfigGraChiDlg()
{
}

void CNewConfigGraChiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edNewConfigName);
	DDX_Control(pDX, IDC_COMBO1, m_cboxNewConfigLine);
	DDX_Control(pDX, IDC_EDIT3, m_edNote);
	DDX_Control(pDX, IDC_COMBO2, m_cboxModule);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
}


BEGIN_MESSAGE_MAP(CNewConfigGraChiDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CNewConfigGraChiDlg::OnCbnSelchangeCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO1, &CNewConfigGraChiDlg::OnCbnDropdownCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CNewConfigGraChiDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNewConfigGraChiDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNewConfigGraChiDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CNewConfigGraChiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CNewConfigGraChiDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON9, &CNewConfigGraChiDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON7, &CNewConfigGraChiDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CNewConfigGraChiDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CNewConfigGraChiDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CNewConfigGraChiDlg::OnNMClickList2)
	ON_EN_KILLFOCUS(IDC_EDIT_CREATE, &CNewConfigGraChiDlg::OnEnKillfocusEditCreate)
	ON_BN_CLICKED(IDC_BUTTON1, &CNewConfigGraChiDlg::OnBnClickedButton1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CNewConfigGraChiDlg::OnEnKillfocusEdit1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CNewConfigGraChiDlg 消息处理程序


BOOL CNewConfigGraChiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bHaveInit = true;
	// TODO:  在此添加额外的初始化
	//初始化两个list的表头//
	CRect rectList1;
	m_list1.GetWindowRect(&rectList1);
	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LBS_OWNERDRAWVARIABLE);
	m_list1.InsertColumn(0, _T(""), LVCFMT_CENTER, rectList1.Width() / 24.0, -1);
	m_list1.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList1.Width() / 9.0, -1);
	m_list1.InsertColumn(2, _T("名称"), LVCFMT_CENTER, rectList1.Width() / 9.0 * 4.0, -1);
	m_list1.InsertColumn(3, _T("是否监控"), LVCFMT_CENTER, rectList1.Width() / 9.0 * 1.8, -1);
	m_list1.InsertColumn(4, _T("单位"), LVCFMT_CENTER, rectList1.Width() / 9.0 * 1.8, -1);

	CRect rectList2;
	m_list2.GetWindowRect(&rectList2);
	m_list2.SetExtendedStyle(m_list2.GetExtendedStyle() | LVS_EX_CHECKBOXES | LBS_OWNERDRAWVARIABLE | LVS_EX_GRIDLINES);
	m_list2.InsertColumn(0, _T(""), LVCFMT_CENTER, rectList2.Width() / 24.0, -1);
	m_list2.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectList2.Width() / 9.0, -1);
	m_list2.InsertColumn(2, _T("名称"), LVCFMT_CENTER, rectList2.Width() / 9.0 * 4.0, -1);
	m_list2.InsertColumn(3, _T("设定值"), LVCFMT_CENTER, rectList2.Width() / 9.0 * 1.8, -1);
	m_list2.InsertColumn(4, _T("单位"), LVCFMT_CENTER, rectList2.Width() / 9.0 * 1.8, -1);

	CRect rectTemp;                //记住原始尺寸//
	if (!m_bHaveRemenberOldSize)
	{
		GetClientRect(&rectTemp);
		m_OldSize.x = rectTemp.right - rectTemp.left;
		m_OldSize.y = rectTemp.bottom - rectTemp.top;
		RememberOldSize(m_vectOldSize);
		m_bHaveRemenberOldSize = true;
	}

	//填写生产线下拉框//
	m_cboxNewConfigLine.ResetContent();                            //先清空下拉框//
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size();n++)
		m_cboxNewConfigLine.AddString(m_pDataProvider->m_vectProductionLine[n].m_strLineName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CNewConfigGraChiDlg::searchModuleByLineID(int lineID)       //根据生产线的ID找到旗下所有的模块，并存储到m_vectCombo//
{
	if (!m_vectCombo.empty())
		m_vectCombo.clear();
	for (int n = 0; n < m_pDataProvider->m_vectProcessModule.size();n++)
		if (m_pDataProvider->m_vectProcessModule[n].m_ProcessLineId==lineID)
			m_vectCombo.push_back(m_pDataProvider->m_vectProcessModule[n]);
}

void CNewConfigGraChiDlg::searchModuleParaByModuleID(int moduleID)     //根据模块ID找到旗下所有进入配方的工艺参数，并存储到m_vectList1//
{
	if (!m_vectList1.empty())
	    m_vectList1.clear();
	for (int n = 0; n < m_pDataProvider->m_vectProModulePara.size();n++)
		if (m_pDataProvider->m_vectProModulePara[n].m_ProcessModuleId == moduleID && m_pDataProvider->m_vectProModulePara[n].m_IsConfig != 0)
		{
			m_vectList1.push_back(m_pDataProvider->m_vectProModulePara[n]);
			m_vectList1[m_vectList1.size() - 1].m_fSetValue = -3001;
		}
}

//生产线下拉框的内容发生变化//
void CNewConfigGraChiDlg::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_nLastSelLineItem == m_cboxNewConfigLine.GetCurSel())
		return;
	else
		m_nLastSelLineItem = m_cboxNewConfigLine.GetCurSel();
	//清空容器和模块下拉框//
	m_list2.DeleteAllItems();
	m_list1.DeleteAllItems();
	m_vectList1.clear();
	m_vectList2.clear();
	m_cboxModule.ResetContent();

	//重新填写模块下拉框//
	int nTemp = m_cboxNewConfigLine.GetCurSel();
	searchModuleByLineID(m_pDataProvider->m_vectProductionLine[nTemp].m_Id);
	for (int n = 0; n < m_vectCombo.size();n++)
		m_cboxModule.AddString(m_vectCombo[n].m_strProcessModuleName);
}

//当用户点开生产线下拉框//
void CNewConfigGraChiDlg::OnCbnDropdownCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
}

//工艺模块下拉框选中内容发生变化//
void CNewConfigGraChiDlg::OnCbnSelchangeCombo2()
{
	// TODO:  在此添加控件通知处理程序代码
	int nTemp = m_cboxModule.GetCurSel();                  //得到下拉框当前选中条目的序号//
	searchModuleParaByModuleID(m_vectCombo[nTemp].m_Id);   //将模块底下的工艺参数写入容器暂存m_vectList1//
	std::vector<int> vectItem;                             //暂存m_vectList1内已经被选入配方的参数的下标//
	for (int n = 0; n < m_vectList1.size();n++)            //查找m_vectList1内已经被选入配方的参数的下标//
		for (int m = 0; m < m_vectList2.size();m++)
			if (m_vectList1[n].m_Id==m_vectList2[m].m_Id)
			{
				vectItem.push_back(n);
				break;
			}
	for (int n = vectItem.size()-1; n >= 0; n--)          //删除容器m_vectList1内已经被选入配方的参数//
	{
		m_vectList1.erase(m_vectList1.begin()+vectItem[n]);
	}

	//填写待选参数列表框//
	m_list1.DeleteAllItems();
	CString strItem;
	for (int n = 0; n < m_vectList1.size();n++)
	{
		m_list1.InsertItem(n, _T(""));
		strItem.Format(_T("%d"), n + 1);
		m_list1.SetItemText(n, 1, strItem);
		m_list1.SetItemText(n, 2, m_vectList1[n].m_strParaName);
		if (m_vectList1[n].m_IsVisible==0)
			m_list1.SetItemText(n, 3, _T("否"));
		else
			m_list1.SetItemText(n, 3, _T("是"));
		m_list1.SetItemText(n, 4, m_vectList1[n].m_strUnit);
	}
}

//待选参数的全选按钮响应函数//
void CNewConfigGraChiDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	int nNumOfCount = m_list1.GetItemCount();
	for (int n = 0; n < nNumOfCount;n++)
		m_list1.SetCheck(n);
}

//待选参数的清空选择按钮//
void CNewConfigGraChiDlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int n = 0; n < m_list1.GetItemCount(); n++)
		m_list1.SetCheck(n,0);
}

//加入配方按钮的响应函数//
void CNewConfigGraChiDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	std::vector<int> vectSelItem;           //暂存LIST1内被选中条目的下标//
	int nNumOfList2Item;                    //目前List2内已有条目的数量//	
	CString strItem;                        //列表框2的行号//
	for (int n = 0;n<m_vectList1.size();n++)                          //获得所有被选中条目的下标//
	{
		if (m_list1.GetCheck(n))
		{
			vectSelItem.push_back(n);
			m_vectList2.push_back(m_vectList1[n]);
			nNumOfList2Item = m_list2.GetItemCount();                //list2内当前已有条目的数量//
			m_list2.InsertItem(nNumOfList2Item, _T(""));
			strItem.Format(_T("%d"), nNumOfList2Item + 1);
			m_list2.SetItemText(nNumOfList2Item, 1, strItem);
			m_list2.SetItemText(nNumOfList2Item, 2, m_vectList1[n].m_strParaName);
			m_list2.SetItemText(nNumOfList2Item, 4, m_vectList1[n].m_strUnit);
		}
	}	
	for (int n = vectSelItem.size()-1; n >= 0; n--)                   
	{
		m_list1.DeleteItem(vectSelItem[n]);                           //从待选列表内删除//
		m_vectList1.erase(m_vectList1.begin() + vectSelItem[n]);      //从m_vectList1删除//
	}
	if (!vectSelItem.empty())                    //如果删除了某些条目，则重写列表框内显示的序号//
		for (int n = 0; n < m_list1.GetItemCount(); n++)
		{
			strItem.Format(_T("%d"), n + 1);
			m_list1.SetItemText(n, 1, strItem);
		}
}

//已选参数的全选按钮//
void CNewConfigGraChiDlg::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int n = 0; n < m_list2.GetItemCount(); n++)
		m_list2.SetCheck(n);
}

//已选参数的清空选择按钮//
void CNewConfigGraChiDlg::OnBnClickedButton9()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int n = 0; n < m_list2.GetItemCount(); n++)
		m_list2.SetCheck(n, 0);
}

//已选参数的删除按钮//
void CNewConfigGraChiDlg::OnBnClickedButton7()
{
	// TODO:  在此添加控件通知处理程序代码
	int nCurSelModuleItem = m_cboxModule.GetCurSel();           //模块下拉框当前选中的条目的序号//
	int nCurSelModuleID = -1;
	int nNumOfList1;                                            //list1内共有几行//
	CString strTemp;                                            //暂存LIST1的行号//
	std::vector<int> vectTemp;                                  //暂存被选中删除的参数在m_vectList2里的下标//
	if (nCurSelModuleItem >= 0)                                 //获取当前选中模块的ID//
		nCurSelModuleID = m_vectCombo[nCurSelModuleItem].m_Id;

	for (int n = 0; n < m_list2.GetItemCount();n++)       
	{
		if (m_list2.GetCheck(n))                          //如果是选中状态//
		{
			if (m_vectList2[n].m_ProcessModuleId == nCurSelModuleID)  //如果被选中参数所属模块正好是模块下拉框里所选的模块//
			{
				m_vectList2[n].m_fSetValue=-3001;                     //将参数放回m_vectList1前，先清空它的设定值//
				m_vectList1.push_back(m_vectList2[n]);                //将该参数放回m_vectList1//
				nNumOfList1 = m_list1.GetItemCount();                 //在列表框1内显示该参数//
				m_list1.InsertItem(nNumOfList1, _T(""));
				strTemp.Format(_T("%d"), nNumOfList1 + 1);
				m_list1.SetItemText(nNumOfList1, 1, strTemp);
				m_list1.SetItemText(nNumOfList1, 2, m_vectList2[n].m_strParaName);
				if (m_vectList2[n].m_IsVisible == 0)
					m_list1.SetItemText(nNumOfList1, 3, _T("否"));
				else
					m_list1.SetItemText(nNumOfList1, 3, _T("是"));
				m_list1.SetItemText(nNumOfList1, 4, m_vectList2[n].m_strUnit);
			}
			vectTemp.push_back(n);                           //将被选中参数在m_vectList2里的下标存入容器//
		}
	}

	for (int n = vectTemp.size() - 1; n >= 0;n--)            //将选中的参数从列表参数容器2及列表2删除//
	{
		m_vectList2.erase(m_vectList2.begin() + vectTemp[n]);
		m_list2.DeleteItem(vectTemp[n]);
	}

	if (!vectTemp.empty())                    //如果删除了某些条目，则重写列表框内显示的序号//
		for (int n = 0; n < m_list2.GetItemCount();n++)
        {
			strTemp.Format(_T("%d"), n + 1);
			m_list2.SetItemText(n, 1, strTemp);
        }
}

//清空值按钮//
void CNewConfigGraChiDlg::OnBnClickedButton8()
{
	// TODO:  在此添加控件通知处理程序代码
	for (int n = 0; n < m_vectList2.size();n++)
	{
		m_vectList2[n].m_fSetValue=-3001;          //清空其设定值//
		m_list2.SetItemText(n, 3, _T(""));         //对应列表框位置显示为空//
	}
}

//重新配置按钮的响应函数//
void CNewConfigGraChiDlg::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strName, strNote;
	m_edNewConfigName.GetWindowText(strName);
	m_edNote.GetWindowText(strNote);
	if (strName == _T("") && strNote == _T("") && m_cboxNewConfigLine.GetCurSel() == CB_ERR)
		return;
	int nResult = MessageBox(_T("该操作将丢失所有未保存的数据，是否继续？"), _T("警告"), MB_YESNO | MB_ICONEXCLAMATION);
	if (nResult == IDYES)
	{ 
		m_edNewConfigName.SetWindowText(_T(""));  //删除所有相关内容//
		m_edNote.SetWindowText(_T(""));
		m_cboxModule.ResetContent();
		m_cboxNewConfigLine.ResetContent();
		m_list1.DeleteAllItems();
		m_vectList1.clear();
		m_list2.DeleteAllItems();
		m_vectList2.clear();
		m_vectCombo.clear();
		for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)   //重新填写生产线下拉框//
			m_cboxNewConfigLine.AddString(m_pDataProvider->m_vectProductionLine[n].m_strLineName);
		m_nLastSelLineItem = -1;
	}
}

//单击列表框2//
void CNewConfigGraChiDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pEditCtrl->iItem == -1 || pEditCtrl->iSubItem!=3)                                                //点击到非工作区//
	{
		if (m_bHaveEditCreate == true)                                            //如果之前创建了编辑框就销毁掉//
		{
			distroyEdit(&m_list2, &m_Edit, m_OldItem, m_OldSubItem);              //销毁单元格编辑框对象//
			m_bHaveEditCreate = false;
		}
	}
	else
	{
		if (m_bHaveEditCreate == true)
		{
			if (!(m_OldItem == pEditCtrl->iItem && m_OldSubItem == pEditCtrl->iSubItem))    //如果点中的单元格不是之前创建好的//
			{
				distroyEdit(&m_list1, &m_Edit, m_OldItem, m_OldSubItem);
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
			m_OldItem = pEditCtrl->iItem;                                                 //将点中的单元格的行赋值给“刚编辑过的行”以便后期处理//
			m_OldSubItem = pEditCtrl->iSubItem;                                           //将点中的单元格的行赋值给“刚编辑过的列”以便后期处理//
			createEdit(pEditCtrl, &m_Edit, m_OldItem, m_OldSubItem, m_bHaveEditCreate);         //创建编辑框//
		}
	}

	*pResult = 0;
}

//创建编辑框//
void CNewConfigGraChiDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)
//pEditCtrl为列表对象指针，createdit为编辑框指针对象//
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准//
{
	Item = pEditCtrl->iItem;                     //将点中的单元格的行赋值给“刚编辑过的行”以便后期处理//
	SubItem = pEditCtrl->iSubItem;               //将点中的单元格的行赋值给“刚编辑过的列”以便后期处理//
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_CENTER | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_EDIT_CREATE);  //创建编辑框对象,IDC_EDIT_CREATE为控件ID号//
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);                   //设置字体//
	createdit->SetParent(&m_list2);                      //将listcontrol设置为父窗口//
	CRect  EditRect;
	m_list2.GetSubItemRect(m_OldItem, 3, LVIR_LABEL, EditRect);      //获取单元格的空间位置信息//
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list2.GetColumnWidth(3) - 1, EditRect.bottom - 1);//+1和-1让编辑框不至于挡住列表框中的网格线//
	CString strTemp;
	strTemp = m_list2.GetItemText(m_OldItem, 3);
	createdit->SetWindowText(strTemp);
	createdit->MoveWindow(&EditRect);                                      //将编辑框位置放在相应单元格上//
	createdit->ShowWindow(SW_SHOW);                                        //显示编辑框在单元格上面//
	createdit->SetFocus();                                                 //设置为焦点//
	createdit->SetSel(-1);                                                 //设置光标在文本框文字的最后//
}

//销毁编辑框//
void CNewConfigGraChiDlg::distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem)
{
	CString strEditData;
	float tempfloat;
	int nTempData;
	distroyedit->GetWindowText(strEditData);
	strEditData = strEditData.Trim();
	if (strEditData.IsEmpty())             //如果输入为空，则销毁编辑框//
	{
		distroyedit->DestroyWindow();
		m_vectList2[Item].m_fSetValue = -3001;
		list->SetItemText(Item, 3, _T(""));
		return;
	}
	if (m_vectList2[Item].m_ValueType == 0 && (strEditData != _T("0") && strEditData != _T("1")))
	{
		AfxMessageBox(_T("非法操作，该参数值只能为“0”或“1”！"));
		distroyedit->DestroyWindow();                          //销毁对象//
		return;
	}
	if (strEditData == strEditData.SpanIncluding(_T("0123456789.")))
	{
		switch (m_vectList2[Item].m_ValueType)
		{
		case 0:
			if (strEditData==_T("0"))
			{
				m_vectList2[Item].m_fSetValue = 0;
				list->SetItemText(Item, 3, _T("0"));
			} 
			else
			{
				m_vectList2[Item].m_fSetValue = 1;
				list->SetItemText(Item, 3, _T("1"));
			}
			break;
		case 1:
		case 2:
		case 3:
			nTempData = (float)_wtof(strEditData);
			m_vectList2[Item].m_fSetValue = nTempData;    //将修改后的值存入容器//
			strEditData.Format(_T("%d"), nTempData);
			list->SetItemText(Item, 3, strEditData);      //将修改写入列表框//
			break;
		case 4:
		case 5:
			tempfloat = (float)_wtof(strEditData);
			strEditData.Format(_T("%.2f"), tempfloat);
			list->SetItemText(Item, 3, strEditData);      //将修改写入列表框//
			m_vectList2[Item].m_fSetValue = tempfloat;    //将修改后的值存入容器//
			break;
		default:
			break;
		}
	}
	else
		AfxMessageBox(_T("非法操作，请输入数字！"));
	distroyedit->DestroyWindow();                          //销毁对象//
}

//编辑框失去焦点//
void CNewConfigGraChiDlg::OnEnKillfocusEditCreate()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bHaveEditCreate == true)                                               //如果之前创建了编辑框就销毁掉//
	{
		distroyEdit(&m_list2, &m_Edit, m_OldItem, m_OldSubItem);                 //销毁单元格编辑框对象//
		m_bHaveEditCreate = false;
	}
}

//重载虚函数//
void CNewConfigGraChiDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	m_list2.SetFocus();
}

//生成配方按钮//
void CNewConfigGraChiDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strConfigName;
	m_edNewConfigName.GetWindowText(strConfigName);
	strConfigName = strConfigName.Trim();
	if (strConfigName == _T(""))          //检测配方名是否为空//
	{
		AfxMessageBox(_T("生成新配方失败！请先为配方命名！"));
		return;
	}
	for (int n = 0; n < m_vectList2.size();n++)   //检测进入配方的参数设定值是否完整//
	{
		if (m_list2.GetItemText(n,3)==_T(""))
		{
			AfxMessageBox(_T("生成新配方失败！请为每个配方参数添加预设值！"));
			return;
		}
	}
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size();n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_strFormulaName==strConfigName)
		{
			AfxMessageBox(_T("生成新配方失败！该配方名称已被使用，请重新命名！"));
			return;
		}
	}
	CString strNote;         //存储备注内容//
	m_edNote.GetWindowText(strNote);
	for (int n = 0; n < m_vectList2.size(); n++)    //存入数据库//
	{
		m_TempFormula.m_IsCurrentFormula = 0;
		m_TempFormula.m_DefaultValue = m_vectList2[n].m_fSetValue;
		m_TempFormula.m_IsDefaultFormula = 0;
		m_TempFormula.m_ProcessParaId = m_vectList2[n].m_Id;
		m_TempFormula.m_ProductionLineId = m_vectList2[n].m_ProductionLineId;
		m_TempFormula.m_strFormulaName = strConfigName;
		m_TempFormula.m_strNote = strNote;
		m_TempFormula.m_strParaValueUnit = m_vectList2[n].m_strUnit;
		m_TempFormula.m_strProcessParaName = m_vectList2[n].m_strParaName;
		m_TempFormula.m_strProductionLineName = m_vectList2[n].m_strProductionLineName;
		m_pDataProvider->AddFormulaToDatabase(m_TempFormula);
	}
	AfxMessageBox(_T("生成新配方成功！"));
	m_bIsAddConfig = true;
	//清空容器及显示区//
	m_edNewConfigName.SetWindowText(_T(""));
	m_edNote.SetWindowText(_T(""));
	m_cboxNewConfigLine.ResetContent();
	for (int n = 0; n < m_pDataProvider->m_vectProductionLine.size(); n++)   //重新填写生产线下拉框//
		m_cboxNewConfigLine.AddString(m_pDataProvider->m_vectProductionLine[n].m_strLineName);
	m_nLastSelLineItem = -1;
	m_cboxModule.ResetContent();
	m_vectCombo.clear();
	m_list1.DeleteAllItems();
	m_vectList1.clear();
	m_list2.DeleteAllItems();
	m_vectList2.clear();
}

//检测配方名称是否已被使用//
void CNewConfigGraChiDlg::OnEnKillfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strConfigName;
	m_edNewConfigName.GetWindowText(strConfigName);
	for (int n = 0; n < m_pDataProvider->m_vectFormula.size(); n++)
	{
		if (m_pDataProvider->m_vectFormula[n].m_strFormulaName == strConfigName)
		{
			AfxMessageBox(_T("该配方名称已被使用，请重新命名新配方！"));
			return;
		}
	}
}


void CNewConfigGraChiDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		if (m_bHaveInit&&m_bHaveRemenberOldSize)
		{
			ReSize(m_vectOldSize, m_OldSize);
			CRect rectList1;
			m_list1.GetWindowRect(&rectList1);
			m_list1.SetColumnWidth(0, rectList1.Width() / 24.0);
			m_list1.SetColumnWidth(1, rectList1.Width() / 24.0*2.0);
			m_list1.SetColumnWidth(2, rectList1.Width() / 24.0 * 10.4);
			m_list1.SetColumnWidth(3, rectList1.Width() / 24.0 * 5.22);
			m_list1.SetColumnWidth(4, rectList1.Width() / 24.0 * 5.3);
			CRect rectList2;
			m_list2.GetWindowRect(&rectList2);
			m_list2.SetColumnWidth(0, rectList2.Width() / 24.0);
			m_list2.SetColumnWidth(1, rectList2.Width() / 24.0*2.0);
			m_list2.SetColumnWidth(2, rectList2.Width() / 24.0 * 10.4);
			m_list2.SetColumnWidth(3, rectList2.Width() / 24.0 * 5.22);
			m_list2.SetColumnWidth(4, rectList2.Width() / 24.0 * 5.3);
			Invalidate();
		}
}


void CNewConfigGraChiDlg::RememberOldSize(std::vector<CRect> &vectOldSize)
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

void CNewConfigGraChiDlg::ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient)
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