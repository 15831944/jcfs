// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "SouiInit.h"

USE_DEFAULT_DLL_MAIN;
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
		CLIDMAPENTRY(CLSID_SouiInit,CSouiInit)
	CLIDMAPENTRY_END
END_CLIDMAP_AND_EXPORTFUN;
