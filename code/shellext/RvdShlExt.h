// JCfsShlExt.h : Declaration of the CRvdShlExt

#ifndef __RVDSHLEXT_H_
#define __RVDSHLEXT_H_

#include "resource.h"       // main symbols
#include <shlobj.h>
#include <comdef.h>
#include <atlstr.h>

#include "commx\commx3.h"
using namespace msdk;

//#include "vdiskoperate.h"
/////////////////////////////////////////////////////////////////////////////
// CRvdShlExt
class ATL_NO_VTABLE CRvdShlExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRvdShlExt, &CLSID_JCfsShlExt>,
	public IDispatchImpl<IJCfsShlExt, &IID_IJCfsShlExt, &LIBID_JCFSEXTLib>,
    public IShellExtInit,
    public IContextMenu
	{
public:
	CRvdShlExt();
	virtual ~CRvdShlExt();

DECLARE_REGISTRY_RESOURCEID(IDR_RVDSHLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRvdShlExt)
	COM_INTERFACE_ENTRY(IJCfsShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

// IJCfsShlExt
public:
INT GetRvdPath(LPTSTR,INT);
protected:
	HBITMAP      m_hBmp;
	HBITMAP      m_hBmpCreate;
    TCHAR        m_szFile [MAX_PATH];
    TCHAR		 m_szRvdBmpFile[MAX_PATH];
	INT          m_flag;//�Ҽ��������������հ״��������̣������ļ�����
	CString      m_szText;//�洢���������Ҫ������״̬��״̬����ʾ
    CString      m_strPath;//�洢����״̬�µ��Ҽ����·��
//	CVDiskOperateProxy m_diskOperate;
public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	
    // IContextMenu
    STDMETHOD(GetCommandString)(UINT_PTR, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
private:


private:
	};
	
#endif //CRvdShlExt
