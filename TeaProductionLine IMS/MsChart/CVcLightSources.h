// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CVcLightSources ��װ��

class CVcLightSources : public COleDispatchDriver
{
public:
	CVcLightSources(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CVcLightSources(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcLightSources(const CVcLightSources& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IVcLightSources ����
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Add(float X, float Y, float Z, float Intensity)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, X, Y, Z, Intensity);
		return result;
	}
	void Remove(short Index)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	LPDISPATCH get_Item(short Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	LPDISPATCH get__Item(short Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}

	// IVcLightSources ����
public:

};
