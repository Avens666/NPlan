//  File:        NBottomMenuView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/2/28
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef _NBottomMenuVIEW_H_
#define _NBottomMenuVIEW_H_

#include "KViewGroup.h"
#include "KButtonView.h"
#include "KSwitchButtonView.h"
#include "KTextView.h"
#include "KSurfaceManager.h"

#define BottomMenuView_BTN_COUNT 5
class NBottomMenuView : public KViewGroup
{
public:
	NBottomMenuView(void);
	~NBottomMenuView(void);

	//初始化控件
	void init();
	//封装创建的一些方法

	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);

	//按钮回调方法
	void OnBtnMan(KView_PTR);

	void OnBtnTask(KView_PTR);

	void OnBtnTimeBar(KView_PTR);

	void OnBtnCalendar(KView_PTR);

	void OnBtnPrjectEdit(KView_PTR);

	void OnChangeBtnSmall(kn_bool isOFF);

	void enterBottom();
	void leaveBottom();
	void setSwitchButton(kn_bool);

	void showAni(int waitTime = 0);
	void hideAni();
	//进入界面菜单的初始动画
	void initAnimation();
	virtual kn_bool OnUserMsg(KMessage* pMsg);
protected:

	//底部栏
	//KImgButtonView* m_bottom_small;
	KSwitchButtonView_PTR m_bottom_small;
	KTextView_PTR m_bottom_big;

	//关于
	KImgButtonView_PTR m_btn[BottomMenuView_BTN_COUNT];
	REPoint m_btn_pos[BottomMenuView_BTN_COUNT];
	CAnimationThread m_ani_thread; 

};

typedef boost::shared_ptr<NBottomMenuView> NBottomMenuView_PTR;

#endif

