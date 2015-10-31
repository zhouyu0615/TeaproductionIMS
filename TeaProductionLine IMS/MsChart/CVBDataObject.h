// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVBDataObject 包装类

class CVBDataObject : public COleDispatchDriver
{
public:
	CVBDataObject(){} // 调用 COleDispatchDriver 默认构造函数
	CVBDataObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVBDataObject(const CVBDataObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVBDataObject 方法
public:
	void Clear()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	VARIANT GetData(short sFormat)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, sFormat);
		return result;
	}
	BOOL GetFormat(short sFormat)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sFormat);
		return result;
	}
	void SetData(VARIANT& vValue, VARIANT& vFormat)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &vValue, &vFormat);
	}
	LPDISPATCH get_Files()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IVBDataObject 属性
public:

};
