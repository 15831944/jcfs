// CJfsSrv.h : CCJfsSrv ������

#pragma once
#include "resource.h"       // ������



#include "JCSrv_i.h"
#include "..\JCfsClient\JCfsClient_i.h"
#include <process\CheckProcExit.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;

#include <IfSearch.h>
#include <json/json.h>
// CCJfsSrv

#define BEGIN_CALL_JSON_FUNC()\
		virtual HRESULT OnCallJsonFunction(LPCWSTR lpszFuncName, Json::Value Param, Json::Value& Result){

#define CALL_JSON_FUNC(funcName, func)\
	if ( _tcsicmp(lpszFuncName, funcName) == 0 )\
		return func(Param, Result);\

#define END_CALL_JSON_FUNC() return E_NOTIMPL;};

class ATL_NO_VTABLE CCJfsSrv :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCJfsSrv, &CLSID_CJfsSrv>,
	public IDispatchImpl<ICJfsSrv, &IID_ICJfsSrv, &LIBID_JCSrvLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDiskSearchNotify,
	public IProcExit
{
public:
	CCJfsSrv()
	{
		
		m_checkPid.SetCallback(this);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CJFSSRV)


BEGIN_COM_MAP(CCJfsSrv)
	COM_INTERFACE_ENTRY(ICJfsSrv)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDiskSearchNotify)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()


	HRESULT FinalConstruct();
	void FinalRelease();

protected://IDiskSearchNotify
	STDMETHOD(OnDiskSearch_FileChange)(WCHAR cDosName, LPCWSTR lpFile, DWORD dwAction, DWORD dwAttr);
	STDMETHOD(OnDiskSearch_Progress)(WCHAR cDosName, DWORD dwTotalFile, DWORD dwCur);
	STDMETHOD(OnDiskSearch_FileCountChange)(WCHAR cDosName, DWORD dwFileCount, DWORD dwDirCount);
	STDMETHOD(OnDiskSearch_StateChangeNotify)(WCHAR cDosName, INT nMsg, WPARAM wParam, LPARAM lParam);
	STDMETHOD(OnDiskSearch_Result)( DWORD dwCount, DWORD dwTickCount);
public:


private:
	CComQIPtr<ICJCfsCallBack> m_pCallBack;
	UTIL::com_ptr<IMscomRunningObjectTable> m_pRot;
	UTIL::com_ptr<IDiskSearchCli>		m_pFileSearch;
public:
	STDMETHOD(Init)(IUnknown* pCallBack, DWORD dwPid);
	STDMETHOD(UnInit)(void);

	STDMETHOD(GetState)(DWORD* dwState);
	STDMETHOD(Query)(DWORD dwConditionMask, BSTR lpstrIncludeDir, BSTR lpstrExtension, BSTR lpstrFileName);
	STDMETHOD(GetResultAt)(DWORD dwIndex, DWORD GetValueMask, BSTR* szName, BSTR* szPath, BSTR* szExtension, DWORD* dwLeave, ULONGLONG*	ullFileSize,
		ULONGLONG*	ullMondifyTime,
		ULONGLONG*	ullCreateTime,
		ULONGLONG*	ullAccessTime,
		DWORD*		dwFileAttr,
		DWORD*		dwIconIndex);
	STDMETHOD(SetFileMark)(BSTR lpszFile, DWORD dwLeave);

	virtual HRESULT OnAddProc(HANDLE hHandle, DWORD dwData);

	virtual HRESULT OnProcExit(HANDLE hHandle);

private:
	CCheckProcExit m_checkPid;
};

OBJECT_ENTRY_AUTO(__uuidof(CJfsSrv), CCJfsSrv)
