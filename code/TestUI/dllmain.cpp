// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "TestWindow.h"
USE_DEFAULT_DLL_MAIN;
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY(CLSID_TestWindow,CTestWindow)
	//CLIDMAPENTRY(CLSID_DiskSearchSrv,CDiskSearchSrv)
	//CLIDMAPENTRY(CLSID_Fat,CNtfsDisk)
	CLIDMAPENTRY_END
END_CLIDMAP
DEFINE_ALL_EXPORTFUN

