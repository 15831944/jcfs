// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "DeskTray.h"
USE_DEFAULT_DLL_MAIN;//��Ҫ�滻ԭ����DllMain
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY(CLSID_DeskTray,CDeskTray)
	CLIDMAPENTRY_END
END_CLIDMAP_AND_EXPORTFUN;
