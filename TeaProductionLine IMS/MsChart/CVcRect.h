// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcRect 包装类

class CVcRect : public COleDispatchDriver
{
public:
	CVcRect(){} // 调用 COleDispatchDriver 默认构造函数
	CVcRect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcRect(const CVcRect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVcRect 方法
public:
	LPDISPATCH get_Min()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Max()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IVcRect 属性
public:

};
