#pragma once
#include "StrMatch.h"
#include "SyncObject/criticalsection.h"
#include <shellapi.h>
#include <msapi/msapp.h>
#include <vector>
//��չ��������,û�б�Ҫ���ڴ��


//��չ�����ݿ⣬������չ���������ٹʸÿ������ɾ
//������չ�����ֵ���������

#define ICON_INDEX_UNINITIALIZED    -3
#define ICON_INDEX_REALTIMECALL     -2
#define ICON_INDEX_NOICON           -1

static PWCHAR g_pOmitExt[]=
{
	 L"cur"
	,L"exe"
	,L"ico"
	,L"lnk"
};

class CExtenNameMgr
{
public:
	//��չ����Ϣ
	struct ExtenNameInfo
	{
		LPCWSTR  lpszExtenName;//��չ��
		INT		 nIconID;	   //��չ����Ӧ��ID
	};

	CExtenNameMgr()
	{
		ZeroMemory(&m_Mpool, sizeof(m_Mpool));
		mpool_init_default(&m_Mpool, 4096,1024);
		Clear();
		m_dwJournalID = 0;
		m_bLoad  = FALSE;
		m_bWrite = FALSE;
	}

	~CExtenNameMgr()
	{
		mpool_destroy(&m_Mpool);
	}
	
	inline INT Find(LPCWSTR lpszName)
	{
		if(!lpszName) return -1;

		//�滻�ɸ���������㷨
		INT nCount = m_ExtenNameInfo.size();
		for (INT nIndex = 0 ; nIndex < nCount ; ++nIndex)
		{
			ExtenNameInfo& pInfo = m_ExtenNameInfo.at(nIndex);
			if(inline_wcscmp(lpszName,pInfo.lpszExtenName) == 0)
			{
				return nIndex + 1; //�ҵ���
			}
		}

		return -1;
	}

	inline std::vector<int> Find2(LPCWSTR lpszName)
	{
		std::vector<int> v;
		if(!lpszName) return v;

		//�滻�ɸ���������㷨
		INT nCount = m_ExtenNameInfo.size();
		for (INT nIndex = 0 ; nIndex < nCount ; ++nIndex)
		{
			ExtenNameInfo& pInfo = m_ExtenNameInfo.at(nIndex);
			if(wcsstr (pInfo.lpszExtenName, lpszName) != 0)
			{
				v.push_back(nIndex + 1); //�ҵ���
			}
		}

		return v;
	}

	/*ͨ����չ������*/
	inline INT InsertExtenName(LPCWSTR lpszName, const DWORD& dwLen,INT nIconIndex = ICON_INDEX_NOICON)
	{
		AUTOLOCK_CS(s);

		if(!lpszName) return -1;

		WCHAR szExtension[MAX_PATH];
		inline_wcsncpy(szExtension, lpszName,dwLen);
		inline_wcswr(szExtension,dwLen);
		//�ҵ��˾;Ͳ���Ҫ�ٴβ�����
		INT nIndex = Find(szExtension);
		if (nIndex != -1)
		{
			return nIndex;
		}

		DWORD dwAlloc = dwLen * 2 + 2;
		LPWSTR pExt = (LPWSTR)mpool_salloc(&m_Mpool, dwAlloc);
	
		ZeroMemory(pExt,dwAlloc);
		inline_wcscpy(pExt,szExtension);

		ExtenNameInfo info = {pExt,nIconIndex};
		m_ExtenNameInfo.insert(m_ExtenNameInfo.end(), info);

		return m_ExtenNameInfo.size();
	}

	/*��ȡͼ��ID*/
	inline INT GetIconId(DWORD dwIndex)
	{
		AUTOLOCK_CS(s);
		ExtenNameInfo& info = m_ExtenNameInfo.at(dwIndex);
		return info.nIconID;
	}

	/*��ȡ�ļ���չ��*/
	inline LPCWSTR GetExtenName(DWORD dwIndex)
	{
		AUTOLOCK_CS(s);
		dwIndex--;
		if ( dwIndex >= m_ExtenNameInfo.size())
		{
			return L"";
		}
		
		ExtenNameInfo& info = m_ExtenNameInfo.at(dwIndex);
		
		return info.lpszExtenName;
		return L"";
	}

	inline VOID SetIconId(DWORD dwIndex,INT nIconID)
	{
		AUTOLOCK_CS(s);
		ExtenNameInfo& info = m_ExtenNameInfo.at(dwIndex);
		info.nIconID = nIconID;
	}

