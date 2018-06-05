#pragma once
//�����������򱨴�
#pragma comment(lib, "wbemuuid.lib")

#include<iostream>
#include<string>
#include<Windows.h>
//COM���һ�����������ͷ�ļ�
#include<atlbase.h>
#include<atlcom.h>
#include<atlctl.h>
//WMI����
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

