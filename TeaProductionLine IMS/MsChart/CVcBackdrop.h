// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcBackdrop ��װ��

class CVcBackdrop : public COleDispatchDriver
{
public:
	CVcBackdrop(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcBackdrop(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcBackdrop(const CVcBackdrop& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcBackdrop ����
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

	// IVcBackdrop ����
public:

};
