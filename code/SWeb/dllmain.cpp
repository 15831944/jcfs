// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "WebBaidu.h"
#include "WebHaosou.h"
#include "WebSogou.h"
USE_DEFAULT_DLL_MAIN;
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY(CLSID_WebBaidu,CWebBaidu)
		CLIDMAPENTRY(CLSID_WebHaosou,CWebHaosou)
		CLIDMAPENTRY(CLSID_WebSogou,CWebSogou)
	CLIDMAPENTRY_END
END_CLIDMAP_AND_EXPORTFUN;