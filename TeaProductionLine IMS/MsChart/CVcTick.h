// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcTick ��װ��

class CVcTick : public COleDispatchDriver
{
public:
	CVcTick(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcTick(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcTick(const CVcTick& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcTick ����
public:
	float get_Length()
	{
		float result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Length(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Style()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Style(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IVcTick ����
public:

};
