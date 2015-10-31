// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcAxisGrid 包装类

class CVcAxisGrid : public COleDispatchDriver
{
public:
	CVcAxisGrid(){} // 调用 COleDispatchDriver 默认构造函数
	CVcAxisGrid(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcAxisGrid(const CVcAxisGrid& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVcAxisGrid 方法
public:
	LPDISPATCH get_MinorPen()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_MajorPen()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IVcAxisGrid 属性
public:

};
