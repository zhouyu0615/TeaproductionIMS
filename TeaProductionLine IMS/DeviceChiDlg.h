#pragma once
#include "afxcmn.h"
#include "DataProvider.h"
#include "afxwin.h"
#include <vector>
#include "DevicePara.h"
#include "ProcessModulePara.h"
#include "VideoClass.h"
#include "realplayxctrl1.h"
#include "DeviceContrlPopDlg.h"

// CDeviceChiDlg 对话框

class CDeviceChiDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeviceChiDlg)

public:
	CDeviceChiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceChiDlg();

// 对话框数据
	enum { IDD = IDD_DEVICE_CHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT m_OldSize;                                    //窗体大小发生变化前的尺寸//
	bool m_bHaveInit;                 //是否已经初始化//
	bool m_bHaveRemenberOldSize;       //是否已经记住初始尺寸//
	std::vector<CRect> m_vectOldSize;  //存放各个控件的初始尺寸//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);

	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeDevice;                             //树形控件//
	CDataProvider *m_pDataProvider = CDataProvider::getInstance(); //数据源//
	void InitTreeContrl();                              //初始化树形控件//
	afx_msg void OnTvnSelchangedTreeDeviceChidlg(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listModulePara;
	CListCtrl m_listDevicePara;
	UINT m_nCurSelModuleId;                             //当前选中工艺模块的ID//

	CStatic m_textCurPosition;                          //显示当前位置的静态文本框//
	std::vector<CDevicePara> m_vectTempDevicePara;      //暂存当前选中模块下所有的设备参数//
	std::vector<CProcessPara> m_vectTempModulePara;     //暂存当前选中模块下的所有工艺参数//
	std::vector<CVideoClass> m_vectTempVideo;           //暂存当前选中模块下所属的摄像头//
	void SearchFromDataProvider(UINT nCerSelModuleId);  //将当前选中模块底下的摄像头，工艺参数，设备参数分别存储到临时容器//
	CRealplayxctrl1 m_video;
	void WriteToModuleList();                           //填写工艺参数列表框//
	void WriteToDeviceList();                           //填写设备参数列表框//
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMDblclkLi2Devicepara(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nDClkLineItem;                               //用户在设备参数列表框内双击行的行标//
	CImageList m_imageList;  //图标序列//

	int m_OldItem;    //刚编辑的行//  
	int m_OldSubItem; //刚编辑的列//  
	CEdit m_Edit;     //生成单元编辑框对象//
	bool m_bHaveEditCreate;//标志编辑框已经被创建//
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//创建单元格编辑框函数//
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//销毁单元格编辑框对象//

	afx_msg void OnNMDblclkLi1Modulepara(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	afx_msg void OnEnKillfocusEditCreateid();                     //编辑框失去焦点//
	static UINT ThreadFunc(LPVOID lpParam);     //线程函数//

	void UpdateModuleParaValueShow();
	void UpdateDeviceParaValueShow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool IsPlcConnected(int nPlcId);
	afx_msg void OnTvnSelchangingTreeDeviceChidlg(NMHDR *pNMHDR, LRESULT *pResult);
};
