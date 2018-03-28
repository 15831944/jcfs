#pragma once

//ǰ������
namespace msdk{
	namespace mscom{
		struct IMsDwordVector;
	};};

/*�򿪲�������*/
enum{
	OpenDiskMask_UseCache			= ( 1 << 1 ),			//ʹ��Cache����,lpszNameSpaceΪ����Ĭ��ʹ��Cache
	OpenDiskMask_DeleteCacheFile  = (OpenDiskMask_UseCache << 1), //ɾ����ǰCACHE���ļ�
};


/*�ļ���������*/
enum{//OnFileChange
	FileAction_New,		//�����ļ�
	FileAction_Del,		//ɾ���ļ�
	FileAction_Rm,		//������
	FileAction_OldRm,	//������ǰ������
};


/*���̵�ǰ��״̬*/
enum //OnDiskSearch_StateChangeNotify
{
	DiskState_UnKnown,
	DiskState_Ready,	//׼������
	DiskState_Scaning,	//����ɨ��
	DiskState_Finish,	//���
	DiskState_Failed,	//ʧ��

	//һ���Ƿ����ʹ�õ�
	DiskState_Mount,	//����
	DiskState_uMount,	//ж��
};

/*�ļ�����*/
enum
{
	FileLeave_0,	
	FileLeave_1,
	FileLeave_2,
	FileLeave_3,
	FileLeave_4,
	FileLeave_5,
	FileLeave_6,
	FileLeave_7,
};


//��ѯ��������
enum
{
	QueryConditionMask_File		=	  (1)	<< 1,	//�����ļ�
	QueryConditionMask_Dir		=	  (1)	<< 2,	//����Ŀ¼
	QueryConditionMask_Case		=	  (1)	<< 4,	//��Сд����
	QueryConditionMask_Full		=	  (1)	<< 5,	//ȫ��ƥ��
	QueryConditionMask_Sub		=	  (1)	<< 6,	//Ŀ¼���˵�ʱ���Ƿ������Ŀ¼����������ã��򲻰�����
	
	QueryConditionMask_Exclude_System =		(1)	<< 7,	//�ų�ϵͳ�ļ�
	QueryConditionMask_Exclude_Hidden =		(1)	<< 8,	//�ų����ص��ļ�
	QueryConditionMask_FullExt		  =		(1)	<< 9,	//����ƥ���׺
};

struct QueryCondition
{
	QueryCondition()
	{
		dwSize = 0;
		dwConditionMask = /*QueryConditionMask_File | QueryConditionMask_Dir*/0;
		lpstrIncludeDir = _T("");
		lpstrExtension  = _T("");
		lpstrFileName   = _T("");
	}

	DWORD	dwSize;				//sizeof(QueryCondition)
	DWORD	dwConditionMask;	//��ѯ�������� QueryConditionMask_All

	//��Ҫ�������ļ���:���Ϊ�գ���Ĭ��������ǰ�����������ڵĸ�Ŀ¼��
	LPCWSTR lpstrIncludeDir;		//������·��,��(|)�ָ�;����ÿ��·������'\'��β
	LPCWSTR lpstrExtension;		//������չ��,��(|)�ָ�

	LPCWSTR	lpstrFileName;		//���ļ����Ʋ�ѯ��֧��ģ��ƥ��
};


//��ѯ�������
struct QUERY_RESULT_INDEX
{
	QUERY_RESULT_INDEX(CHAR d, BOOL f, CHAR l, DWORD i):
		Volume(d),IsFile(f),Leave(l),Index(i){}

	QUERY_RESULT_INDEX(DWORD r):Result(r){}

	union {
		DWORD Result;
		struct {
			DWORD Volume 	: 5;	//������
			DWORD IsFile   	: 1; //�Ƿ�Ϊ�ļ�
			DWORD Leave    	: 3; //���ȼ�
			DWORD Index	   	: 23;//����
		};
	};
};



enum 
{
	GetValueMask_IcoIndex		= (1)	<<	1,	//��ȡͼ������
	GetValueMask_FileSize		= (1)	<<	2,	//��ȡ�ļ���С
	GetValueMask_MondifyTime	= (1)	<<	3,	//��ȡ�޸�ʱ��
	GetValueMask_CreateTime		= (1)	<<	4,	//��ȡ����ʱ��
	GetValueMask_AccessTime		= (1)	<<	5,	//��ȡ����ʱ��
	GetValueMask_Attributes		= (1)	<<	6,	//��ȡ����
};

struct SearchResult
{
	WCHAR		szName[MAX_PATH];			//����
	WCHAR		szPath[MAX_PATH];			//·��
	WCHAR		szExtension[MAX_PATH];		//��չ��
	
	ULONGLONG	ullFileSize;				//�ļ���С

	ULONGLONG	ullMondifyTime;				//�޸�ʱ��
	ULONGLONG	ullCreateTime;				//����ʱ��
	ULONGLONG	ullAccessTime;				//����ʱ��

	DWORD		dwFileAttr;					//�ļ�����

	DWORD		dwLeave;
	DWORD		dwIconIndex;				//ͼ������
};

struct IDiskSearch;
struct IDiskSearchNotify : public IMSBase
{
	//�ļ������䶯
	STDMETHOD(OnDiskSearch_FileChange)(WCHAR cDosName, LPCWSTR lpFile, DWORD dwAction, DWORD dwAttr) = 0;

