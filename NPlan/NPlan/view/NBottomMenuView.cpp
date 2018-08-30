//  File:        NBottomMenuView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/3
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NBottomMenuView.h"
#include "../manager/KStringManager.h"
#include "NProjectPage.h"

#define MENU_SHOW TRUE //初始菜单显示状态 
NBottomMenuView::NBottomMenuView(void)
{
	
}


NBottomMenuView::~NBottomMenuView(void)
{
}

void NBottomMenuView::init()
{
	KImageDrawable_PTR btnsmallbg  = KImageDrawable_PTR(new KImageDrawable(	getSurfaceManager()->GetSurface(_T("./resource/bottom_small.png")),true));
	m_bottom_small = KSwitchButtonView_PTR(new KSwitchButtonView);
	m_bottom_small->Create((m_rect.width() - 60)/2, m_rect.height() - 24, 60, 24);
	m_bottom_small->setBKG(btnsmallbg, btnsmallbg, btnsmallbg, btnsmallbg);
	//菜单初始为显示状态所以switch按钮应该显示关闭icon，提示点击可以关闭菜单
	m_bottom_small->setResourse(_T("./resource/up_icon.png"), _T("./resource/down_icon.png"),!MENU_SHOW);
	AddView(m_bottom_small);
	createTextViewHelper(&m_bottom_big,_T("./resource/bottom_big.png"), (m_rect.width() - 414)/2, m_rect.height()-50);
	m_bottom_big->enableMessage(FALSE);
	m_bottom_big->SetScale(0.1,1, m_bottom_big->GetRect().width()/2, m_bottom_big->GetRect().height() );

	int step = 40;
	m_btn_pos[0].set(step/2,24);
	m_btn_pos[1].set(step/2+(m_rect.width() - step)/5,10);
	m_btn_pos[2].set(step/2+(m_rect.width() - step)*2/5,10);
	m_btn_pos[3].set(step/2+(m_rect.width() - step)*3/5,10);
	m_btn_pos[4].set(step/2+(m_rect.width() - step)*4/5,24);

	//打开项目
	createImgViewHelper(&m_btn[0],_T("./resource/prj_edit_n.png"),_T("./resource/prj_edit_a.png"),_T("./resource/prj_edit_a.png"),			m_btn_pos[3].x(), m_btn_pos[3].y() );
	createImgViewHelper(&m_btn[1],_T("./resource/timebar_n.png"),_T("./resource/timebar_a.png"),_T("./resource/timebar_a.png"),		m_btn_pos[0].x(), m_btn_pos[0].y() );
	createImgViewHelper(&m_btn[2],_T("./resource/story_task_n.png"),_T("./resource/story_task_a.png"),_T("./resource/story_task_a.png"),	m_btn_pos[1].x(), m_btn_pos[1].y() );
	createImgViewHelper(&m_btn[3],_T("./resource/story_man_n.png"),_T("./resource/story_man_a.png"),_T("./resource/story_man_a.png"),				m_btn_pos[2].x(), m_btn_pos[2].y() );
	createImgViewHelper(&m_btn[4],_T("./resource/calendar_n.png"),_T("./resource/calendar_a.png"),_T("./resource/calendar_a.png"),			m_btn_pos[3].x(), m_btn_pos[3].y() );

	//绑定方法
	m_btn[0]->m_clicked_signal.connect(this,&NBottomMenuView::OnBtnPrjectEdit);
	m_btn[1]->m_clicked_signal.connect(this,&NBottomMenuView::OnBtnTimeBar);
	m_btn[2]->m_clicked_signal.connect(this,&NBottomMenuView::OnBtnTask);
	m_btn[3]->m_clicked_signal.connect(this,&NBottomMenuView::OnBtnMan);
	m_btn[4]->m_clicked_signal.connect(this,&NBottomMenuView::OnBtnCalendar);
	m_btn[0]->setTip(getStringManager()->GetStringByID(_T("project_edit")), NO_TIMER, 4000);
	m_btn[1]->setTip(getStringManager()->GetStringByID(_T("timeline_bar")), NO_TIMER, 4000);
	m_btn[2]->setTip(getStringManager()->GetStringByID(_T("task_board")), NO_TIMER, 4000);
	m_btn[3]->setTip(getStringManager()->GetStringByID(_T("resource_board")), NO_TIMER, 4000);
	m_btn[4]->setTip(getStringManager()->GetStringByID(_T("project_calendar")), NO_TIMER, 4000);

	m_btn[0]->checkAlpha(TRUE);
	m_btn[1]->checkAlpha(TRUE);
	m_btn[2]->checkAlpha(TRUE);
	m_btn[3]->checkAlpha(TRUE);
	m_btn[4]->checkAlpha(TRUE);

	m_btn[0]->SetShow(FALSE);
	m_btn[1]->SetShow(FALSE);
	m_btn[2]->SetShow(FALSE);
	m_btn[3]->SetShow(FALSE);
	m_btn[4]->SetShow(FALSE);
	m_bottom_big->SetShow(FALSE);

	m_bottom_small->m_mouse_enter_signal.connect(this, &NBottomMenuView::enterBottom);
	m_bottom_small->m_mouse_leave_signal.connect(this, &NBottomMenuView::leaveBottom);
	//m_bottom_small->m_clicked_signal.connect(this,&NBottomMenuView::OnBtnSmall);
	m_bottom_small->m_sign_state_changed.connect(this,&NBottomMenuView::OnChangeBtnSmall);

	leaveBottom();
}

