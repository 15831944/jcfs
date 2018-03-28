#pragma once

//���ӵ�

enum DeskBandItem
{
	DeskBandItem_TypeButton,		//���Ͱ�ť
	DeskBandItem_SearchButton,		//������ť
};

struct IJCDeskBandConnectPoint : public IMSBase
{
	STDMETHOD(OnDeskBand_KeyDown)(int nKey) = 0;				//
	STDMETHOD(OnDeskBand_TextChang)() = 0;				//���ݱ仯
	STDMETHOD(OnDeskBand_Click)( int nDeskBandItem) = 0;//������ť�����
	STDMETHOD(OnDeskBand_Show)(BOOL bShow) = 0;			//��������ʾ
	STDMETHOD(OnDeskBand_SetFocus)(BOOL bSet) = 0;		//�Ƿ�����Ϊ����
};
MS_DEFINE_IID(IJCDeskBandConnectPoint, "{44D7A1DB-495E-4F8A-B1B4-D9B70ED2ACDE}");


enum DeskBandPos
{
	DeskBandPos_None,
	DeskBandPos_Top,
	DeskBandPos_Botton,
	DeskBandPos_Left,
	DeskBandPos_Rigth,
};
struct IJCDeskBand : public IMSBase
{
	STDMETHOD(Show)( BOOL bShow) = 0;
	STDMETHOD_(BOOL,Show)() = 0;
	STDMETHOD_(HWND, GetDeskBandWnd)() = 0;
	STDMETHOD_(HWND, GetParentWnd)() = 0;
	STDMETHOD_(DWORD, GetText)( LPWSTR lpszText, DWORD dwLen) =0;
	STDMETHOD(SetImage)( LPCWSTR lpszPath) = 0;		//���õ�ǰ��ͼ��
	//��ȡ���ڵ�λ��
	STDMETHOD_(DWORD, GetDeskBandPos)() = 0;
};

MS_DEFINE_IID(IJCDeskBand, "{31D9B93F-16BF-4105-976A-A20E4F45F7CE}");


// {A8F76E43-A3B4-4551-8425-FCD685AB80D0}
MS_DEFINE_GUID(CLSID_JCDeskBand, 
	0xa8f76e43, 0xa3b4, 0x4551, 0x84, 0x25, 0xfc, 0xd6, 0x85, 0xab, 0x80, 0xd0);
