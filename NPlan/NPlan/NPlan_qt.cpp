//  File:        NPlan_qt.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/10/29
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 

#ifdef QT_HMI

#include "NPlan.h"
#include "FilePath.h"
#include "KScreen.h"
#include "mainforwindows.h"

#include <QLabel>

extern int g_iScreenWidth; // 屏幕宽度
extern int g_iScreenHeight; // 屏幕高度

extern CNUIInstance  g_nui_instance;

// 设置资源路径
void SetResourcePath()
{

	kn_char szResPath[] = _T("");		// 当前目录
	SetCurrentPath(szResPath);

}

void CreateQtSoftDeviceSurface(QLabel* pLabel)
{
	KNUIInitPara para;
	para.m_w = g_iScreenWidth;
	para.m_h = g_iScreenHeight;
	para.m_wnd = pLabel;

	// QPixmap: It is not safe to use pixmaps outside the GUI thread
	//	Qt 无法在非GUI线程上屏
	para.m_render_mode = SCREEN_RENDER_MAIN_THREAD;
	//para.m_render_mode = SCREEN_RENDER_MAIN_THREAD;

	para.m_device_type = QT_SOFT_DEVICE;
	para.m_format = SkBitmap::kARGB_8888_Config;
	g_nui_instance.create(para);

	//	g_screen.setClipDraw(TRUE);

	//g_hWnd = hWnd;

	CreateView();

	g_nui_instance.getScreen()->SetRenderFlag(TRUE);
}


void SendQtMessage(int hWnd, int message, int wParam, int lParam)
{
	g_nui_instance.NUIWndProc( hWnd, message, wParam, lParam );
}


#endif
