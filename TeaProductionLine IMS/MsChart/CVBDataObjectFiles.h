// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVBDataObjectFiles 包装类

class CVBDataObjectFiles : public COleDispatchDriver
{
public:
	CVBDataObjectFiles(){} // 调用 COleDispatchDriver 默认构造函数
	CVBDataObjectFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVBDataObjectFiles(const CVBDataObjectFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVBDataObjectFiles 方法
public:
	CString get_Item(long lIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, lIndex);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Add(LPCTSTR bstrFilename, VARIANT& vIndex)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrFilename, &vIndex);
	}
	void Clear()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Remove(VARIANT& vIndex)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &vIndex);
	}
	LPUNKNOWN _NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// IVBDataObjectFiles 属性
public:

};
