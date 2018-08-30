//  File:        NPlanAppView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2017/5/7
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NPlanAppView.h"
#include "NProjectView.h"
#include "NProjectPage.h"
#include "../manager/KProjectManager.h"
#include "../manager/KConfigManager.h"
#include "KScreen.h"
#include "KSlideView.h"
#ifdef WIN32
//句柄
extern HWND g_hWnd; 
#endif 

extern	int g_iScreenWidth; // 屏幕宽度
extern	int g_iScreenHeight;  // 屏幕高度
//宏


NPlanAppView::NPlanAppView(void)
{
	m_bMoveWindow = false;
	m_b_fullscreen = FALSE;

}


NPlanAppView::~NPlanAppView(void)
{

}

//初始化控件
void NPlanAppView::Init()
{
	//大背景(1024*700)

	m_main_view = NMainView_PTR(new NMainView);
	m_main_view->Create(0,0, m_rect.width(), m_rect.height());
	m_main_view->init();
	AddView(m_main_view);

	m_img_title_bar = KTextView_PTR(new KTextView);
	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/move_window_bk.9.png")),true));
	normal->setOpacity(50);
	m_img_title_bar->setBKDrawable(normal);
	KImageDrawable_PTR icon  = KImageDrawable_PTR(
		new KImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/move_window_icon.png")),true));
	m_img_title_bar->setIconDrawable(icon);
	m_img_title_bar->Create(RERect::MakeXYWH( (m_rect.width() - 340)/2, 0, 340, 19) );
	m_img_title_bar->setIconCenter();
	
//	m_img_title_b111ar->setBKGColor( ColorSetARGB(2,250,250,250)  );

	//m_img_title_bar->SetOpacity(10);
	AddView(m_img_title_bar);	
	createImgViewHelper(&m_close_btn,_T("./resource/close_n.png"),
									 _T("./resource/close_a.png"),
									 _T("./resource/close_f.png"), m_rect.width() - 30, 15);
	m_close_btn->setTip(getStringManager()->GetStringByID(_T("exit")),NO_TIMER,4000);
	createImgViewHelper(&m_max_size_btn,_T("./resource/sizemax_n.png"),
		_T("./resource/sizemax_a.png"),
		_T("./resource/sizemax_f.png"), m_rect.width() - 60, 15);
	m_max_size_btn->setTip(getStringManager()->GetStringByID(_T("max")),NO_TIMER,4000);
	createImgViewHelper(&m_normal_size_btn,_T("./resource/sizemid_n.png"),
		_T("./resource/sizemid_a.png"),
		_T("./resource/sizemid_f.png"), m_rect.width() - 60, 15);
	m_normal_size_btn->setTip(getStringManager()->GetStringByID(_T("restore")),NO_TIMER,4000);

	createImgViewHelper(&m_min_size_btn,_T("./resource/sizemin_n.png"),
		_T("./resource/sizemin_a.png"),
		_T("./resource/sizemin_f.png"), m_rect.width() - 90, 15);
	m_min_size_btn->setTip(getStringManager()->GetStringByID(_T("min")),NO_TIMER,4000);


	m_img_title_bar->m_sign_down.connect(this,&NPlanAppView::OnBtnTitleDown);
	m_img_title_bar->m_sign_up.connect(this,&NPlanAppView::OnBtnTitleUp);
	m_img_title_bar->m_sign_move.connect(this,&NPlanAppView::OnBtnTitleMove);
	m_img_title_bar->m_mouse_enter_signal.connect(this,&NPlanAppView::OnBtnTitleMoveIn);
	m_img_title_bar->m_mouse_leave_signal.connect(this,&NPlanAppView::OnBtnTitleMoveOut);
	m_img_title_bar->m_sign_dclick.connect(this,&NPlanAppView::OnBtnTitleDClick);

	m_close_btn->m_clicked_signal.connect(this, &NPlanAppView::onCloseClick);
	m_max_size_btn->m_clicked_signal.connect(this, &NPlanAppView::onFullScreenClick);
	m_normal_size_btn->m_clicked_signal.connect(this, &NPlanAppView::onMidScreenClick);
	m_min_size_btn->m_clicked_signal.connect(this, &NPlanAppView::onMinClick);

	m_main_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_img_title_bar->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_close_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_max_size_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_normal_size_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_min_size_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	refreshSizeBtn();
	showWelcomeImages();
}

void NPlanAppView::showWelcomeImages()
{
	kn_string strRunFirst = getConfigManager()->getConfig(_T("runfirst"));
	kn_string strImageNum = getConfigManager()->getConfig(_T("first_start_image_mumber"));

	if(strRunFirst.empty() || strImageNum.empty())
	{
		return;
	}

	if ((strRunFirst == _T("0")) || (strImageNum==_T("0")) || (strRunFirst == _T("")) ||  (strImageNum==_T("")) )
	{
		return;
	}
	kn_int iImageNum = boost::lexical_cast<kn_int>(strImageNum);
	m_welcome_images = NWelcomeImagePage_PTR(new NWelcomeImagePage());
	m_welcome_images->Create(0, 60, m_rect.width(), m_rect.height() - 60);
	kn_string keyValue = _T("WelcomeImage");
	m_welcome_images->Init(keyValue, iImageNum);
	AddView(m_welcome_images);
	m_welcome_images->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_max_size_btn->SetShow(false);
	m_min_size_btn->SetShow(false);
	
}

void NPlanAppView::OnBtnTitleDown(kn_int x,kn_int y , KMessageMouse* msg)
{
#ifdef WIN32
	tagRECT rect;
	POINT point;
	GetWindowRect(g_hWnd,&rect);
	GetCursorPos(&point);
	m_iMoveWidth = point.x - rect.left;
	m_iMoveHeight = point.y - rect.top;
	m_bMoveWindow = TRUE;
	m_b_mouse_in_view = TRUE;

#endif
}
void NPlanAppView::OnBtnTitleUp(kn_int x,kn_int y , KMessageMouse* msg)
{
#ifdef WIN32
	m_bMoveWindow = FALSE;
#endif
}

void NPlanAppView::OnBtnTitleDClick(kn_int x,kn_int y , KMessageMouse* msg)
{
#ifdef WIN32
	if (m_b_fullscreen)
	{
		onMidScreenClick(msg->m_p_view);
	}
	else
	{
		onFullScreenClick(msg->m_p_view);
	}

#endif
}

void NPlanAppView::OnBtnTitleMove( kn_int x,kn_int y , KMessageMouse* msg )
{
#ifdef WIN32
	if (m_bMoveWindow)
	{
		POINT point;
		GetCursorPos(&point);
		::SetWindowPos(
			g_hWnd,
			HWND_TOP,
			point.x - m_iMoveWidth, 
			point.y - m_iMoveHeight, 
			0,
			0,
			SWP_NOZORDER|SWP_NOSIZE);
	}

#endif
}

void NPlanAppView::OnBtnTitleMoveIn(  )
{
	m_img_title_bar->getBKDrawable()->setOpacity(150);
	InvalidateView();
}

void NPlanAppView::OnBtnTitleMoveOut( )
{
	m_img_title_bar->getBKDrawable()->setOpacity(50);
	InvalidateView();
}

void  NPlanAppView::enterMainView()
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}

	m_ani_thread.addAnimationHelper(m_main_view, PropertyPARA_ID_POS_Y, 0, 700, 0, 0 );
	m_ani_thread.addAnimationHelper(m_project_page, PropertyPARA_ID_POS_Y, m_rect.height(), 700, 0, 0 );
	m_ani_thread.setStopMsg(PROJECTPAGE_OUT);
	m_main_view->SetShow(TRUE);
	m_ani_thread.Start();
}


