// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcAxisGrid ��װ��

class CVcAxisGrid : public COleDispatchDriver
{
public:
	CVcAxisGrid(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcAxisGrid(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcAxisGrid(const CVcAxisGrid& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcAxisGrid ����
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

	// IVcAxisGrid ����
public:

};
