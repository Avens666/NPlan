//  File:        NPlanAppView.h
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
#ifndef _NPALANAPPVIEW_H_
#define _NPALANAPPVIEW_H_
#include "KViewGroup.h"
#include "NGridView.h"
#include "NProjectPage.h"
#include "NMainView.h"
#include "KSurfaceManager.h"
#include "KButtonView.h"

//Nplan主View
class NPlanAppView : public KViewGroup
{
public:
	NPlanAppView(void);
	~NPlanAppView(void);
	//初始化控件
	void Init();
	
	//标题栏按下
	void OnBtnTitleDown(kn_int x,kn_int y , KMessageMouse* msg);
	//标题栏弹起
	void OnBtnTitleUp(kn_int x,kn_int y , KMessageMouse* msg);
	//标题栏移动
	void OnBtnTitleMove(kn_int x,kn_int y , KMessageMouse* msg);
	void OnBtnTitleMoveIn();
	void OnBtnTitleMoveOut();

	void OnBtnTitleDClick(kn_int x,kn_int y , KMessageMouse* msg);

	void onMinClick(KView_PTR);
	void onFullScreenClick(KView_PTR);
	void onMidScreenClick(KView_PTR);
	void onCloseClick(KView_PTR);
	void refreshSizeBtn();

	void enterProjectView( NProjectView_PTR p_v );
	void enterMainView();
	virtual kn_bool OnUserMsg(KMessage* pMsg);
	void showWelcomeImages();

private:

	//大背景(1024*700)
	NMainView_PTR m_main_view;
	CProjectPage_PTR m_project_page;
	//标题栏(1024*100)
	KImgButtonView_PTR m_close_btn;
	KTextView_PTR m_img_title_bar;

	NWelcomeImagePage_PTR m_welcome_images;

	KImgButtonView_PTR m_max_size_btn;
	KImgButtonView_PTR m_normal_size_btn;
	KImgButtonView_PTR m_min_size_btn;

	kn_int m_iMoveWidth;
	kn_int m_iMoveHeight;
	kn_bool m_bMoveWindow;
	kn_bool m_b_fullscreen;

	CAnimationThread m_ani_thread;
};
typedef boost::shared_ptr<NPlanAppView> NPlanAppView_PTR;

#endif _NMAINVIEW_H_