void NPlanAppView::enterProjectView( NProjectView_PTR p_v )
{
	if (p_v)
	{
		if (m_ani_thread.isPlaying())
		{
			return;
		}

		m_project_page = CProjectPage_PTR(new CProjectPage);
		m_project_page->Create(0,m_rect.height() , m_rect.width(), m_rect.height() );
		m_project_page->init(p_v->getProject());

		AddView( m_project_page );
		changeViewLayerBottom(m_project_page);

		m_ani_thread.addAnimationHelper(m_main_view, PropertyPARA_ID_POS_Y, -m_rect.height(), 900, 0, 0 );
		m_ani_thread.addAnimationHelper(m_project_page, PropertyPARA_ID_POS_Y, 0, 900, 0, 0 );
		m_ani_thread.setStopMsg(PROJECTPAGE_IN);

		m_project_page->InvalidateView();
		m_project_page->initAnimation();

		m_ani_thread.Start();
	}
}

kn_bool  NPlanAppView::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == PROJECTPAGE_IN)
	{
		m_main_view->SetShow(FALSE);
		return TRUE;
	}

	if (pMsg->m_msg_type == PROJECTPAGE_OUT)
	{
		AddViewToDel(m_project_page);
		m_project_page.reset();
		GetScreen()->sendNUIMessage( PROJECTPAGE_DATA_UPDATE );
		return TRUE;
	}

	if (pMsg->m_msg_type == MSG_WELCOME_ANI_OVER)
	{
		m_max_size_btn->SetShow(true);
		m_min_size_btn->SetShow(true);
		AddViewToDel(m_welcome_images);
		m_welcome_images.reset();
		return TRUE;
	}

	return FALSE;
}

