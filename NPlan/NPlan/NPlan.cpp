//  File:        NPlan.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/18
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 

//
//#include "vld.h"
#include "NPlan.h"
#include "KScreen.h"
#include "KView.h"
#include "mainforwindows.h"
#include "NPlanAppView.h"
#include "KSurfaceManager.h"
#include "./manager/KProjectManager.h"
#include "./manager/KStringManager.h"
#include "./manager/KConfigManager.h"
#include "KFontManager.h"
#include "KKoteiMessage.h"


// 全局变量:
 
HWND g_hWnd = 0;
 
 

//美工资源默认高度
int g_iScreenWidth	= 1024; // 屏幕宽度
int g_iScreenHeight = 700;  // 屏幕高度

CNUIInstance  g_nui_instance;

 
 


bool g_bExit = false;

 

const kn_int VIEW_X = 4;
const kn_int VIEW_Y = 4;
const kn_int VIEW_SIZE = 60;

 
int m_focus_window_id = 0;
int m_old_focus_window_id = 1;
int m_window_num = 0;

#ifdef  WIN32
#include "UserCountSuggestion.h"
#include <WinSock2.h>


void sendUserInfo()
{
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (res != NO_ERROR)
	{
		cout << "Initialize error. Error code = " << WSAGetLastError() << endl;
		WSACleanup();
	}

	UserCountSuggestion *user = new UserCountSuggestion();
	user->QueryRequest();
}

#endif
  
void CreateView()
{
	SetResourcePath();

	int n = 0;

	NPlanAppView_PTR window;
	window = NPlanAppView_PTR(new NPlanAppView);
	window->Create(0,0 , g_iScreenWidth, g_iScreenHeight);
	window->Init();
	window->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	g_nui_instance.getScreen()->AddView( window );

	m_focus_window_id = 0;
	m_window_num = n;
	window->InvalidateView();
}





 


void ReleaseAll()
{
	g_nui_instance.release();
	ReleaseSurfaceManager();
	ReleaseProjectManager();
	releaseConfigManager();
	releaseStringManager();
	DeleteFontManagerSingleton();

}

//====================================================================
// for  andorid &  ios
void ResizeWindow(void* dst, int w, int h)
{
	g_nui_instance.ResizeWindow(dst, w, h);
	g_iScreenWidth = w;
	g_iScreenHeight = h;
}

 

void SendOSMessage(int message, int wParam, int lParam)
{
	if(message == KMSG_TIMER)
	{
		g_nui_instance.SendTimerEvent(0);

	}
	else
	{
		g_nui_instance.NUIWndProc(0, message, wParam, lParam);

	}

}



 
