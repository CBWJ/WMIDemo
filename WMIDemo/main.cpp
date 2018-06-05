#include<Windows.h>
#include<tchar.h>
#include<string>
//#include "CSynQueryData.h"
#include "WMIBase.h"

void wmi_step();
void wmi_loop();

int _tmain(int argc, _TCHAR* argv[]) 
{
	while (true) {
		wmi_loop();
	}
	cout << "任意键退出...";
	getchar();
	return 0;
}

void wmi_loop() {
	CWMIBase wmi(L"ROOT\\CIMV2");
	if (wmi.Init()) {
		//cout << "wmibase initialize success" << endl;
		if (wmi.ExecQuery(L"select * from Win32_Processor")) {
			//cout << "wmibase query success" << endl;
			wstring ret;
			if (wmi.GetProperty(L"LoadPercentage", ret)) {
				wcout << ret << endl;
			}
			else {
				cout << "get property failed." << endl;
			}
		}
	}
	else {
		//cout << "wmibase initialize fail" << endl;
	}
}
void wmi_step()
{
#pragma region 初始化工作
	//一、初始化COM组件
	HRESULT hr;
	CComPtr<IWbemLocator> pLoc = NULL;
	wstring wstrNamespace = L"ROOT\\CIMV2";
	CComPtr<IWbemServices> pSvc = NULL;
	CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
	CComPtr<IWbemClassObject> pclsObj = NULL;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		//cout << "COM initialize success..." << endl;
		//设置进程的安全级别，（调用COM组件时在初始化COM之后要调用CoInitializeSecurity设置进程安全级别，否则会被系统识别为病毒）  
		hr = CoInitializeSecurity(
			NULL,		//Defines the access permissions
			-1,			//Count of entries in asAuthSvc.
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_PKT,	// The default authentication level for the process
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE,
			NULL);

		//二、创建一个WMI命名空间连接  
		//创建一个CLSID_WbemLocator对象 

		hr = CoCreateInstance(
			CLSID_WbemLocator,		//创建对象类标识码
			0,						//
			CLSCTX_INPROC_SERVER,	//指定对象代码运行上下文
			IID_IWbemLocator,		//要用来与对象通信的接口的标识符
			(LPVOID*)&pLoc);		//返回对象接口指针

		if (!SUCCEEDED(hr)) return;
		//cout << "create CLSID_WbemLocator object success..." << endl;
		//连接到WMI并指定命名空间

		hr = pLoc->ConnectServer(
			CComBSTR(wstrNamespace.c_str()),
			NULL, NULL, NULL, NULL, NULL, NULL, &pSvc);
		if (!SUCCEEDED(hr)) return;
		//cout << "ConnectServer success..." << endl;
		//三、设置将用于在指定代理上进行调用的认证信息。
		hr = CoSetProxyBlanket(
			pSvc,							//代理
			RPC_C_AUTHN_WINNT,				//指定要使用的认证服务
			RPC_C_AUTHZ_NONE,
			NULL,							//指示用于身份验证服务的服务器主体名称
			RPC_C_AUTHN_LEVEL_CALL,			//要使用的认证级别
			RPC_C_IMP_LEVEL_IMPERSONATE,	//要使用的模拟级别
			NULL,
			EOAC_NONE);						//指定此代理功能的标志
		if (!SUCCEEDED(hr)) return;
		//cout << "CoSetProxyBlanket success..." << endl;
	}
#pragma endregion

	wstring query = L"SELECT * FROM Win32_PerfFormattedData_PerfOS_Processor where Name='_Total'";
	hr = pSvc->ExecQuery(
		CComBSTR("WQL"),	//必须是“WQL”
		CComBSTR(query.c_str()), 				//查询文本
		WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
		NULL,				//一个指向IWbemContext对象的指针
		&pEnumerator);		//接收结果的枚举器对象
	if (!SUCCEEDED(hr)) return;
	//cout << "ExecQuery success..." << endl;

	ULONG uReturn = 0;
	int count = 0;
	//返回从当前位置起的第一个对象到pclsObj中
	while (pEnumerator) {
		pclsObj = NULL;
		hr = pEnumerator->Next(
			WBEM_INFINITE,
			1,						//请求的对象数
			&pclsObj,				//指定的 IWbemClassObject接口指针
			&uReturn);

		if (SUCCEEDED(hr) && uReturn > 0) {
			//cout << "get WbemClassObject success..." << endl;
			//从IWbemClassObject中找出ClassMember标识的成员属性值,并保存到vtProp变量中
			VARIANT vtProp;
			hr = pclsObj->Get(CComBSTR("PercentProcessorTime"), 0, &vtProp, 0, 0);
			if (SUCCEEDED(hr)) {
				//cout << "get property success..." << endl;
				if (vtProp.vt == VT_BSTR) {
					//cout << W2T(vtProp.bstrVal) << endl;
					wprintf(vtProp.bstrVal);
				}
				else if (vtProp.vt == VT_I4) {
					//wprintf(L"%d", vtProp.bstrVal);
					int temp = stoi(vtProp.bstrVal);
					cout << temp;
				}
				cout << endl;
				VariantClear(&vtProp);
				if (pclsObj != NULL) {
					pclsObj.Release();
				}
			}
		}
		else break;
	}
	//必须按正确的顺序释放
	if (pclsObj != NULL) {
		pclsObj.Release();
	}
	if (pEnumerator != NULL) {
		pEnumerator.Release();
	}
	if (pSvc != NULL) {
		pSvc.Release();
	}
	if (pLoc != NULL) {
		pLoc.Release();
	}
	//释放COM组件资源
	CoUninitialize();
}