	//���ȱ���
	STDMETHOD(OnDiskSearch_Progress)(WCHAR cDosName, DWORD dwTotalFile, DWORD dwCur) = 0;

	//�ļ����������仯
	STDMETHOD(OnDiskSearch_FileCountChange)(WCHAR cDosName, DWORD dwFileCount, DWORD dwDirCount) = 0;

	//״̬�����仯
	STDMETHOD(OnDiskSearch_StateChangeNotify)(WCHAR cDosName, INT nMsg, WPARAM wParam, LPARAM lParam) = 0;

	STDMETHOD(OnDiskSearch_Result)( DWORD dwCount, DWORD dwTickCount) = 0;
};

MS_DEFINE_IID(IDiskSearchNotify, "{7E40E819-CD28-4816-88B7-493EDBCBC35D}");



struct IDiskSearch : public IMSBase
{
	STDMETHOD(OpenDisk)(WCHAR cDosName, IDiskSearchNotify* pNotify, DWORD OpenDiskMask = OpenDiskMask_UseCache) = 0;
	STDMETHOD(CloseDisk)() = 0;

	STDMETHOD(LoadDisk)() = 0;
	STDMETHOD(UnLoadDisk)() = 0;

	STDMETHOD_(WCHAR, GetVolumeName)() = 0;
	STDMETHOD_(DWORD, GetCurrentState)() = 0;

	STDMETHOD(LockDisk)() = 0;
	STDMETHOD(UnLockDisk)() = 0;
	STDMETHOD_(BOOL, IsLockDisk)() = 0;

	STDMETHOD(Query)(const QueryCondition& Condition, IMsDwordVector* pDwordVector) = 0;
	STDMETHOD(GetResultAt)( DWORD dwIndex, DWORD GetValueMask,SearchResult& info) = 0;

	STDMETHOD_(DWORDLONG, GetCount)(DWORD* dwFileCount, DWORD* dwDirCount) = 0;


	STDMETHOD(SetFileMark)(LPCWSTR lpszFile, DWORD dwLeave) = 0;
};

MS_DEFINE_IID(IDiskSearch, "{96A167C1-2CAB-47D6-A4BF-BAE83547B045}");

// {6AD7BD55-3181-48B5-9528-11F3C932B8A7}
MS_DEFINE_GUID(CLSID_Ntfs, 
	0x6ad7bd55, 0x3181, 0x48b5, 0x95, 0x28, 0x11, 0xf3, 0xc9, 0x32, 0xb8, 0xa7);

// {EADA0E2D-E66A-4E83-BBB6-4EDE5920892C}
MS_DEFINE_GUID(CLSID_Fat, 
	0xeada0e2d, 0xe66a, 0x4e83, 0xbb, 0xb6, 0x4e, 0xde, 0x59, 0x20, 0x89, 0x2c);




//////////////////////////////////////////////////////////////////////////
//�ļ������������
struct IDiskSearchSrv : public IMSBase
{
	STDMETHOD_(DWORD, GetSearchDisk)(IMSBase /*IProperty2*/** pSerchDisk) = 0;
	STDMETHOD(GetSearchDisk)(WCHAR cDosName, IMSBase/*IDiskSearch*/** pDisk) = 0;
	STDMETHOD(Lock)() = 0;
	STDMETHOD(UnLock)() = 0;
	STDMETHOD(SetFileMark)(LPCWSTR lpszFile, DWORD dwLeave) = 0;

	STDMETHOD_(DWORD,GetState)() = 0;
	STDMETHOD(ReBuildIndex)() = 0;
};

MS_DEFINE_IID(IDiskSearchSrv,"{9D361F24-1B63-4E5D-98A6-373A992A4F4A}");


// {B4EFD843-3001-4602-9B3E-1BA2245DB6F7}
MS_DEFINE_GUID(CLSID_DiskSearchSrv, 
	0xb4efd843, 0x3001, 0x4602, 0x9b, 0x3e, 0x1b, 0xa2, 0x24, 0x5d, 0xb6, 0xf7);

//�ļ������ͻ������
struct IDiskSearchCli : public IMSBase
{
	STDMETHOD(Init)(IDiskSearchNotify* pClientNotify) = 0;
	STDMETHOD(UnInit)() = 0;

	STDMETHOD(Query)(QueryCondition& queryCondition) = 0;
	STDMETHOD(GetResultAt)(DWORD dwIndex, DWORD GetValueMask, SearchResult& ResultInfo)  = 0;

	STDMETHOD_(DWORD, GetState)() = 0;

	STDMETHOD(SetFileMark)(LPCWSTR lpszFile, DWORD dwLeave) = 0;
};

MS_DEFINE_IID(IDiskSearchCli, "{AE68E005-4E7A-4495-BF58-8131744C7379}");

// {11EBD2AD-DC63-496B-8359-561072B9C009}
MS_DEFINE_GUID(CLSID_DiskSearchCli, 
	0x11ebd2ad, 0xdc63, 0x496b, 0x83, 0x59, 0x56, 0x10, 0x72, 0xb9, 0xc0, 0x9);


//RPC ���̵��ýӿ�
// {505E90B5-222A-4C5D-94B9-0FE39621503D}
MS_DEFINE_GUID(CLSID_RpcDiskSearchCli,
	0x505E90B5,0x222A,0x4C5D,0x94,0xB9,0x0F,0xE3,0x96,0x21,0x50,0x3D);