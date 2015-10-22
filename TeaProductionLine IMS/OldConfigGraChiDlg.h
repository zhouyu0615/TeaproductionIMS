#pragma once
#include "DataProvider.h"
#include "NewConfigGraChiDlg.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include "FormulaClass.h"
#include "AddConfigParaPopDlg.h"

// COldConfigGraChiDlg 对话框

class COldConfigGraChiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COldConfigGraChiDlg)

public:
	COldConfigGraChiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COldConfigGraChiDlg();

// 对话框数据
	enum { IDD = IDD_OLDCONFIG_GRACHIDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listAllConfig;     //控件变量//
	CListCtrl m_listConfigPara;
	CTreeCtrl m_treeConfig;
	CEdit m_edConfigName;
	CEdit m_edConfigLine;
	CEdit m_edNote;

	CDataProvider *m_pDataProvider = CDataProvider::getInstance();    //数据源//
	std::vector<int> m_vectConfigID;
	std::vector<CString> m_vectConfigName;

	void InitTreeContrl();                                  //初始化树形区域//
	void searchConfigFromLine(int nLineID);                 //根据生产线的ID找到旗下的配方,并将配方名称和ID存到SET容器//
	void WriteToList1(int nLineID,CString strLineName,int nItem);    //填写list的第nItem+1行//

	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);

	bool m_bIsNameChange;                 //当前编辑区内的配方是否发生了更改//
	bool m_bIsParaValueChange;            //是否修改了配方参数的值//
	bool m_bIsNoteChange;                 //是否修改了配方的备注//
	bool m_bIsDelePara;                   //是否删除了配方参数//
	bool m_bIsAddPara;                    //是否添加了配方参数//
	CString m_strOldName;                  //修改之前的配方名//
	CString m_strOldNote;                 //修改之前的备注//
	std::vector<CFormulaClass> m_vectConfigPara;    //按顺序存储list2内的配方参数//

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void saveChange();          //保存修改//
	void AddParaToConfig();     //添加参数到配方//
	CAddConfigParaPopDlg m_AddConfigParaPopDlg;   // 添加配方参数的界面//
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton2();

	//list2编辑框所需变量及函数的声明//
	int m_OldItem;           //刚编辑的行//  
	int m_OldSubItem;        //刚编辑的列//  
	CEdit m_Edit;            //生成单元编辑框对象//
	bool m_bHaveEditCreate;  //标志编辑框已经被创建//
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat); //创建单元格编辑框函数//
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);                       //销毁单元格编辑框对象//
	virtual void OnOK();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditCreate1();
	void ReInitDlg();                  //重新初始化窗口//
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton3();
	bool m_bIsCurConfigChange;         //当前使用配方是否发生变化//
	afx_msg void OnBnClickedButton4();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT m_OldSize;                                    //窗体大小发生变化前的尺寸//
	bool m_bHaveInit;                                   //是否已经初始化//
	bool m_bHaveRemenberOldSize;                        //是否已经记住初始尺寸//
	std::vector<CRect> m_vectOldSize;                   //存放各个控件的初始尺寸//
	void ReSize(std::vector<CRect> &vectOldSize, POINT OldSizeOfClient);
	void RememberOldSize(std::vector<CRect> &vectOldSize);
	CImageList m_imageList;  //图标序列//
	bool IsPlcConnected(int nFormulaId);    //判断配方参数所属PLC是否连接//
	bool IsPlcConnectedByPlcId(int nPlcId);  //判断某台PLC是否连接//
	afx_msg void OnBnClickedButton8();
	void reInitList1();//重新填写列表框1//
};