void NPlanAppView::onCloseClick(KView_PTR v)
{
	if (m_project_page)
	{
		if (!m_project_page->confirmModifyView())
		{
			return;
		}
	}
#ifdef WIN32
	GetScreen()->sendSystemMessage(WM_CLOSE, NULL,NULL);
#endif

#ifdef QT_HMI
    void EndLoop(int nExitCode);
    EndLoop(0);
#endif
}
void NPlanAppView::onMinClick(KView_PTR)
{
#ifdef WIN32
	::ShowWindow(GetScreen()->getWnd(), SW_SHOWMINIMIZED);
#endif
}
void NPlanAppView::onFullScreenClick(KView_PTR)
{
#if WIN32

	//多屏显示器支持
	HMONITOR hMonitor;
	MONITORINFO mi;

	hMonitor = MonitorFromWindow(GetScreen()->getWnd(), MONITOR_DEFAULTTONEAREST);
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	//int w = mi.rcMonitor.right - mi.rcMonitor.left;
	//int h = mi.rcMonitor.bottom - mi.rcMonitor.top;
	int w = mi.rcWork.right - mi.rcWork.left;
	int h = mi.rcWork.bottom - mi.rcWork.top;
	
	::SetWindowPos(GetScreen()->getWnd(),HWND_TOP, mi.rcMonitor.left,  mi.rcMonitor.top, w, h,SWP_NOZORDER |SWP_NOREDRAW );
	GetScreen()->resize(w , h);
	g_iScreenWidth = w;
	g_iScreenHeight = h;
	m_b_fullscreen = true;
	refreshSizeBtn();
	m_max_size_btn->hideTip();
#endif
}
void NPlanAppView::onMidScreenClick(KView_PTR)
{
#if WIN32
	//多屏显示器支持
	HMONITOR hMonitor;
	MONITORINFO mi;

	hMonitor = MonitorFromWindow(GetScreen()->getWnd(), MONITOR_DEFAULTTONEAREST);
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	int w = mi.rcMonitor.right - mi.rcMonitor.left;
	int h = mi.rcMonitor.bottom - mi.rcMonitor.top;
	::SetWindowPos(GetScreen()->getWnd(),HWND_TOP, mi.rcMonitor.left + (w-1024)/2, mi.rcMonitor.top + (h-700)/2, 1024, 700,SWP_NOZORDER);
	GetScreen()->resize(1024, 700);
	g_iScreenWidth = 1024;
	g_iScreenHeight = 700;
	m_b_fullscreen = false;
	refreshSizeBtn();
#endif
}
void NPlanAppView::refreshSizeBtn()
{
	if (m_b_fullscreen)
	{
		m_max_size_btn->SetShow(false);
		m_normal_size_btn->SetShow(true);
	}
	else
	{
		m_max_size_btn->SetShow(true);
		m_normal_size_btn->SetShow(false);
	}
}
