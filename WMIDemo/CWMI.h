#pragma once
//必须加这个否则报错
#pragma comment(lib, "wbemuuid.lib")

#include<iostream>
#include<Windows.h>
//COM编程一般加下面三个头文件
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
	HRESULT InitialCom();	//初始化COM组件库
	HRESULT SetComSecLevels();	//设置进程COM安全信息
	HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);	//创建进程内COM服务器
	HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc, CComPtr<IWbemServices>& pSvc);	//连接WMI命名空间
	HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);	//设置WMI连接的安全等级
	virtual HRESULT Excute(CComPtr<IWbemServices> pSvc) = 0;	//虚函数
protected:
	wstring m_wstrNamespace;
};