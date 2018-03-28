// CJCfsClient.h : CCJCfsClient ������

#pragma once
#include "resource.h"       // ������



#include "JCfsClient_i.h"
#include "..\JCSrv\JCSrv_i.h"
#include <json/json.h>
#include "IfSearch.h"

#include <map>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;



#define BEGIN_CALL_JSON_FUNC()\
	virtual HRESULT OnCallJsonFunction(LPCWSTR lpszFuncName, Json::Value Param, Json::Value& Result){

#define CALL_JSON_FUNC(funcName, func)\
	if ( _tcsicmp(lpszFuncName, funcName) == 0 )\
	return func(Param, Result);\

#define END_CALL_JSON_FUNC() return E_NOTIMPL;};


// CCJCfsClient

class ATL_NO_VTABLE CCJCfsClient :
	public IDiskSearchCli,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCJCfsClient, &CLSID_CJCfsClient>,
	public IDispatchImpl<ICJCfsCallBack, &IID_ICJCfsCallBack, &LIBID_JCfsClientLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
	
{
public:
	CCJCfsClient()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CJCFSCLIENT)


BEGIN_COM_MAP(CCJCfsClient)
	COM_INTERFACE_ENTRY(IDiskSearchCli)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ICJCfsCallBack)
END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	

	HRESULT FinalConstruct()
	{
		
		return S_OK;
	}

	void FinalRelease()
	{
		UnInit();
		return ;
	}


protected:
	STDMETHOD(Init)(IDiskSearchNotify* pClientNotify);
	STDMETHOD(UnInit)();
	STDMETHOD(Query)(QueryCondition& queryCondition);
	STDMETHOD(GetResultAt)(DWORD dwIndex, DWORD GetValueMask, SearchResult& ResultInfo);
	STDMETHOD_(DWORD, GetState)();
	STDMETHOD(SetFileMark)(LPCWSTR lpszFile, DWORD dwLeave);

public:
private:
	CComQIPtr<ICJfsSrv> m_CJfsSrv;
	UTIL::com_ptr<IDiskSearchNotify> m_pNotify;
public:
	STDMETHOD(OnDiskSearch_FileChange)(WCHAR cDosName, BSTR lpFile, DWORD dwAction, DWORD dwAttr);
	STDMETHOD(OnDiskSearch_Progress)(WCHAR cDosName, DWORD dwTotalFile, DWORD dwCur);
	STDMETHOD(OnDiskSearch_FileCountChange)(WCHAR cDosName, DWORD dwFileCount, DWORD dwDirCount);
	STDMETHOD(OnDiskSearch_StateChangeNotify)(WCHAR cDosName, INT nMsg, WPARAM wParam, LPARAM lParam);
	STDMETHOD(OnDiskSearch_Result)(DWORD dwCount, DWORD dwTickCount);

private:
	typedef std::map<CString, DWORD > CIConIndex;
	CIConIndex m_IConIndex;
};

OBJECT_ENTRY_AUTO(__uuidof(CJCfsClient), CCJCfsClient)