	static inline INT GetDirectoryIcon()
	{
		static INT g_iDirIcon = GetDirectoryIconIndex(L"c:\\",3);
		return g_iDirIcon;
	}
	static inline INT GetRootIcon()
	{
		static INT gRootDirIcon = GetDirectoryIconIndex(_T("D:"),0);
		return gRootDirIcon;
	}

	static inline INT GetSysRootIcon()
	{
		static INT gSysDirIcon = GetDirectoryIconIndex(_T("C:"),0);
		return gSysDirIcon;
	}

	//
public:

	BOOL SetNameSpace(LPCWSTR lpszNameSpace, DWORDLONG dwJournalID)
	{
		m_strNameSpace = lpszNameSpace;
		m_dwJournalID = dwJournalID;
		return TRUE;
	}

	BOOL Clear()
	{
		m_ExtenNameInfo.clear();
		for (INT nIndex = 0 ; nIndex < _countof(g_pOmitExt) ; nIndex++)
		{
			//ExtenNameInfo info = {g_pOmitExt[nIndex],ICON_INDEX_REALTIMECALL};
			InsertExtenName(g_pOmitExt[nIndex],inline_wcslen(g_pOmitExt[nIndex]),ICON_INDEX_REALTIMECALL);
		}

		return TRUE;
	}

	BOOL LoadFromDatabase()
	{
		AUTOLOCK_CS(s);
		if (m_bLoad) return TRUE;  //��ֹ�ظ�����
		m_bLoad = TRUE;

		TCHAR _szExtPath[MAX_PATH] = {0};
		TCHAR szExtPath[MAX_PATH] = {0};
		msapi::CApp(m_strNameSpace.c_str()).GetDataPath(_szExtPath, MAX_PATH);

		_stprintf_s(szExtPath,MAX_PATH ,_T("%s\\fscache\\ext-%I64d.dat"),_szExtPath, m_dwJournalID);
		msapi::CreateDirectoryEx(szExtPath);
		HANDLE hFile = CreateFile(szExtPath
			,GENERIC_READ|GENERIC_WRITE
			,FILE_SHARE_READ|FILE_SHARE_WRITE
			,NULL
			,OPEN_EXISTING
			,FILE_ATTRIBUTE_NORMAL
			,NULL);

		RASSERT(hFile != INVALID_HANDLE_VALUE, FALSE);

		DWORD dwFileSize=GetFileSize(hFile,NULL);
		PBYTE pBuffer = new BYTE[dwFileSize];
		PBYTE pBufferBegin = pBuffer;

		ZeroMemory(pBuffer, dwFileSize);

		DWORD dwRead = 0;
		DWORD dwExtCount = 0;
		ReadFile(hFile,pBuffer,dwFileSize,&dwRead,NULL);

		PBYTE pBufferEnd = pBuffer + dwFileSize;
		dwExtCount = *(DWORD*)(pBuffer);


		int extLen;
		int i = 0 , j = 0;
		int idExt = 0;
		int nIconIndex = 0;
		PWCHAR pName;
		for(pBuffer += 4 ; pBuffer < pBufferEnd ; )
		{
			ExtenNameInfo extinfo;
			idExt=*(int*)pBuffer;  //��չ��ID
			
			pBuffer += 4;
			extinfo.nIconID = *(int*)pBuffer;  //ICON

			pName = PWCHAR(pBuffer + 4);
			extLen = wcslen(pName);     //��չ������

			//LPWSTR pIconName = (LPWSTR)m_Mpool.Alloc(sizeof(WCHAR)*(extLen+1));
			LPWSTR pIconName = (LPWSTR)mpool_salloc(&m_Mpool, (sizeof(WCHAR)*(extLen + 1)));
			extinfo.lpszExtenName = pIconName;


			//m_vpIndexExtName[i].pExtName=m_vpExtName[idExt];
			for(j = 0 ; j < extLen ; ++j )
			{
				pIconName[j]=*pName++;
			}

			pIconName[j]=L'\0';
			pBuffer=PBYTE(pName+1);   

			m_ExtenNameInfo.insert(m_ExtenNameInfo.end(),extinfo);
			//i++;
		}
		SAFE_DELETE_BUFFER(pBufferBegin);
		SAFE_CLOSEHANDLE(hFile);

		//assert(i==m_size);
		//CloseHandle(hFile);
		return TRUE;
	}

