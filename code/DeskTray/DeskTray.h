#pragma once
#include <IDeskTray.h>
#include <mscom/SrvBaseImp.h>
#include <tray/traysrvplugin.h>
#include <tray/traywndplugin.h>
#include <mscomhelper/UseConnectionPoint.h>

#include <JCDeskMsc.h>

class CDeskTray : 
	public CSrvPluginImp<CDeskTray>,
	public ITrayMenuConnectPoint,
	public ITrayMsgConnectPoint,
	public IExit
{
public:
	UNKNOWN_IMP5_( IMsPlugin , IMsPluginRun, IExit,ITrayMenuConnectPoint,ITrayMsgConnectPoint);
	CDeskTray(void);
	~CDeskTray(void);
public:

	

	//CSrvPluginImp
	virtual HRESULT OnAfterInit(void*);
	virtual HRESULT OnBeforeUnint();
	virtual HRESULT OnAfterStart();
	virtual HRESULT OnBeforeStop();
	virtual VOID WINAPI FireConnectBroken();
protected:
	//ITrayMenuConnectPoint
	STDMETHOD_(LRESULT, OnShowLeftButtonMenu)(INT x, INT y, BOOL& bHandle) ;		//����˵�
	STDMETHOD_(LRESULT, OnShowRightButtonMenu)(INT x, INT y, BOOL& bHandle);		//�Ҽ��˵�
	STDMETHOD_(LRESULT, OnMenuCommand)(WORD nMenuId, BOOL &bHandle);
	STDMETHOD_(LRESULT, OnMenuCommand)(UINT nMenuIndex, HMENU hMenu, BOOL &bHandle);
	STDMETHOD_(LRESULT, OnDefaultMenu)(BOOL &bHandle);	//���˫����ӦĬ�ϲ˵�

protected:
	//ITrayMsgConnectPoint
	STDMETHOD_(LRESULT, OnTrayIconMsg)(LPARAM lParam, INT x, INT y, BOOL& bHandle);	//����ͼ����Ϣ
	STDMETHOD_(LRESULT, OnBalloonClicked)(UINT nActionID, BOOL& bHandle);				//���򱻵��
	STDMETHOD_(LRESULT, OnOtherMsg)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandle); //������Ϣ

protected:
	STDMETHOD(NotifyExit)(bool* bExit = NULL);

private:
	VOID DoReportActive();
private:
	UTIL::com_ptr<ITraySrv>					m_pTraySrv	; //����
	UseConnectPoint<ITrayMenuConnectPoint>  m_TrayMenuConnectPoint;
	UseConnectPoint<ITrayMsgConnectPoint>	m_TrayMsgConnectPoint;


private:

};

