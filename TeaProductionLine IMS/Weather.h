#pragma once

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class CWeather
{
public:
	CWeather();
	~CWeather();
	CString OpenUrl(CString url);

	void ConvertToUnicode(CString& strUtf8);


	int GetDataFromHtml(CString strHtml);

	void UpdateWeather(CString strCity);

	CString m_strCity;
	CString m_strWeatherInfo;

	CString m_strWeather;
	CString m_strPM25;
	CString m_curTemp;
	CString m_HighTemp;
	CString m_LowTemp;
	CString m_strHum;
	CString m_strDate;
	CString m_strqlty; //ø’∆¯÷ ¡ø


};

