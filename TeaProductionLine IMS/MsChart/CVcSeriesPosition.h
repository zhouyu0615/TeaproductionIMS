// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcSeriesPosition ��װ��

class CVcSeriesPosition : public COleDispatchDriver
{
public:
	CVcSeriesPosition(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcSeriesPosition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcSeriesPosition(const CVcSeriesPosition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcSeriesPosition ����
public:
	BOOL get_Excluded()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Excluded(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Hidden()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Hidden(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Order()
	{
		short result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Order(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_StackOrder()
	{
		short result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_StackOrder(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IVcSeriesPosition ����
public:

};
