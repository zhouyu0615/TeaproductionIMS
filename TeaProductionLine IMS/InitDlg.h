#pragma once
#include "EditPlcParaDlg.h"
#include  "DataProvider.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "EditDevicePopDlg.h"
#include "EditOwnerPopDlg.h"
#include "EditLinePopDlg.h"
#include "EditModulePopDlg.h"
#include "EditPlcPopDlg.h"
#include "EditVedioPopDlg.h"

// CInitDlg 对话框

class CInitDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitDlg)

public:

	CInitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitDlg();

	static const int OWER_EDIT_TAG = 1;
	static const int PRODUCTION_LINE_EDIT_TAG = 2;
	static const int MODULE_EDIT_TAG = 3;
	static const int DEVICE_EDIT_TAG = 4;
	static const int PLC_EDIT_TAG = 5;
	static const int VIDEO_EDIT_TAG = 6;

	int m_InitTag;    //标识当前正在添加的内容，1添加用户；2生产线；3模块；4设备；5PLC;6摄像头//
	int m_nSelectedItem; //右击listctl选中的条目行号//
// 对话框数据
	enum { IDD = IDD_INITDLG };

	CDataProvider *m_pDataProvider = CDataProvider::getInstance();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtPlcparaInitdlg();
	CEditPlcParaDlg m_EditPlcParaDlg;            //PLC参数配置界面//
	afx_msg void OnBnClickedBtOwnerInitdlg();
	afx_msg void OnBnClickedBtLineInitdlg();
	afx_msg void OnBnClickedBtModuleInitdlg();
	afx_msg void OnBnClickedBtDeviceInitdlg();
	afx_msg void OnBnClickedBtPlcInitdlg();
	afx_msg void OnBnClickedBtVedioInitdlg();
	afx_msg void OnBnClickedAddInitdlg();
	afx_msg void OnBnClickedBtGobackInitdlg();
	afx_msg void OnPaint();

	CEditOwnerPopDlg  m_EditOwerPopDlg;
	CEditLinePopDlg   m_EditLinePopDlg;
	CEditModulePopDlg m_EditModulePopDlg;
	CEditDevicePopDlg m_EditDevicePopDlg;
	CEditVedioPopDlg  m_EditVideoPopDlg;
	CEditPlcPopDlg    m_EditPlcPopDlg;



public:
	void ClearEditCtrl();
	void MyOnPaint();
	CEdit m_EditText1;
	CEdit m_EditText2;
	CEdit m_EditText3;
	CEdit m_EditText4;
	CEdit m_EditText5;

	CStatic m_GroupBox;
	CListCtrl m_list_init;
	CComboBox m_LineComboBox;
	CComboBox m_ModuleComboBox;
	CStatic m_StaticText1;
	CStatic m_StaticText2;
	CStatic m_StaticText3;
	CStatic m_StaticText4;
	CStatic m_StaticText5;

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCoLineInitdlg();
	afx_msg void OnNMRClickLiInitdlg(NMHDR *pNMHDR, LRESULT *pResult);

	int LineComboboxOnPaint();
	int ListOnPaint();

	int DeleteListItem(int nItem);
	int ModifyListItem();
	int MoveUpItem();
	int MoveDownItem();
	virtual void OnOK();
};
