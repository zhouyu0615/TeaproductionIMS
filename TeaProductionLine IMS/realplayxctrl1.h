#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装器类

// 注意:  不要修改此文件的内容。  如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CRealplayxctrl1 包装器类

class CRealplayxctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CRealplayxctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x30209FBC, 0x57EB, 0x4F87, { 0xBF, 0x3E, 0x74, 0xE, 0x3D, 0x80, 0x19, 0xD2 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:


// 操作
public:

// _DRealPlayX

// Functions
//

	signed char SetDeviceInfo(LPCTSTR strDvrIP, long nPort, short nChannel, LPCTSTR strUserId, LPCTSTR strUserPwd)
	{
		signed char result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I1, (void*)&result, parms, strDvrIP, nPort, nChannel, strUserId, strUserPwd);
		return result;
	}
	signed char StartPlay()
	{
		signed char result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I1, (void*)&result, NULL);
		return result;
	}
	signed char StopPlay()
	{
		signed char result;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I1, (void*)&result, NULL);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//



};
