//  File:        NPlan_win32.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/15
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifdef WIN32
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

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例

TCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//KScreen g_screen;
extern HWND g_hWnd;
 
//美工资源默认高度
extern int g_iScreenWidth; // 屏幕宽度
extern int g_iScreenHeight;  // 屏幕高度

extern CNUIInstance  g_nui_instance;





 
extern bool g_bExit;


// 设置资源路径
void SetResourcePath()
{
 
	kn_char szResPath[] = _T("");		// 当前目录
	SetCurrentPath(szResPath);

}

 


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK About( HWND, UINT, WPARAM, LPARAM );

int APIENTRY _tWinMain( HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR	  lpCmdLine,
    int		  nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // TODO: 在此放置代码。
    MSG msg;
    HACCEL hAccelTable;

    // 初始化全局字符串
    LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadString( hInstance, IDC_NPLAN, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

 
    CoInitialize( NULL );   //com 初始化
 

    // 执行应用程序初始化:
    if ( !InitInstance ( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_NPLAN ));

    // 主消息循环:
	g_nui_instance.RunEventLoop(g_bExit);

	ReleaseAll();

 
    CoUninitialize();
 

    //	SAFE_DELETE(g_window);
    //return (int) msg.wParam;
	return 0;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra	 = 0;
    wcex.cbWndExtra	 = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_NPLAN ));
	//wcex.hCursor =NULL;
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HMI);
	wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_NPLAN ));
	
    return RegisterClassEx( &wcex );
}


// 获取可执行文件路径
kn_string GetExeFilePath()
{
	kn_char szFileName[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFileName, MAX_PATH - 1);

	int iLen = _tcslen(szFileName);
	for(int i = iLen - 1; i >= 0; i--)
	{
		if(szFileName[i] != _T('\\'))
		{
			szFileName[i] = 0;
		}
	}

	kn_string str = szFileName;
	return str;
}

int GetScreenSurfaceType()
{
	int iType = WIN_DEVICE;
	kn_string strFlagFile = GetExeFilePath();

	strFlagFile += _T("gpu.txt");

	FILE* fs = _tfopen(strFlagFile.c_str(), _T("rb"));

	if(fs != NULL)
	{
		fclose(fs);

		 iType = SKIAGL_DEVICE;
	}

	return iType;
}

// VOID CALLBACK TimerProc(kn_hwnd hwnd, UINT uMsg, UINT idEvent,  DWORD dwTime)
// {
// 	CheckNUITimer();
// }
// 

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    HWND hWnd;

    hInst = hInstance; // 将实例句柄存储在全局变量中

    hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_POPUP,
        (GetSystemMetrics( SM_CXSCREEN ) - g_iScreenWidth) / 2,
        (GetSystemMetrics( SM_CYSCREEN ) - g_iScreenHeight) / 2 - 20,
        g_iScreenWidth, g_iScreenHeight,
        NULL, NULL, hInstance, NULL );

    if ( !hWnd )
    {
        return FALSE;
    }

    // INIT NUI
//    g_screen.Create( g_iScreenWidth, g_iScreenHeight, REBitmap::kARGB_8888_Config, hWnd);// ,SCREEN_RENDER_MAIN_THREAD);
	KNUIInitPara para;
	para.m_w = g_iScreenWidth;
	para.m_h = g_iScreenHeight;
	para.m_wnd = hWnd;
	para.m_render_mode = SCREEN_RENDER_MULTI_THREAD;

	int iType = GetScreenSurfaceType();
	//para.m_device_type = WIN_DEVICE;
	para.m_device_type = iType;
	
	g_nui_instance.create(para );
    //	g_screen.setClipDraw(TRUE);
	// 开始异步线程加载导航数据；
	boost::thread send_data_thread(&sendUserInfo);
    CreateView();
    g_hWnd = hWnd;

	g_nui_instance.getScreen()->SetRenderFlag(TRUE);
	ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

	//::SetTimer(NULL, NULL, 10, TimerProc);
    return TRUE;
}

void CALLBACK SleepWindow( HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime )
{
    //    g_window[m_old_focus_window_id]->SleepPage();
    KillTimer( hWnd, nTimerid );
}



//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch ( message )
    {
        //case WM_PAINT:
        //	hdc = BeginPaint(hWnd, &ps);
        //	EndPaint(hWnd, &ps);
        //	break;
    case WM_DESTROY:
		//异步渲染，要在此结束渲染线程
        PostQuitMessage( 0 );
        break;
    case WM_KEYDOWN:
        if((wParam == 'B') && (GetKeyState( VK_CONTROL ) < 0))
        {
            // ctrl + B 刷新区域显示
            extern bool g_b_draw_drawing;
            g_b_draw_drawing = !g_b_draw_drawing;
        }

        if ( wParam == 'T' )
        {
            //CTimeBarPage* pView = new CTimeBarPage;
            //pView->Create(RERect::MakeXYWH(0,0 , g_iScreenWidth, g_iScreenHeight));
            //g_screen.AddView( pView );
            //pView->init();

            //g_window->SetShow(FALSE);
            //pView->InvalidateView();
        }
        if ( wParam == 'R' )
        {

        }
        if ( wParam == VK_RIGHT )
        {
        }

        if ( wParam == VK_LEFT )
        {
        }

        break;
    default:
        break;
    }

    g_nui_instance.NUIWndProc( hWnd, message, wParam, lParam );

    return DefWindowProc( hWnd, message, wParam, lParam );
}


#endif // WIN32

 