void NBottomMenuView::enterBottom()
{
	m_bottom_small->setOpacity(255);
}

void NBottomMenuView::leaveBottom()
{
	m_bottom_small->setOpacity(50);

}

void NBottomMenuView::OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
//	pMsg->setIdle( KMSG_RETURN_CONTINUE );
}

void NBottomMenuView::OnBtnMan(KView_PTR V)
{
	CProjectPage_PTR proj_view = VIEW_PTR(CProjectPage)(m_p_parent.lock());
	proj_view->switchPage(STORYBOARD_MAN_PAGE);

}

void NBottomMenuView::OnBtnTask(KView_PTR V)
{
	CProjectPage_PTR proj_view = VIEW_PTR(CProjectPage)(m_p_parent.lock());
	proj_view->switchPage(STORYBOARD_PAGE);

}

void NBottomMenuView::OnBtnTimeBar(KView_PTR V)
{
	CProjectPage_PTR proj_view = VIEW_PTR(CProjectPage)(m_p_parent.lock());
	proj_view->switchPage(TIMEBAR_PAGE);
}

void NBottomMenuView::OnBtnCalendar(KView_PTR V)
{
	CProjectPage_PTR proj_view = VIEW_PTR(CProjectPage)(m_p_parent.lock());
	proj_view->switchPage(CALENDAR_PAGE);
	
}

void NBottomMenuView::OnBtnPrjectEdit(KView_PTR V)
{
	CProjectPage_PTR proj_view = VIEW_PTR(CProjectPage)(m_p_parent.lock());
	proj_view->switchPage(PROJECT_EDIT_PAGE);

}

void NBottomMenuView::OnChangeBtnSmall(kn_bool isOFF)
{
	m_bottom_small->enableMessage(false);
	//状态要跟switch按钮icon显示相反
	if (!isOFF == MENU_SHOW)
	{
		showAni();	
	}
	else
	{
		hideAni();
	}

}
void NBottomMenuView::initAnimation()
{
	m_bottom_small->enableMessage(false);
	showAni(600);
}

void NBottomMenuView::setSwitchButton(kn_bool b)
{
	m_bottom_small->setStatus(b, TRUE);
}

void NBottomMenuView::showAni(int waitTime)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	m_btn[0]->SetShow(TRUE);
	m_btn[1]->SetShow(TRUE);
	m_btn[2]->SetShow(TRUE);
	m_btn[3]->SetShow(TRUE);
	m_btn[4]->SetShow(TRUE);
	m_bottom_big->SetShow(TRUE);

	m_btn[0]->setOpacity(0);
	m_btn[1]->setOpacity(0);
	m_btn[2]->setOpacity(0);
	m_btn[3]->setOpacity(0);
	m_btn[4]->setOpacity(0);
	m_bottom_big->setOpacity(0);

	m_btn[0]->SetPosition( m_btn_pos[0].x(),70 );
	m_btn[1]->SetPosition( m_btn_pos[1].x(),70 );
	m_btn[2]->SetPosition( m_btn_pos[2].x(),70 );
	m_btn[3]->SetPosition( m_btn_pos[3].x(),70 );
	m_btn[4]->SetPosition( m_btn_pos[4].x(),70 );
