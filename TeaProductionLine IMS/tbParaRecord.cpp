// tbParaRecord.h : CtbParaRecord ���ʵ��



// CtbParaRecord ʵ��

// ���������� 2015��10��31�� ������, 11:47

#include "stdafx.h"
#include "tbParaRecord.h"
IMPLEMENT_DYNAMIC(CtbParaRecord, CRecordset)

CtbParaRecord::CtbParaRecord(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Id = 0;
	m_CreatedTime;
	m_ProParaId = 0;
	m_ParaValue = 0.0;
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error ��ȫ���⣺�����ַ������ܰ������롣
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CtbParaRecord::GetDefaultConnect()
{
	return _T("DSN=xfcj_SQLSERVER;UID=sa;PWD=123456;APP=Microsoft\x00ae Visual Studio\x00ae 2013;WSID=ZHOUYU-PC");
}

CString CtbParaRecord::GetDefaultSQL()
{
	return _T("[dbo].[tbParaRecord]");
}

void CtbParaRecord::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Id]"), m_Id);
	RFX_Date(pFX, _T("[CreatedTime]"), m_CreatedTime);
	RFX_Long(pFX, _T("[ProParaId]"), m_ProParaId);
	RFX_Single(pFX, _T("[ParaValue]"), m_ParaValue);

}
/////////////////////////////////////////////////////////////////////////////
// CtbParaRecord ���

#ifdef _DEBUG
void CtbParaRecord::AssertValid() const
{
	CRecordset::AssertValid();
}

void CtbParaRecord::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


