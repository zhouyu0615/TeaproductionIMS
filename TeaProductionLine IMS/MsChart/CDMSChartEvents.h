// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\Windows\\SysWOW64\\MSCHRT20.OCX" no_namespace
// CDMSChartEvents 包装类

class CDMSChartEvents : public COleDispatchDriver
{
public:
	CDMSChartEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CDMSChartEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDMSChartEvents(const CDMSChartEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// _DMSChartEvents 方法
public:
	void OLEStartDrag(LPDISPATCH * Data, long * AllowedEffects)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PI4 ;
		InvokeHelper(0x60e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Data, AllowedEffects);
	}
	void OLEGiveFeedback(long * Effect, BOOL * DefaultCursors)
	{
		static BYTE parms[] = VTS_PI4 VTS_PBOOL ;
		InvokeHelper(0x60f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Effect, DefaultCursors);
	}
	void OLESetData(LPDISPATCH * Data, short * DataFormat)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PI2 ;
		InvokeHelper(0x610, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Data, DataFormat);
	}
	void OLECompleteDrag(long * Effect)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x611, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Effect);
	}
	void OLEDragOver(LPDISPATCH * Data, long * Effect, short * Button, short * Shift, float * X, float * Y, short * State)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PI4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PR4 VTS_PI2 ;
		InvokeHelper(0x612, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Data, Effect, Button, Shift, X, Y, State);
	}
	void OLEDragDrop(LPDISPATCH * Data, long * Effect, short * Button, short * Shift, float * X, float * Y)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PI4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PR4 ;
		InvokeHelper(0x613, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Data, Effect, Button, Shift, X, Y);
	}
	void ChartSelected(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void TitleSelected(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void FootnoteSelected(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void LegendSelected(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void PlotSelected(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void AxisSelected(short * axisID, short * AxisIndex, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, MouseFlags, Cancel);
	}
	void AxisLabelSelected(short * axisID, short * AxisIndex, short * labelSetIndex, short * LabelIndex, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, labelSetIndex, LabelIndex, MouseFlags, Cancel);
	}
	void AxisTitleSelected(short * axisID, short * AxisIndex, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, MouseFlags, Cancel);
	}
	void PointSelected(short * Series, short * DataPoint, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, DataPoint, MouseFlags, Cancel);
	}
	void PointLabelSelected(short * Series, short * DataPoint, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, DataPoint, MouseFlags, Cancel);
	}
	void SeriesSelected(short * Series, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, MouseFlags, Cancel);
	}
	void ChartActivated(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void TitleActivated(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void FootnoteActivated(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void LegendActivated(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void PlotActivated(short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MouseFlags, Cancel);
	}
	void AxisActivated(short * axisID, short * AxisIndex, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, MouseFlags, Cancel);
	}
	void AxisLabelActivated(short * axisID, short * AxisIndex, short * labelSetIndex, short * LabelIndex, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, labelSetIndex, LabelIndex, MouseFlags, Cancel);
	}
	void AxisTitleActivated(short * axisID, short * AxisIndex, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, MouseFlags, Cancel);
	}
	void PointActivated(short * Series, short * DataPoint, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, DataPoint, MouseFlags, Cancel);
	}
	void PointLabelActivated(short * Series, short * DataPoint, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, DataPoint, MouseFlags, Cancel);
	}
	void SeriesActivated(short * Series, short * MouseFlags, short * Cancel)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, MouseFlags, Cancel);
	}
	void AxisUpdated(short * axisID, short * AxisIndex, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, updateFlags);
	}
	void AxisLabelUpdated(short * axisID, short * AxisIndex, short * labelSetIndex, short * LabelIndex, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, labelSetIndex, LabelIndex, updateFlags);
	}
	void AxisTitleUpdated(short * axisID, short * AxisIndex, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, axisID, AxisIndex, updateFlags);
	}
	void ChartUpdated(short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, updateFlags);
	}
	void FootnoteUpdated(short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, updateFlags);
	}
	void LegendUpdated(short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, updateFlags);
	}
	void PlotUpdated(short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, updateFlags);
	}
	void PointUpdated(short * Series, short * DataPoint, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, DataPoint, updateFlags);
	}
	void PointLabelUpdated(short * Series, short * DataPoint, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, DataPoint, updateFlags);
	}
	void SeriesUpdated(short * Series, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, updateFlags);
	}
	void TitleUpdated(short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, updateFlags);
	}
	void DataUpdated(short * Row, short * Column, short * labelRow, short * labelColumn, short * labelSetIndex, short * updateFlags)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Column, labelRow, labelColumn, labelSetIndex, updateFlags);
	}
	void DonePainting()
	{
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Click()
	{
		InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DblClick()
	{
		InvokeHelper(DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void KeyDown(short * KeyCode, short Shift)
	{
		static BYTE parms[] = VTS_PI2 VTS_I2 ;
		InvokeHelper(DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms, KeyCode, Shift);
	}
	void KeyPress(short * KeyAscii)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms, KeyAscii);
	}
	void KeyUp(short * KeyCode, short Shift)
	{
		static BYTE parms[] = VTS_PI2 VTS_I2 ;
		InvokeHelper(DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms, KeyCode, Shift);
	}
	void MouseDown(short Button, short Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(DISPID_MOUSEDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void MouseMove(short Button, short Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(DISPID_MOUSEMOVE, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void MouseUp(short Button, short Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(DISPID_MOUSEUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}

	// _DMSChartEvents 属性
public:

};
