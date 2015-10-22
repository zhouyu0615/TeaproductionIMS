#pragma once
#include "realplayxctrl1.h"
#include "DataProvider.h"

// CFullScreenDlg 对话框

class CFullScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenDlg)

public:
	CFullScreenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFullScreenDlg();

// 对话框数据
	enum { IDD = IDD_FULLSCREENDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CRealplayxctrl1 m_video;
	int m_nItem; //全屏显示摄像头在数据源容器的位置//
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void DblClickRealplayxctrl20();
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();    //数据源//
};
