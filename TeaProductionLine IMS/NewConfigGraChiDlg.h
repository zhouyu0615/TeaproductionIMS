#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DataProvider.h"
#include <vector>
#include "ProcessModulePara.h"
#include "ProcessModuleClass.h"
#include "FormulaClass.h"
// CNewConfigGraChiDlg 对话框

class CNewConfigGraChiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewConfigGraChiDlg)

public:
	CNewConfigGraChiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewConfigGraChiDlg();

// 对话框数据
	enum { IDD = IDD_NEWCONFIG_GRACHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edNewConfigName;         //配方名称编辑框//
	CComboBox m_cboxNewConfigLine;   //适用生产线下拉框//
	CEdit m_edNote;                  //备注编辑框//
	CComboBox m_cboxModule;          //模块下拉框//
	CListCtrl m_list1;               //可选参数列表框//
	CListCtrl m_list2;               //已选参数列表框//
	virtual BOOL OnInitDialog();
	CDataProvider *m_pDataProvider = CDataProvider::getInstance();       //数据源//
	std::vector<CProcessPara>  m_vectList1;           //显示到列表框1的容器//
	std::vector<CProcessPara>  m_vectList2;           //存储新配方参数的容器//
	std::vector<CProcessModuleClass>  m_vectCombo;    //工艺模块下拉框的暂存容器//
	CFormulaClass m_TempFormula;                      //一条配方记录//
	void searchModuleByLineID(int lineID);                      //根据生产线的ID找到旗下所有的模块，并存储到m_vectCombo//
	void searchModuleParaByModuleID(int moduleID);              //根据模块ID找到旗下所有工艺参数，并存储到m_vectList1//
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnDropdownCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	int m_nLastSelLineItem;                          //上一次所选生产线在下拉框的序号//
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();

	//list2编辑框所需变量及函数的声明//
	int m_OldItem;           //刚编辑的行//  
	int m_OldSubItem;        //刚编辑的列//  
	CEdit m_Edit;            //生成单元编辑框对象//
	bool m_bHaveEditCreate;  //标志编辑框已经被创建//
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat); //创建单元格编辑框函数//
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);                       //销毁单元格编辑框对象//
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditCreate();
	virtual void OnOK();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnKillfocusEdit1();
	static bool m_bIsAddConfig;  //配方是否发生改变//
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT m_OldSize;                                    //窗体大小发生变化前的尺寸//
	bool m_bHaveInit;                                   //是否已经初始化//
	bool m_bHaveRemenberOldSize;                        //是否已经记住初始尺寸//
	std::vector<CRect> m_vectOldSize;                   //存放各个控件的初始尺寸//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);
};
