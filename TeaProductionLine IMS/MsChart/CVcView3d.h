// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcView3d ��װ��

class CVcView3d : public COleDispatchDriver
{
public:
	CVcView3d(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcView3d(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcView3d(const CVcView3d& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcView3d ����
public:
	float get_Rotation()
	{
		float result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Rotation(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_Elevation()
	{
		float result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_Elevation(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Set(float Rotation, float Elevation)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Rotation, Elevation);
	}

	// IVcView3d ����
public:

};
