// tbFaultPara.h : CtbFaultPara 的声明

#pragma once

// 代码生成在 2015年7月26日 星期日, 08:57

class CtbFaultPara : public CRecordset
{
public:
	CtbFaultPara(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CtbFaultPara)

// 字段/参数数据

// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
// 数据类型的 CStringW)的实际数据类型。
//  这是为防止 ODBC 驱动程序执行可能
// 不必要的转换。如果希望，可以将这些成员更改为
// CString 类型，ODBC 驱动程序将执行所有必要的转换。
// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
// 以同时支持 Unicode 和这些转换)。

	long	m_Id;
	CTime	m_CreatedDateTime;
	CTime	m_LastUpdatedDateTime;
	long	m_PLCId;
	long	m_ProcessModuleId;
	long	m_DeviceId;
	long	m_ProductionLineId;
	CStringA	m_ProductionLineName;
	CStringA	m_ProcessModuleName;
	CStringA	m_DeviceName;
	CStringA	m_PlcName;
	CStringA	m_Description;
	long	m_ParaValue;
	CStringA	m_ParaName;
	CStringA	m_AddressIndex;

// 重写
	// 向导生成的虚函数重写
	public:
	virtual CString GetDefaultConnect();	// 默认连接字符串

	virtual CString GetDefaultSQL(); 	// 记录集的默认 SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

// 实现
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


