#pragma once
//必须加这个否则报错
#pragma comment(lib, "wbemuuid.lib")

#include<iostream>
#include<string>
#include<Windows.h>
//COM编程一般加下面三个头文件
#include<atlbase.h>
#include<atlcom.h>
#include<atlctl.h>
//WMI必须
#include<Wbemcli.h>

using namespace std;

class CWMIBase
{
public:
	CWMIBase();
	CWMIBase(wstring wstrNamespace);
	~CWMIBase();
private:
	wstring m_wstrNamespace;
	CComPtr<IWbemLocator> m_pLoc = NULL;
	CComPtr<IWbemServices> m_pSvc = NULL;
	CComPtr<IEnumWbemClassObject> m_pEnumClsObj = NULL;
public:
	bool Init();
	bool ExecQuery(wstring query);
	bool GetProperty(wstring propName, wstring& result);
};

