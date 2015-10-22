#pragma once
#include "afxcmn.h"
#include "OldConfigGraChiDlg.h"
#include "NewConfigGraChiDlg.h"
#include <vector>

// CConfigChiDlg 对话框

class CConfigChiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigChiDlg)

public:
	CConfigChiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigChiDlg();

// 对话框数据
	enum { IDD = IDD_CONFIG_CHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabConfig;                                  //标签控件//
	COldConfigGraChiDlg m_OldConfigGraChiDlg;              //修改配方界面//
	CNewConfigGraChiDlg m_NewConfigGraChiDlg;              //新建配方界面//
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT m_OldSize;                                    //窗体大小发生变化前的尺寸//
	bool m_bHaveInit;                                   //是否已经初始化//
	bool m_bHaveRemenberOldSize;                        //是否已经记住初始尺寸//
	std::vector<CRect> m_vectOldSize;                   //存放各个控件的初始尺寸//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);
};
