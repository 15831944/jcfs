#pragma once
//�ַ���������
const int ALPHABETA_SIZE = 0x80; /**/
extern BYTE g_NoCaseTable[ALPHABETA_SIZE]; /*�ӿ��ַ�����Сдת��*/

#pragma pack(push)
#pragma pack(1)

struct JC_FILE_BASIC_INFO{
	JC_FILE_BASIC_INFO( DWORD FileReferenceNumber)
	{
		Index = FileReferenceNumber;
	}

	JC_FILE_BASIC_INFO()
	{
	}

	union{
		struct{
			DWORD NoNascii			: 1;	//�Ƿ�������
			DWORD TwoByteNameLen	: 1;	//0��ʾ�ļ�����ռ1�ֽ� 1�����ļ�����ռ2�ֽ�
			DWORD ExtensionLen		: 2;	//��ʾ��չ���ֽ���0 1 4
			DWORD Leave				: 3;	//�ļ�����
			DWORD IsSystem			: 1;	//ϵͳ�ļ�
			DWORD IsHide			: 1;	//�����ļ�
			DWORD Index				: 23;	//�ļ�����
		};
		DWORD BasicInfo;
	};
};
/*
struct JC_DIRECTORY_BASIC_INFO{
	union {
		struct {
			DWORD IsSystem			: 1;	//ϵͳ�ļ�
			DWORD IsHide			: 1;	//�����ļ�
			DWORD Leave				: 3;	//�ļ�����
			DWORD DosDrive			: 5;	//������
			DWORD Index				: 22;	//�ļ�����
		};
		DWORD BasicInfo;
	};
};
*/


/*Ŀ¼�洢�ṹ*/

/*
struct DIRECTORY_RECORD_NAME_LEN
{
	union{
		struct{
			WORD NameHasTwoByte : 1;	//���λ�Ƿ���>0x7F
			WORD NameLen : 7;
		};
		WORD   NameLenInfo;
	};
};
*/
/*
typedef struct DIRECTORY_RECORD_INFO{
	//4λ
	JC_DIRECTORY_BASIC_INFO BasicInfo;
	union{
		DIRECTORY_RECORD_INFO * pParentDirRecord;	   //��������ʱ�õ�
		DWORD dwParentFileReferenceNumber;			   //��ʼ��ʱ�õ�
	};

	DIRECTORY_RECORD_NAME_LEN            NameLenInfo;

	//����ʾ�ļ�����
	BYTE            Name[1];            //������ �ļ���
}DIRECTORY_RECORD_INFO,*PDIRECTORY_RECORD_INFO;
*/


/*�ļ��洢�ṹ*/
typedef struct FILE_RECORD_INFO{


	bool operator==(const FILE_RECORD_INFO& l)
	{
		return BasicInfo.Index == l.BasicInfo.Index;
	}

	bool operator<(const FILE_RECORD_INFO& l)
	{
		return BasicInfo.Index < l.BasicInfo.Index;
	}

	JC_FILE_BASIC_INFO BasicInfo; 

	union{
		FILE_RECORD_INFO*	pParentDirRecord;				//��Ŀ¼��ָ��
		DWORD	dwParentFileReferenceNumber;    //���ڳ�ʼʱ����Ŀ¼��
	};

	BYTE            NameInformation[1]; //�洢��ʽ:�ļ������볤��+�ļ�������+[��չ��ID ��ext����]
}FILE_RECORD_INFO, *PFILE_RECORD_INFO;


#pragma pack(pop)

struct mpool;

//�����ļ�
//���ر����ĳ���
DWORD EncodFileName(LPCWSTR lpszFileName, const DWORD& dwNameLen, BYTE* szFileNameCode );
DWORD DecodeFileName(BYTE* szFileNameCode, const DWORD& dwCodeLen, LPWSTR wFileName);
DWORD DecodeFileNameNoCase(BYTE* szFileNameCode, const DWORD& dwCodeLen, LPWSTR wFileName);


DWORD GetFileName(PFILE_RECORD_INFO fileInfo, LPWSTR wFileName);
DWORD GetFileNameNoCase(PFILE_RECORD_INFO fileInfo, LPWSTR wFileName);

DWORD GetFileMemLenght(PFILE_RECORD_INFO fileInfo);

//��ȡ��չ��������ռ�õ��ֽ���
DWORD GetExtendID(PFILE_RECORD_INFO fileInfo);
DWORD GetFileExtendIdMemLen(WORD dwID);

//��ȡ�ļ���������Ҫռ�õ��ֽ���
DWORD GetFileNameLenMemLen(DWORD dwFileNameLen);

int GetFilePath( PFILE_RECORD_INFO, LPWSTR path);

inline DWORD GetResult(DWORD volume, DWORD file, DWORD leave, DWORD index)
{
	volume -= 'C';
	return QUERY_RESULT_INDEX(volume, file, leave, index).Result;
}

inline DWORD GetResultLeave(DWORD dwResult)
{
	return QUERY_RESULT_INDEX(dwResult).Leave;
}

inline CHAR GetResultVolume(DWORD dwResult)
{
	return QUERY_RESULT_INDEX(dwResult).Volume + 'C';
}

inline BOOL IsResultFile(DWORD dwResult)
{
	return QUERY_RESULT_INDEX(dwResult).IsFile;
}

inline BOOL GetResultIndex(DWORD dwResult)
{
	return QUERY_RESULT_INDEX(dwResult).Index;
}

static DWORD GetCoreVersions()
{
	return 4;
}

static DWORDLONG GetCurrentTime2()
{
	DWORDLONG dwCurrentTime = 0;

	SYSTEMTIME systemTime = { 0 };

	GetSystemTime(&systemTime);

	PWORD pTime = (PWORD)&dwCurrentTime;
	pTime[0] = systemTime.wYear;
	pTime[1] = systemTime.wMonth;
	pTime[2] = systemTime.wHour;
	pTime[3] = systemTime.wMinute;

	return dwCurrentTime;
}

WCHAR UpCase( WCHAR cch);

DWORD GetVolumeVolumeSerialNumber(WCHAR cDosName);

PFILE_RECORD_INFO CreateFileInfo(
	mpool*						pool,			//�ڴ��
	const DWORD&			fileBasicInfo,	
	const DWORD&			parentInfo,
	LPCWSTR						lpszFileName,	//�ļ�����
	const DWORD&				dwFileLen,		//�ļ�������
	const WORD&					pwExtensionId	//��չ��ID
	);


VOID DestroyFileInfo(mpool*	pool, PFILE_RECORD_INFO info);