//	m_bottom_big->SetPosition( (m_rect.width() - 414)/2, m_rect.height());
	m_ani_thread.addAnimationHelper(m_btn[0], PropertyPARA_ID_Opacity,		255, 500, waitTime + 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[1], PropertyPARA_ID_Opacity,		255, 400, waitTime + 100, 0);
	m_ani_thread.addAnimationHelper(m_btn[2],	PropertyPARA_ID_Opacity,	255, 350, waitTime + 150, 0);
	m_ani_thread.addAnimationHelper(m_btn[3],	PropertyPARA_ID_Opacity,	255, 300, waitTime + 200, 0);
	m_ani_thread.addAnimationHelper(m_btn[4],	PropertyPARA_ID_Opacity,	255, 300, waitTime + 200, 0);
	m_ani_thread.addAnimationHelper(m_bottom_big, PropertyPARA_ID_Opacity,	255, 500, waitTime + 0, 0);

	//m_ani_thread.addAnimationHelper(m_btn[0], PropertyPARA_ID_POS_X, m_btn_pos[0].x(), 250, 0, 0);
	//m_ani_thread.addAnimationHelper(m_btn[1], PropertyPARA_ID_POS_X, m_btn_pos[1].x(), 300, 0, 0);
	//m_ani_thread.addAnimationHelper(m_btn[2], PropertyPARA_ID_POS_X, m_btn_pos[2].x(), 400, 0, 0);
	//m_ani_thread.addAnimationHelper(m_btn[3], PropertyPARA_ID_POS_X, m_btn_pos[3].x(), 500, 0, 0);

	m_ani_thread.addAnimationHelper(m_btn[0], PropertyPARA_ID_POS_Y, 80, m_btn_pos[0].y(), 350, waitTime + 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[1], PropertyPARA_ID_POS_Y, 80, m_btn_pos[1].y(), 400, waitTime + 50, 0);
	m_ani_thread.addAnimationHelper(m_btn[2], PropertyPARA_ID_POS_Y, 80, m_btn_pos[2].y(), 500, waitTime + 50, 0);
	m_ani_thread.addAnimationHelper(m_btn[3], PropertyPARA_ID_POS_Y, 80, m_btn_pos[3].y(), 600, waitTime + 50, 0);
	m_ani_thread.addAnimationHelper(m_btn[4], PropertyPARA_ID_POS_Y, 80, m_btn_pos[4].y(), 600, waitTime + 50, 0);

	m_ani_thread.addAnimationHelper(m_bottom_big, PropertyPARA_ID_ScaleX, 1, waitTime + 500, 0, 0);
//	m_ani_thread.addAnimationHelper(m_bottom_big, PropertyPARA_ID_ScaleY, 1, 500, 0, 0);
	m_ani_thread.setStopMsg(BOTTOM_MENU_SHOWANI_END );
	m_ani_thread.setFrameTime(40);
	m_ani_thread.Start();
}

void NBottomMenuView::hideAni()
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}

	m_ani_thread.addAnimationHelper(m_btn[0], PropertyPARA_ID_Opacity, 0, 900, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[1], PropertyPARA_ID_Opacity, 0, 750, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[2], PropertyPARA_ID_Opacity, 0, 500, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[3], PropertyPARA_ID_Opacity, 0, 400, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[4], PropertyPARA_ID_Opacity, 0, 400, 0, 0);

	//m_ani_thread.addAnimationHelper(m_btn[0], PropertyPARA_ID_POS_X, 170, 500, 0, 0);
	//m_ani_thread.addAnimationHelper(m_btn[1], PropertyPARA_ID_POS_X, 170, 400, 100, 0);
	//m_ani_thread.addAnimationHelper(m_btn[2], PropertyPARA_ID_POS_X, 170, 350, 150, 0);
	//m_ani_thread.addAnimationHelper(m_btn[3], PropertyPARA_ID_POS_X, 170, 300, 200, 0);

	m_ani_thread.addAnimationHelper(m_btn[0], PropertyPARA_ID_POS_Y, 80,900, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[1], PropertyPARA_ID_POS_Y, 80,750, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[2], PropertyPARA_ID_POS_Y, 80,500, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[3], PropertyPARA_ID_POS_Y, 80,400, 0, 0);
	m_ani_thread.addAnimationHelper(m_btn[4], PropertyPARA_ID_POS_Y, 80,400, 0, 0);

	m_ani_thread.addAnimationHelper(m_bottom_big, PropertyPARA_ID_Opacity, 0, 500, 0, 0);
	m_ani_thread.addAnimationHelper(m_bottom_big, PropertyPARA_ID_ScaleX, 0.1, 500, 0, 0);
//	m_ani_thread.addAnimationHelper(m_bottom_big, PropertyPARA_ID_ScaleY, 0.1, 500, 0, 0);
	m_ani_thread.setStopMsg(BOTTOM_MENU_HIDEANI_END );

	m_ani_thread.Start();
}

kn_bool  NBottomMenuView::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == BOTTOM_MENU_HIDEANI_END)
	{
		m_btn[0]->SetShow(FALSE);
		m_btn[1]->SetShow(FALSE);
		m_btn[2]->SetShow(FALSE);
		m_btn[3]->SetShow(FALSE);
		m_bottom_big->SetShow(FALSE);
		m_bottom_small->enableMessage(true);
		return TRUE;
	}
	else if (pMsg->m_msg_type == BOTTOM_MENU_SHOWANI_END)
	{

		m_bottom_small->enableMessage(true);
		return TRUE;
	}

	return KViewGroup::OnUserMsg( pMsg);
}