	BOOL WriteToDatabase()
	{	
		AUTOLOCK_CS(s);
		if(m_bWrite) return TRUE; //��ֹ�ظ�д��
		m_bWrite = TRUE;

		TCHAR _szExtPath[MAX_PATH] = {0};
		TCHAR szExtPath[MAX_PATH] = {0};
		msapi::CApp(m_strNameSpace.c_str()).GetDataPath(_szExtPath, MAX_PATH);

		_stprintf_s(szExtPath,MAX_PATH ,_T("%s\\fscache\\ext-%I64d.dat"),_szExtPath, m_dwJournalID);

		msapi::CreateDirectoryEx(szExtPath);
		UTIL::sentry<HANDLE, UTIL::handle_sentry> hFile(CreateFileW(szExtPath
			,GENERIC_READ|GENERIC_WRITE
			,FILE_SHARE_READ|FILE_SHARE_WRITE
			,NULL
			,CREATE_ALWAYS
			,FILE_ATTRIBUTE_NORMAL
			,NULL));

		RASSERT(hFile != INVALID_HANDLE_VALUE, FALSE);

		DWORD dwWrited = 0;
		CHAR  Buffer[1024] = {0};
		INT32 nLen = 0;
		int *pId=(int*)Buffer;
		int *pIcon=(int*)(Buffer+4);
		PWCHAR pExtName=(PWCHAR)(Buffer+8),pSrc = NULL;

		//д����չ������
		INT32 dwExtCount = m_ExtenNameInfo.size();
		INT32 dwExitUseCount = dwExtCount- _countof(g_pOmitExt); //��Ч����չ��
		RASSERT(WriteFile(hFile, &dwExitUseCount, sizeof(INT), &dwWrited,NULL), FALSE);
		for(INT32 nIndex = _countof(g_pOmitExt) ; nIndex < dwExtCount ; ++nIndex)
		{      
			*pId = nIndex;
			*pIcon = ICON_INDEX_UNINITIALIZED/*m_ExtenNameInfo[*pId].nIconID*/;
			pSrc = (PWCHAR)m_ExtenNameInfo[*pId].lpszExtenName;
			for(nLen = 0 ; *pSrc ; ++pSrc)
			{
				pExtName[nLen++] = *pSrc;
			}

			pExtName[nLen++] = L'\0';
			nLen=(nLen<<1) + 8;
			RASSERT(WriteFile(hFile, Buffer, nLen, &dwWrited, NULL), FALSE);
			RASSERT(nLen == dwWrited, FALSE);
		}
		return TRUE;
	}
private:
	DECLARE_AUTOLOCK_CS(s);
	std::vector<ExtenNameInfo> m_ExtenNameInfo;  //��չ�����ڵ�ID
	mpool				   m_Mpool;

	BOOL					   m_bLoad;
	BOOL					   m_bWrite;

	std::wstring m_strNameSpace;
	DWORDLONG	 m_dwJournalID;
public:
	inline static int GetDirectoryIconIndex(PWCHAR pszPath,int pathLen) 
	{      
		if(0==pathLen)
		{//��Ŀ¼
			SHFILEINFOW sfi;
			sfi.iIcon = -1;
			WCHAR path[MAX_PATH];      
			//GetSystemDirectoryW(path,MAX_PATH);
			_tcscpy(path, pszPath);
			path[3]=L'\0';
			SHGetFileInfoW(path,  
				FILE_ATTRIBUTE_DIRECTORY,  
				&sfi,  
				sizeof(sfi),  
				/*SHGFI_SMALLICON |*/ SHGFI_SYSICONINDEX 
				/*| SHGFI_USEFILEATTRIBUTES*/
				);
			return sfi.iIcon;
		}else{
			static int dirIconIndex=-1;
			if(-1==dirIconIndex){
				SHFILEINFOW sfi;
				WCHAR path[MAX_PATH];
				GetSystemDirectoryW(path,MAX_PATH);
				SHGetFileInfoW(path,  
					FILE_ATTRIBUTE_DIRECTORY,  
					&sfi,  
					sizeof(sfi),  
					/*SHGFI_SMALLICON |*/ SHGFI_SYSICONINDEX 
					/*| SHGFI_USEFILEATTRIBUTES*/
					);
				dirIconIndex=sfi.iIcon;              
			}
			return dirIconIndex;      
		}
	}

	inline static int GetFileIconIndex(PWCHAR pszFullPath)
	{
		static SHFILEINFOW sfi;
		SHGetFileInfoW(pszFullPath,  
			FILE_ATTRIBUTE_NORMAL,  
			&sfi,  
			sizeof(sfi),  

			//SHGFI_SMALLICON //SHGFI_LARGEICON
			/*|*/SHGFI_SYSICONINDEX 
			//| SHGFI_USEFILEATTRIBUTES
			) ;

		return sfi.iIcon;
	}

	//CPoolMap<>
};
