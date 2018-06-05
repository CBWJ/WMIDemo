#include "WMIBase.h"



CWMIBase::CWMIBase()
{
}

CWMIBase::CWMIBase(wstring wstrNamespace)
{
	m_wstrNamespace = wstrNamespace;
}


CWMIBase::~CWMIBase()
{
	if (m_pEnumClsObj != NULL) {
		m_pEnumClsObj.Release();
	}
	if (m_pSvc != NULL) {
		m_pSvc.Release();
	}
	if (m_pLoc != NULL) {
		m_pLoc.Release();
	}
	//释放COM组件资源
	CoUninitialize();
}

bool CWMIBase::Init()
{
	//一、初始化COM组件
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
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
			(LPVOID*)&m_pLoc);		//返回对象接口指针

		if (!SUCCEEDED(hr)) return false;
		//连接到WMI并指定命名空间

		hr = m_pLoc->ConnectServer(
			CComBSTR(m_wstrNamespace.c_str()),
			NULL, NULL, NULL, NULL, NULL, NULL, &m_pSvc);
		if (!SUCCEEDED(hr)) return false;
		//三、设置将用于在指定代理上进行调用的认证信息。
		hr = CoSetProxyBlanket(
			m_pSvc,							//代理
			RPC_C_AUTHN_WINNT,				//指定要使用的认证服务
			RPC_C_AUTHZ_NONE,
			NULL,							//指示用于身份验证服务的服务器主体名称
			RPC_C_AUTHN_LEVEL_CALL,			//要使用的认证级别
			RPC_C_IMP_LEVEL_IMPERSONATE,	//要使用的模拟级别
			NULL,
			EOAC_NONE);						//指定此代理功能的标志
		if (!SUCCEEDED(hr)) return false;
	}
	return true;
}

bool CWMIBase::ExecQuery(wstring query)
{
	if (!m_pSvc) return false;
	HRESULT hr = m_pSvc->ExecQuery(
		CComBSTR("WQL"),	//必须是“WQL”
		CComBSTR(query.c_str()), 				//查询文本
		WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
		NULL,				//一个指向IWbemContext对象的指针
		&m_pEnumClsObj);		//接收结果的枚举器对象
	if (!SUCCEEDED(hr)) return false;
	return true;
}

bool CWMIBase::GetProperty(wstring propName, wstring& result)
{
	if (!m_pEnumClsObj) return false;
	ULONG uReturn = 0;
	CComPtr<IWbemClassObject> pclsObj = NULL;
	HRESULT hr = m_pEnumClsObj->Next(
		WBEM_INFINITE,
		1,						//请求的对象数
		&pclsObj,				//指定的 IWbemClassObject接口指针
		&uReturn);				//获取对象数
	if (!SUCCEEDED(hr)) return false;
	VARIANT vtProp;
	hr = pclsObj->Get(propName.c_str(), 0, &vtProp, 0, 0);
	if (!SUCCEEDED(hr)) return false;
	//设置值
	TCHAR szValue[1024] = { 0x00 };
	switch (vtProp.vt)
	{
	case VT_BSTR:	//字符串
		//result = vtProp.bstrVal;
		break;
	case VT_I4:	//32位整数
		swprintf_s(szValue, _T("%d"), vtProp.intVal);
		result = szValue;
		break;
	default:
		break;
	}
	VariantClear(&vtProp);
	if (pclsObj != NULL) {
		pclsObj.Release();
	}
	return true;
}
