// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcDataPoint 包装类

class CVcDataPoint : public COleDispatchDriver
{
public:
	CVcDataPoint(){} // 调用 COleDispatchDriver 默认构造函数
	CVcDataPoint(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcDataPoint(const CVcDataPoint& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVcDataPoint 方法
public:
	LPDISPATCH get_DataPointLabel()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Brush()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EdgePen()
	{
		LPDISPATCH result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Marker()
	{
		LPDISPATCH result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	float get_Offset()
	{
		float result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Offset(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void ResetCustom()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Select()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// IVcDataPoint 属性
public:

};
