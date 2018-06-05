#pragma once
//�����������򱨴�
#pragma comment(lib, "wbemuuid.lib")

#include<iostream>
#include<Windows.h>
//COM���һ�����������ͷ�ļ�
#include<atlbase.h>
#include<atlcom.h>
#include<atlctl.h>

#include<Wbemcli.h>

using namespace std;

class CWMI
{
public:
	CWMI();
	~CWMI(void);
public:
	HRESULT ExcuteFun();
protected:
	VOID SetNamespace(wstring wstrNamespace);
private:
	HRESULT InitialCom();	//��ʼ��COM�����
	HRESULT SetComSecLevels();	//���ý���COM��ȫ��Ϣ
	HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);	//����������COM������
	HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc, CComPtr<IWbemServices>& pSvc);	//����WMI�����ռ�
	HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);	//����WMI���ӵİ�ȫ�ȼ�
	virtual HRESULT Excute(CComPtr<IWbemServices> pSvc) = 0;	//�麯��
protected:
	wstring m_wstrNamespace;
};