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
	cout << "������˳�...";
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
#pragma region ��ʼ������
	//һ����ʼ��COM���
	HRESULT hr;
	CComPtr<IWbemLocator> pLoc = NULL;
	wstring wstrNamespace = L"ROOT\\CIMV2";
	CComPtr<IWbemServices> pSvc = NULL;
	CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
	CComPtr<IWbemClassObject> pclsObj = NULL;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		//cout << "COM initialize success..." << endl;
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
			(LPVOID*)&pLoc);		//���ض���ӿ�ָ��

		if (!SUCCEEDED(hr)) return;
		//cout << "create CLSID_WbemLocator object success..." << endl;
		//���ӵ�WMI��ָ�������ռ�

		hr = pLoc->ConnectServer(
			CComBSTR(wstrNamespace.c_str()),
			NULL, NULL, NULL, NULL, NULL, NULL, &pSvc);
		if (!SUCCEEDED(hr)) return;
		//cout << "ConnectServer success..." << endl;
		//�������ý�������ָ�������Ͻ��е��õ���֤��Ϣ��
		hr = CoSetProxyBlanket(
			pSvc,							//����
			RPC_C_AUTHN_WINNT,				//ָ��Ҫʹ�õ���֤����
			RPC_C_AUTHZ_NONE,
			NULL,							//ָʾ���������֤����ķ�������������
			RPC_C_AUTHN_LEVEL_CALL,			//Ҫʹ�õ���֤����
			RPC_C_IMP_LEVEL_IMPERSONATE,	//Ҫʹ�õ�ģ�⼶��
			NULL,
			EOAC_NONE);						//ָ���˴����ܵı�־
		if (!SUCCEEDED(hr)) return;
		//cout << "CoSetProxyBlanket success..." << endl;
	}
#pragma endregion

	wstring query = L"SELECT * FROM Win32_PerfFormattedData_PerfOS_Processor where Name='_Total'";
	hr = pSvc->ExecQuery(
		CComBSTR("WQL"),	//�����ǡ�WQL��
		CComBSTR(query.c_str()), 				//��ѯ�ı�
		WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
		NULL,				//һ��ָ��IWbemContext�����ָ��
		&pEnumerator);		//���ս����ö��������
	if (!SUCCEEDED(hr)) return;
	//cout << "ExecQuery success..." << endl;

	ULONG uReturn = 0;
	int count = 0;
	//���شӵ�ǰλ����ĵ�һ������pclsObj��
	while (pEnumerator) {
		pclsObj = NULL;
		hr = pEnumerator->Next(
			WBEM_INFINITE,
			1,						//����Ķ�����
			&pclsObj,				//ָ���� IWbemClassObject�ӿ�ָ��
			&uReturn);

		if (SUCCEEDED(hr) && uReturn > 0) {
			//cout << "get WbemClassObject success..." << endl;
			//��IWbemClassObject���ҳ�ClassMember��ʶ�ĳ�Ա����ֵ,�����浽vtProp������
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
	//���밴��ȷ��˳���ͷ�
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
	//�ͷ�COM�����Դ
	CoUninitialize();
}
