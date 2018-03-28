#pragma once


enum
{
	SOFT_MATCH_AUTO,
	SOFT_MATCH_FULL,
};

struct ISSoft : public IMSBase
{
	//����ƥ�������-1 ʧ��
	virtual DWORD GetMatch( LPCWSTR strMatch , DWORD dwMatch = SOFT_MATCH_AUTO) = 0 ;

	//����ƥ�䵽�Ŀ�ݷ�ʽ·��
	virtual LPCWSTR GetLinkPath( DWORD dwIndex) = 0;

	//��ȡƥ�������
	virtual LPCWSTR GetName( DWORD dwIndex) = 0;
};

MS_DEFINE_IID(ISSoft, "{B18829D2-08A9-4E17-B5A6-EEB134E335A5}");

// {8CBC2B6B-40CD-4342-BF90-C289F77C0988}
MS_DEFINE_GUID(CLSID_SSoft, 
	0x8cbc2b6b, 0x40cd, 0x4342, 0xbf, 0x90, 0xc2, 0x89, 0xf7, 0x7c, 0x9, 0x88);
