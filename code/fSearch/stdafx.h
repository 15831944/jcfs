// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include "mscom/mscominc.h"
using namespace msdk;

#include <mslog/stdlog_dll.h>
#define GroupName _T("fSearch")

#define MODULE_NAME GroupName
#include <mslog/msdkoutput.h>
#include <IfSearch.h>
#include <atlstr.h>
