// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcRect ��װ��

class CVcRect : public COleDispatchDriver
{
public:
	CVcRect(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcRect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcRect(const CVcRect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcRect ����
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

	// IVcRect ����
public:

};
