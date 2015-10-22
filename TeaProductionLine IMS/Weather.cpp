#include "stdafx.h"
#include "Weather.h"
#include "afxinet.h"
#include "atlutil.h"





CWeather::CWeather()
{
    m_strCity = _T("长沙");
	
}


CWeather::~CWeather()
{
}




void CWeather::UpdateWeather(CString strCity)
{
	m_strCity = strCity;


	//CString strURL = _T("http://api.heweather.com/x3/weather?city=%E9%95%BF%E6%B2%99&key=e54b86179fff4845966f864bdac9d488");


	//CString strURL;
	//strURL.Format(_T("http://api.heweather.com/x3/weather?city='%s'&key=e54b86179fff4845966f864bdac9d488"),location);


	DWORD len = 1024;
	CString location = m_strCity;
	PTCHAR pchBuffer = NULL;
	pchBuffer = new TCHAR[1024];

	AtlEscapeUrl(location, pchBuffer, &len, 1024, ATL_URL_ENCODE_PERCENT);
	location.Format(_T("%s"), pchBuffer);
	delete[] pchBuffer;

	CString strURL;
	strURL.Format(_T("http://api.heweather.com/x3/weather?city=%s&key=e54b86179fff4845966f864bdac9d488"), location);

	bool bConnect = InternetCheckConnection(strURL,
		FLAG_ICC_FORCE_CONNECTION, 0);
	if (!bConnect)
	{
		//AfxMessageBox(_T("网络连接失败，无法获取天气"));
		m_strWeatherInfo = _T("\r\n  网络连接失败，无法获取天气");
		return;
	}
	
	CString strHtml;
	strHtml=OpenUrl(strURL);
	GetDataFromHtml(strHtml);
	
}

CString CWeather::OpenUrl(CString strURL)
{
	CInternetSession session(NULL, 0);
	CHttpFile *file = NULL;

	CString  strHtml = _T("网络连接失败"); //存放网页数据	//
	try
	{
		file = (CHttpFile*)session.OpenURL(strURL);
	}
	catch (CInternetException *m_pException)
	{
		file = NULL;
		m_pException->m_dwError;
		m_pException->Delete();
		session.Close();
		AfxMessageBox(_T("网络连接错误"));
		return strHtml;
	}
	CString strLine;
	char sRecived[1024];
	CString tempstr;
	if (file != NULL) {
		while (file->ReadString(tempstr)!=NULL)
		{
			strHtml += tempstr;
		}
	}
	else
	{
		AfxMessageBox(_T("fail"));
	}

	session.Close();
	file->Close();
	delete file; 
	file = NULL;
	ConvertToUnicode(strHtml);
	
	return strHtml;
}




int CWeather::GetDataFromHtml(CString strHtml)
{

	int leftPos = strHtml.Find(_T("pm25"));
	
	m_strPM25 = strHtml.Mid(leftPos + 7, 2);
	
	//leftPos = strHtml.Find(_T("\"basic\":{\"city\":"));
	//m_strCity = strHtml.Mid(leftPos + 17, 2);

	leftPos = strHtml.Find(_T("\"qlty\":"));
	m_strqlty = strHtml.Mid(leftPos + 8, 4);
	m_strqlty.Replace(_T("\""), _T(""));
	m_strqlty.Replace(_T("\,"), _T(""));

	leftPos = strHtml.Find(_T("\"hum\":"));
	m_strHum = strHtml.Mid(leftPos + 7, 2);

	leftPos = strHtml.Find(_T("\"tmp\":{\"max\":"));
	m_HighTemp = strHtml.Mid(leftPos + 14, 2);
	m_LowTemp = strHtml.Mid(leftPos + 25, 2);

	leftPos = strHtml.Find(_T("\"txt_d\":"));
	m_strWeather = strHtml.Mid(leftPos + 9, 7);

	m_strWeather.Replace(_T("\""), _T(""));
	m_strWeather.Replace(_T("\,"), _T(""));
	m_strWeather.Replace(_T("t"), _T(""));
	m_strWeather.Replace(_T("x"), _T(""));

    int startPos = strHtml.Find(_T("\"hourly_forecast\""));
	leftPos = strHtml.Find(_T("\"tmp\":"),startPos);
	m_curTemp = strHtml.Mid(leftPos + 7, 2);

	leftPos = strHtml.Find(_T("\"loc\":"));
	m_strDate = strHtml.Mid(leftPos+7,17);
	m_strDate.Replace(_T("\""), _T(""));


	m_strWeatherInfo.Format(_T("\r\n  天       气：%s\r\n\r\n  当 前 温 度：%s℃\r\n\r\n  今 日 温 度：%s~%s℃ \r\n\r\n  空 气 质 量：%s \r\n\r\n  空 气 湿 度：%s%% \r\n\r\n  发 布 时 间：%s"), m_strWeather, m_curTemp, m_LowTemp, m_HighTemp,m_strqlty, m_strHum, m_strDate);

	return 0;
}


//UTF8转码到unicode
void CWeather::ConvertToUnicode(CString& strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCTSTR)strUtf8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len];
	memset(wszGBK, 0, len * 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len];
	memset(szGBK, 0, len);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}





