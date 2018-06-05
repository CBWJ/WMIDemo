#include "CSynQuery.h"

CSynQuery::CSynQuery(const wstring & wszNamespace, const wstring & wszWQLQuery)
{
	m_wszWQLQuery = wszWQLQuery;
	m_wstrNamespace = wszNamespace;
}

CSynQuery::~CSynQuery(void)
{
}

HRESULT CSynQuery::Excute(CComPtr<IWbemServices> pSvc)
{
	HRESULT hr = WBEM_S_FALSE;

	do {
		CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
		hr = pSvc->ExecQuery(
			CComBSTR("WQL"),	//目前只支持WQL语言
			CComBSTR(m_wszWQLQuery.c_str()),	//用于查询的命令
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,	//控制着该函数到底是“同步”还是“半同步”
			NULL,	//  pCtx一般设置为NUL
			&pEnumerator);	//是个返回结果的枚举器

		//CHECKWMIHR(hr);

		ULONG uReturn = 0;

		while (pEnumerator) {
			CComPtr<IWbemClassObject> pclsObj = NULL;
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (0 == uReturn) {
				break;
			}
			DealWithIWbemClassObject(pclsObj);
		}

	} while (0);

	return hr;
}

HRESULT CSynQuery::DealWithIWbemClassObject(CComPtr<IWbemClassObject> pclsObj)
{
	HRESULT hr = WBEM_S_NO_ERROR;
	do {
		CComVariant vtClass;

		hr = pclsObj->Get(L"__CLASS", 0, &vtClass, NULL, NULL);
		//CHECKWMIHR(hr);
		if (VT_BSTR == vtClass.vt) {
			wprintf(L"\n%s\n", vtClass.bstrVal);
		}

		hr = pclsObj->BeginEnumeration(WBEM_FLAG_LOCAL_ONLY);

		do {
			CComBSTR bstrName;
			CComVariant Value;
			CIMTYPE type;
			LONG lFlavor = 0;
			hr = pclsObj->Next(0, &bstrName, &Value, &type, &lFlavor);
			//CHECKWMIHR(hr);
			hr = DealWithSingleItem(bstrName, Value, type, lFlavor);
		} while (WBEM_S_NO_ERROR == hr);

		hr = pclsObj->EndEnumeration();
	} while (0);

	return hr;
}

HRESULT CSynQuery::DealWithSingleItem(CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor) {
	return S_OK;
}