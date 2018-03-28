// CDeskBandExt.h : CDeskBandExt ������

#pragma once
#include "resource.h"       // ������



#include "JCDeskBand_i.h"

#include <shobjidl.h>
#include <ShlGuid.h>
#include <atlstr.h>
#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CDeskBandExt

class ATL_NO_VTABLE CDeskBandExt :
	public CWindowImpl<CDeskBandExt>,  
	public CComObjectRootEx<CComSingleThreadModel>,  
	public CComCoClass<CDeskBandExt, &CLSID_CDeskBandExt>,  
	public IObjectWithSiteImpl<CDeskBandExt>,  
	public IDispatchImpl<ICDeskBandExt, &IID_ICDeskBandExt, &LIBID_JCDeskBandLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,  
	public IPersistStream,  
	public IContextMenu,  
	public IInputObject,  
	public IDeskBand2,
	public IDuiBuilderCallBack
{
public:
	CDeskBandExt();
	~CDeskBandExt();
DECLARE_REGISTRY_RESOURCEID(IDR_CDESKBANDEXT)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDeskBandExt)
	COM_INTERFACE_ENTRY(ICDeskBandExt)  
	COM_INTERFACE_ENTRY(IDispatch)  
	COM_INTERFACE_ENTRY(IObjectWithSite)  
	COM_INTERFACE_ENTRY(IOleWindow)  
	COM_INTERFACE_ENTRY(IDockingWindow)  
	COM_INTERFACE_ENTRY(IDeskBand)  
	COM_INTERFACE_ENTRY(IDeskBand2)  
	COM_INTERFACE_ENTRY(IInputObject)  
	COM_INTERFACE_ENTRY(IPersist)  
	COM_INTERFACE_ENTRY(IPersistStream)  
END_COM_MAP()

BEGIN_CATEGORY_MAP(CDeskBandExt)  
	IMPLEMENTED_CATEGORY(CATID_DeskBand)  
END_CATEGORY_MAP() 

BEGIN_MSG_MAP(CDeskBandExt)  
	//MESSAGE_HANDLER(WM_SIZE, OnSize)  
	//MESSAGE_HANDLER(WM_PAINT, OnPaint)  
	//MESSAGE_HANDLER(WM_TIMER, OnTimer)  
	MESSAGE_HANDLER(WM_CREATE, OnCreate)  
	//MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)  
	//MESSAGE_HANDLER(WM_COMMAND, OnCommand)  
	//MESSAGE_HANDLER(WM_KILLFOCUS, OnFocus)  
	//MESSAGE_HANDLER(WM_DESTROY, OnDestroy)  
	//MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)  
	//MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)  
	//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)  
	//MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)  
	//MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)  
	MESSAGE_HANDLER(WM_THEMECHANGED, OnThemeChanged)  
	//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)  
	MESSAGE_HANDLER(WM_POWERBROADCAST, OnPowerBroadcast)
	//if (!bHandled) return lResult;
END_MSG_MAP()  

	

	HRESULT FinalConstruct();
	void FinalRelease();
	POINTL CalcIdealSize() const;


	//CWindowImpl
// 	static LPCTSTR GetWndClassName() throw()
// 	{
// 		return _T("jc_desk_band");
// 	}
public:  
	// IObjectWithSite  
	STDMETHOD (SetSite)(IUnknown *);  
	STDMETHOD (GetSite)(REFIID, LPVOID*);  

public:  
	// IInputObject  
	STDMETHOD (HasFocusIO)();  
	STDMETHOD (UIActivateIO)(BOOL,MSG *);  
	STDMETHOD (TranslateAcceleratorIO)(MSG *);  

public:  
	// IDeskBand  
	STDMETHOD (GetBandInfo)(DWORD, DWORD, DESKBANDINFO *);  

public:  
	// IDeskBand2  
	STDMETHOD (CanRenderComposited)(BOOL *);  
	STDMETHOD (SetCompositionState)(BOOL);  
	STDMETHOD (GetCompositionState)(BOOL *);  

public:  
	// IOleWindow  
	STDMETHOD (GetWindow)(HWND *);  
	STDMETHOD (ContextSensitiveHelp)(BOOL);  

public:  
	// IDockingWindow  
	STDMETHOD (ShowDW)(BOOL);  
	STDMETHOD (CloseDW)(DWORD);  
	STDMETHOD (ResizeBorderDW)(LPCRECT,IUnknown *, BOOL);  

public:  
	// IPersistStream  
	STDMETHOD (GetClassID)(LPCLSID);  
	STDMETHOD (IsDirty)(void);  
	STDMETHOD (Load)(LPSTREAM);  
	STDMETHOD (Save)(LPSTREAM, BOOL);  
	STDMETHOD (GetSizeMax)(ULARGE_INTEGER*);  

public:  
	// IContextMenu methods  
	STDMETHOD (QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);  
	STDMETHOD (InvokeCommand)(LPCMINVOKECOMMANDINFO);  
	STDMETHOD (GetCommandString)(UINT_PTR, UINT, LPUINT, LPSTR, UINT);  

public://IDuiBuilderCallBack
	virtual LRESULT OnBuilderCallBack( IDuiObject* pObj, int nCallBack);
private:  
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnFocus(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnRButtonUp(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnMouseHover(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnThemeChanged(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnPowerBroadcast(UINT, WPARAM, LPARAM, BOOL&);  
	LRESULT OnEraseBackground(UINT, WPARAM, LPARAM, BOOL&);  




private:
	void OnFinalMessage( HWND hWnd );

private:  
	BOOL m_bFocus;  
	int m_nBandID;  

	HMENU m_hMenu;  
	HWND m_hwndParent;  

	DWORD m_dwViewMode;  
	//CAutoPtr<CStyle> m_pStyle;  
	BOOL m_bCompositionEnabled;  
	IInputObjectSite *m_pSite;  

	CString m_lastString;  
	CString m_Ss;  
	int m_sl;  


private:
	CDuiObjectPtr<IDuiCore, IIDuiCore> m_pDuiCore;
	CDuiObjectPtr<IDuiSkin, IIDuiSkin> m_pDuiSkin;
	CDuiObjectPtr<IDuiWindowCtrl, IIDuiWindowCtrl> m_pHostWnd;
};

OBJECT_ENTRY_AUTO(__uuidof(CDeskBandExt), CDeskBandExt)
