//  File:        NBottomView.cpp
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
#include "NBottomView.h"
#include "../manager/KStringManager.h"
#include "NMainView.h"

#define	TEXT_POS_X			(0)					//文字X坐标
#define	TEXT_POS_Y			(73)				//文字Y坐标
//#define	MENUBTN_POS_X		(60)				//第一个按钮的X坐标
#define	MENUBTN_POS_Y		(10)				//第一个按钮的Y坐标


NBottomView::NBottomView(void)
{
	m_surfaceManager = NULL;
}


NBottomView::~NBottomView(void)
{
}

void NBottomView::init()
{

	int btn_count = 6; //现在是6个按钮
	int MENUBTN_INTERVAL = 165;
	int btn_w = 70;

	if (m_rect.width() / btn_count < MENUBTN_INTERVAL)
	{
		MENUBTN_INTERVAL = m_rect.width() / btn_count-10;
	}
	int MENUBTN_POS_X =  (m_rect.width() - MENUBTN_INTERVAL*(btn_count-1) - btn_w)/2;

	//底部栏

	m_surfaceManager = getSurfaceManager();
	KStringManager* s_stringManager = getStringManager();
	m_img_bottom_bar = KTextView_PTR(new KTextView);
	KImageDrawable_PTR drawable = KImageDrawable_PTR(new KImageDrawable(m_surfaceManager->GetSurface(_T("./resource/bottom_bar.png")),true));

    RERect rect = RERect::MakeXYWH(0,0 ,m_rect.width(),  m_rect.height());
    m_img_bottom_bar->Create(rect);
	drawable->setRect(0,0 ,m_rect.width(),  m_rect.height());
	m_img_bottom_bar->setBKDrawable(drawable);
	AddView(m_img_bottom_bar);

	int i = 0;
	//新建项目
	createImgViewHelper(&m_btn_create,_T("./resource/createproject_n.png"),
								_T("./resource/createproject_a.png"),
								_T("./resource/createproject_f.png"),MENUBTN_POS_X+i*MENUBTN_INTERVAL,MENUBTN_POS_Y);
	SetViewText(m_btn_create,s_stringManager->GetStringByID(_T("main_new")),TEXT_POS_X,TEXT_POS_Y);
	i++;
	//打开项目
	createImgViewHelper(&m_btn_open,_T("./resource/openproject_n.png"),
							  _T("./resource/openproject_a.png"),
		                      _T("./resource/openproject_f.png"),MENUBTN_POS_X+i*MENUBTN_INTERVAL,MENUBTN_POS_Y);
	SetViewText(m_btn_open,s_stringManager->GetStringByID(_T("main_open")),TEXT_POS_X,TEXT_POS_Y);

	i++;
	//公共资源管理
	createImgViewHelper(&m_btn_public,_T("./resource/public_n.png"),
								_T("./resource/public_a.png"),
								_T("./resource/public_f.png"),MENUBTN_POS_X+i*MENUBTN_INTERVAL,MENUBTN_POS_Y);
	SetViewText(m_btn_public,s_stringManager->GetStringByID(_T("main_public")),TEXT_POS_X,TEXT_POS_Y);

	i++;
	//设置按钮
	createImgViewHelper(&m_btn_setting,_T("./resource/setting_n.png")
								,_T("./resource/setting_a.png")
								,_T("./resource/setting_f.png"),MENUBTN_POS_X+i*MENUBTN_INTERVAL,MENUBTN_POS_Y);
	SetViewText(m_btn_setting,s_stringManager->GetStringByID(_T("main_setting")),TEXT_POS_X,TEXT_POS_Y);
	i++;
	//使用说明
	createImgViewHelper(&m_btn_help,_T("./resource/help_n.png"),
							  _T("./resource/help_a.png"),
							  _T("./resource/help_f.png"),MENUBTN_POS_X+i*MENUBTN_INTERVAL,MENUBTN_POS_Y);
	SetViewText(m_btn_help,s_stringManager->GetStringByID(_T("main_help")),TEXT_POS_X,TEXT_POS_Y);

	i++;
	//关于
	createImgViewHelper(&m_btn_about,_T("./resource/about_n.png"),
						      _T("./resource/about_a.png"),
		                      _T("./resource/about_f.png"),MENUBTN_POS_X+i*MENUBTN_INTERVAL,MENUBTN_POS_Y);
	SetViewText(m_btn_about,s_stringManager->GetStringByID(_T("main_about")),TEXT_POS_X,TEXT_POS_Y);


	//绑定方法
	m_btn_create->m_clicked_signal.connect(this,&NBottomView::OnBtnCreate);
	m_btn_open->m_clicked_signal.connect(this,&NBottomView::OnBtnOpen);
	m_btn_public->m_clicked_signal.connect(this,&NBottomView::OnBtnPublic);
	m_btn_setting->m_clicked_signal.connect(this,&NBottomView::OnBtnSetting);
	m_btn_help->m_clicked_signal.connect(this,&NBottomView::OnBtnHelp);
	m_btn_about->m_clicked_signal.connect(this,&NBottomView::OnBtnAbout);

	m_img_bottom_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_btn_create->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_btn_open->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_btn_public->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_btn_setting->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_btn_help->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	m_btn_about->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
}



void NBottomView::SetViewText( KImgButtonView_PTR view,const kn_string& str, kn_int x /* = 0*/,kn_int y /*= 0*/ )
{
		//设置文字
		view->SetText(str.c_str());
		//设置文字
        view->setTextAlign(REPaint::kCenter_Align);
		//设置坐标
			RERect viewRect = view->GetRect();
		if (x == 0)
		{
			x = (view->GetRect().width() - view->GetTextWidth())/2;
		}
		if (y == 0)
		{
			y = (view->GetRect().height() - view->GetFontSize())/2;
		}

		view->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE);
		view->SetTextBound(RERect::MakeXYWH(x, y, view->GetTextWidth(), view->GetFontSize() + 2));
}

void NBottomView::OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
//	pMsg->setIdle( KMSG_RETURN_CONTINUE );
}

void NBottomView::OnBtnCreate(KView_PTR V)
{
	( VIEW_PTR(NMainView)(m_p_parent.lock()))->newProject();
}

void NBottomView::OnBtnOpen(KView_PTR V)
{
	( VIEW_PTR(NMainView)(m_p_parent.lock()))->openProject();
}

void NBottomView::OnBtnPublic(KView_PTR V)
{
	( VIEW_PTR(NMainView)(m_p_parent.lock()))->sourceManage();
}

void NBottomView::OnBtnSetting(KView_PTR V)
{
	(VIEW_PTR(NMainView)(m_p_parent.lock()))->setting();
}

void NBottomView::OnBtnHelp(KView_PTR V)
{
	(VIEW_PTR(NMainView)(m_p_parent.lock()))->help();
}

void NBottomView::OnBtnAbout(KView_PTR V)
{
	( VIEW_PTR(NMainView)(m_p_parent.lock()))->about();
}

void NBottomView::GetBottomRect(RERect& rect)
{
	if (m_img_bottom_bar)
	{
		rect = m_img_bottom_bar->GetRect();
	}
}
