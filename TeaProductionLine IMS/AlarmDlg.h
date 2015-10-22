#pragma once
#include "afxcmn.h"
#include "DataProvider.h"
#include "PlcWriteRequest.h"

// CAlarmDlg 对话框

class CAlarmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmDlg)

public:
	CAlarmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlarmDlg();

// 对话框数据
	enum { IDD = IDD_ALARMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtClearalarm();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CListCtrl m_listAlarm;
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();      //数据源//
};
