// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once


#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES


#include <atlbase.h>	// ������ATL��
#include <atlstr.h>
#include <atltypes.h>
#include <atlapp.h>		// ������WTL��


#include <atlwin.h>		// ATL������
#include <atlcrack.h>	// WTL��ǿ����Ϣ��
//#include <atlsplit.h>
#include <atlframe.h>	// WTL����ܴ�����
//#include <atlgdi.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlmisc.h>	// WTLʵ�ù�����

#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>




#include "resource.h"
#include "res\resource.h"
using namespace SOUI;

#include <mscom/mscominc.h>
using namespace mscom;

#define GroupName _T("JCfsUI")
#define MODULE_NAME GroupName
#include <mslog/stdlog_dll.h>
#include <mslog/msdkoutput.h>
#include <shlobj.h>
