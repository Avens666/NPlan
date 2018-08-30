//  File:        NDragStaffView.cpp
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
#include "NDragStaffView.h"



void NDragStaffView::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		SetPosition( x - m_rect.width() / 2, y - m_rect.height() / 2 );
		InvalidateView();

		KMessageDrag* msg = new KMessageDrag;
		msg->m_p_drag_view = shared_from_this();

		int s_x, s_y;
		GetScreenXY(s_x, s_y);
		msg->m_pos_x = s_x + m_rect.width()/2;
		msg->m_pos_y = s_y + m_rect.height()/2;
		msg->m_drag_type = m_i_drag_msg;
		GetScreen()->sendNUIMessage( KMSG_DRAG, msg );

	}

}
void NDragStaffView::OnDown(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if (!m_p_parent.expired())
	{

		//请问为什么down的时候是相对坐标. move的时候是绝对坐标?
		//没办法  转化一次
		KViewGroup_PTR pParent = pMsg->m_p_view->GetParent();
		kn_int ileft = x,itop = y;
		while (pParent)
		{
			ileft += pParent->GetRect().left();
			itop += pParent->GetRect().top();
			pParent = pParent->GetParent();
		}

		m_p_parent.lock()->GetParent()->changeViewLayerTop(m_p_parent.lock());
		SetPosition( ileft - m_rect.width() / 2,itop - m_rect.height() / 2 );
		m_b_mouse_picked = true;
		setViewFocus();

		SetShow(true);
	}
}

void NDragStaffView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		KMessageDrag* msg = new KMessageDrag;
		msg->m_p_drag_view = shared_from_this();

		int s_x, s_y;
		GetScreenXY(s_x, s_y);
		msg->m_drag_type = m_i_drag_up_msg;
		msg->m_pos_x = s_x + m_rect.width()/2;
		msg->m_pos_y = s_y + m_rect.height()/2;
		GetScreen()->sendNUIMessage( KMSG_DRAG_UP, msg );
	}

	SetShow(false);
}

NDragStaffView::NDragStaffView( const kn_string& strName,res_type type ,kn_int id ,kn_bool b /*= true*/ )
{
	OnInit(strName,type,id,b);
	m_i_drag_msg	 = TASK_GROUP_DRAG;
	m_i_drag_up_msg  = TASK_GROUP_DRAG_UP;

}
void NDragStaffView::setDragMsg(kn_int drag_msg, kn_int drag_up_msg)
{
	m_i_drag_msg	= drag_msg;
	m_i_drag_up_msg = drag_up_msg;
}
