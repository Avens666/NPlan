//  File:        NPlan_ios.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/2/27
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
  

#ifdef IOS_DEV

#include "FilePath.h"
#include "SkCGUtils.h"
#include "KKoteiMessage.h"
#include "NPlan.h"
#include "KScreen.h"
#include "mainforwindows.h"

//美工资源默认高度
extern int g_iScreenWidth; // 屏幕宽度
extern int g_iScreenHeight;  // 屏幕高度

extern CNUIInstance  g_nui_instance;
// 设置资源路径
void SetResourcePath()
{
 

}

int RunModalEventLoop(bool& bExit)
{
    return 0;
}


int CreateIOSDeviceSurface(CGImageRef& cgimage, int width, int height)
{
    
    //   SetCurrentPath(_T("/Users/apple/Library/Application Support/iPhone Simulator/7.1/Applications/699F281F-E8D2-4408-9478-41E81C2B1207/Documents/bin/"));
    
    KNUIInitPara para;
    para.m_w = width;
    para.m_h = height;
    para.m_wnd = cgimage;
    
    g_iScreenWidth = width;
	g_iScreenHeight = height;
	
    
    //para.m_render_mode = SCREEN_RENDER_MULTI_THREAD;
    para.m_render_mode = SCREEN_RENDER_MAIN_THREAD;
    
    para.m_device_type = IOS_DEVICE;
    para.m_format = SkBitmap::kARGB_8888_Config;
    //para.m_format = SkBitmap::kRGB_565_Config;
    
    g_nui_instance.create(para);
    
    cgimage = (CGImageRef)para.m_wnd;
    
    //	g_screen.setClipDraw(TRUE);
    
    //g_hWnd = hWnd;
    
    CreateView();
    
    g_nui_instance.getScreen()->SetRenderFlag(TRUE);
    
    return 0;
}


void IOSResize(void* dst, int w, int h)
{
	ResizeWindow(dst, w, h);
}


 

void SendIOSMessage(int message, int wParam, int lParam)
{
	SendOSMessage(message, wParam, lParam);
 
}
#endif

