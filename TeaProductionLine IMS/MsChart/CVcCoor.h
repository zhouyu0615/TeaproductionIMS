// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcCoor 包装类

class CVcCoor : public COleDispatchDriver
{
public:
	CVcCoor(){} // 调用 COleDispatchDriver 默认构造函数
	CVcCoor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcCoor(const CVcCoor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVcCoor 方法
public:
	float get_X()
	{
		float result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_X(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Y()
	{
		float result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Y(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Set(float X, float Y)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y);
	}

	// IVcCoor 属性
public:

};
