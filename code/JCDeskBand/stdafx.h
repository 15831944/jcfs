// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW
#define  _WTL_NO_CSTRING	
#include "resource.h"
#include <atlbase.h>
#include <atlapp.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlmisc.h>

#include <WindowsX.h>

#define MSUI_STATIC
#include <DuiApi.h>
using namespace DuiKit;
extern HINSTANCE g_instance;

//extern CPaintManagerUI m_pm;