// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcBackdrop 包装类

class CVcBackdrop : public COleDispatchDriver
{
public:
	CVcBackdrop(){} // 调用 COleDispatchDriver 默认构造函数
	CVcBackdrop(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcBackdrop(const CVcBackdrop& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVcBackdrop 方法
public:
	LPDISPATCH get_Shadow()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Frame()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Fill()
	{
		LPDISPATCH result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IVcBackdrop 属性
public:

};
