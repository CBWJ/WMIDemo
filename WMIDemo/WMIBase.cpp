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
	//�ͷ�COM�����Դ
	CoUninitialize();
}

bool CWMIBase::Init()
{
	//һ����ʼ��COM���
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		//���ý��̵İ�ȫ���𣬣�����COM���ʱ�ڳ�ʼ��COM֮��Ҫ����CoInitializeSecurity���ý��̰�ȫ���𣬷���ᱻϵͳʶ��Ϊ������  
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

		//��������һ��WMI�����ռ�����  
		//����һ��CLSID_WbemLocator���� 

		hr = CoCreateInstance(
			CLSID_WbemLocator,		//�����������ʶ��
			0,						//
			CLSCTX_INPROC_SERVER,	//ָ�������������������
			IID_IWbemLocator,		//Ҫ���������ͨ�ŵĽӿڵı�ʶ��
			(LPVOID*)&m_pLoc);		//���ض���ӿ�ָ��

		if (!SUCCEEDED(hr)) return false;
		//���ӵ�WMI��ָ�������ռ�

		hr = m_pLoc->ConnectServer(
			CComBSTR(m_wstrNamespace.c_str()),
			NULL, NULL, NULL, NULL, NULL, NULL, &m_pSvc);
		if (!SUCCEEDED(hr)) return false;
		//�������ý�������ָ�������Ͻ��е��õ���֤��Ϣ��
		hr = CoSetProxyBlanket(
			m_pSvc,							//����
			RPC_C_AUTHN_WINNT,				//ָ��Ҫʹ�õ���֤����
			RPC_C_AUTHZ_NONE,
			NULL,							//ָʾ���������֤����ķ�������������
			RPC_C_AUTHN_LEVEL_CALL,			//Ҫʹ�õ���֤����
			RPC_C_IMP_LEVEL_IMPERSONATE,	//Ҫʹ�õ�ģ�⼶��
			NULL,
			EOAC_NONE);						//ָ���˴����ܵı�־
		if (!SUCCEEDED(hr)) return false;
	}
	return true;
}

bool CWMIBase::ExecQuery(wstring query)
{
	if (!m_pSvc) return false;
	HRESULT hr = m_pSvc->ExecQuery(
		CComBSTR("WQL"),	//�����ǡ�WQL��
		CComBSTR(query.c_str()), 				//��ѯ�ı�
		WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
		NULL,				//һ��ָ��IWbemContext�����ָ��
		&m_pEnumClsObj);		//���ս����ö��������
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
		1,						//����Ķ�����
		&pclsObj,				//ָ���� IWbemClassObject�ӿ�ָ��
		&uReturn);				//��ȡ������
	if (!SUCCEEDED(hr)) return false;
	VARIANT vtProp;
	hr = pclsObj->Get(propName.c_str(), 0, &vtProp, 0, 0);
	if (!SUCCEEDED(hr)) return false;
	//����ֵ
	TCHAR szValue[1024] = { 0x00 };
	switch (vtProp.vt)
	{
	case VT_BSTR:	//�ַ���
		//result = vtProp.bstrVal;
		break;
	case VT_I4:	//32λ����
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
