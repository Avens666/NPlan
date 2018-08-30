//  File:        TaskBarView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/2/25
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "TimeBarView.h"
#include "boost/lexical_cast.hpp"
#include "AnimationThread.h"
#include "TaskBarView.h"
#include "KFontManager.h"
#include "KSurfaceManager.h"
#include "NTimeBarPage.h"
using namespace std;
using namespace boost;

CTaskBarView::CTaskBarView(void)
{
	m_e_viewtype = (KVIEWTYPE)KVIEW_TIMEBAR;
	m_move_type = MOVE_NORMAL;
	m_p_task = NULL;
	m_mouse_x_offset = 0;
//	m_timeline_offset = 0;
	m_icon_alpha = 0;
	m_icon_show = FALSE;
	m_action_show = FALSE;
	m_mode = MODE_BIG;
	m_bar_h = 36;
}

void CTaskBarView::shared_ptr_inited()
{
	KTextView::shared_ptr_inited();
	m_sign_unactive.connect(this, &CTaskBarView::taskbarUnActive);
}

CTaskBarView::~CTaskBarView()
{


}

bool CTaskBarView::isActionShow()
{
	return m_action_show;
}

void CTaskBarView::setActionShow(bool b)
{
	m_action_show = b;
}

void CTaskBarView::onMoveDirect(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	kn_bool b_update = FALSE;
	RERect rect = m_taskbar->GetRect();
	time_duration td = hours(10);
	ptime pt;
	if(m_move_type == MOVE_ADJUST_END)
	{//结束时间，占全天
		pt =  m_timebar->getPositisonTimeInt(x - m_timebar->GetRect().left() + m_mouse_x_offset, 18 );
		if (pt > m_p_task->getStartTime() )
		{
			m_p_task->setEndTime( pt);
			b_update = TRUE;
		}
	}
	else if (m_move_type == MOVE_ADJUST_START)
	{
		pt =  m_timebar->getPositisonTimeInt(x - m_timebar->GetRect().left() + m_mouse_x_offset, 9 );
		if (pt < m_p_task->getEndTime() )
		{
			m_p_task->setStartTime( pt);
			b_update = TRUE;
		}
	}
	else if (m_move_type == MOVE_ADJUST_POS)
	{
		ptime start = m_timebar->getPositisonTimeInt( x - m_timebar->GetRect().left() + m_mouse_x_offset, 0 );
		ptime end = start + (m_p_task->getEndTime() - m_p_task->getStartTime() );
		m_p_task->setStartTime( start );
		m_p_task->setEndTime( end );
		b_update = TRUE;
	}
	else 
	{
			if (rect.contains(x - m_rect.left(),y - m_rect.top() ) )
			{
				m_b_mouse_in_view = TRUE;
				if(!m_icon_show)
				{
					//m_ani_thread.Stop();
					//m_ani_thread.clearAnimation();
					//m_ani_thread.addAnimationHelper(this, PARA_ID_TASKBAR_APLHA, 150, 800, 500, 0);
					m_icon_show = TRUE;
					//m_ani_thread.Start();

					m_rmove_icon->setOpacity(255);
					m_lmove_icon->setOpacity(255);
					m_lrmove_icon->setOpacity(255);

					InvalidateView();
				}
			}
			else 
			{
				if(m_icon_show)
				{
					//m_ani_thread.Stop();
					//m_ani_thread.clearAnimation();
					//m_ani_thread.addAnimationHelper(this, PARA_ID_TASKBAR_APLHA, 0, 500, 0, 0);
					m_icon_show = FALSE;
					//m_ani_thread.Start();

					m_rmove_icon->setOpacity(0);
					m_lmove_icon->setOpacity(0);
					m_lrmove_icon->setOpacity(0);
					InvalidateView();
				}
			}
	}
	if (b_update)
	{
		refreshInfo();
		syncTimeline(m_timebar);
		( VIEW_PTR(CTimeBarPage)(m_p_parent.lock()) )->updateModifyData();
		InvalidateView();
		pMsg->setIdle(KMSG_RETURN_DILE);	
	}
}

void CTaskBarView::taskbarUnActive()
{

}


void CTaskBarView::onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if (iScreenX - m_rect.left() > m_rmove_icon->GetRect().left() 
		&& iScreenY - m_rect.top() > m_taskbar->GetRect().top())
	{//尾部拖移
		m_move_type = MOVE_ADJUST_END;
		//m_mouse_x_offset 在于使bar和鼠标相对位置不动
		m_mouse_x_offset = m_rect.right()- iScreenX;
	}
	else if (iScreenX - m_rect.left() < m_lmove_icon->GetRect().right()
		&& iScreenY - m_rect.top() > m_taskbar->GetRect().top())
	{//头部拖移
		m_move_type = MOVE_ADJUST_START;
		m_mouse_x_offset = m_rect.left() - iScreenX ;

	}else if (iScreenX - m_rect.left() > m_lrmove_icon->GetRect().left()
		&& iScreenX - m_rect.left() < m_lrmove_icon->GetRect().right()
		&& iScreenY - m_rect.top() > m_taskbar->GetRect().top())
	{//整体拖移
		m_move_type = MOVE_ADJUST_POS;
		m_mouse_x_offset = m_rect.left() - iScreenX ;

	}else
	{
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
	}
}

void CTaskBarView::onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	pMsg->setIdle(KMSG_RETURN_CONTINUE);
	m_move_type = MOVE_NORMAL;
}

void  CTaskBarView::setTaskBarBK(K9PatchImageDrawable_PTR da)
{
	writeLock lock(m_lst_drawable_mutex);
	RERect rect = m_taskbar->GetRect();
	m_taskbar = (da);
	m_taskbar->SetRect(rect);
	m_lst_drawable[m_taskbar_index] = da;
	lock.unlock();
//	setBarWidth(m_rect);
}

void CTaskBarView::init(CNProjectTaskCardBase* task, CTimeBarView_PTR timebar)
{
	int head = 5; 
	
	m_pm_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/pm_icon.png")), TRUE ) );
	m_pm_icon->SizeToImage();
	m_pm_icon->SetRect(RERect::MakeXYWH(0, head, m_pm_icon->GetRect().width(), m_pm_icon->GetRect().height()) );
	setIconDrawable(m_pm_icon);	
	m_taskbar_y = m_icon_drawable->GetRect().height()+head+1;

	kn_string icon_file =  _T("./resource/timebar"); 
	int color = ((CNProjectTask*)task)->getColorType() ;
	if (color >= 1 && color <=9)
	{
		icon_file += boost::lexical_cast<kn_string>( color);
	}
	else
	{//不合法自动分配
		icon_file += boost::lexical_cast<kn_string>( ( ((CNProjectTask*)task)->getVectorId() + 1)%9 +1);
	}

	icon_file += _T(".9.png");
	m_taskbar = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( icon_file), TRUE) );
	m_taskbar->SizeToImage();
	m_taskbar->SetRect(RERect::MakeXYWH(0, m_taskbar_y-1, 100, m_taskbar->GetRect().height()) );
	m_bar_h = m_taskbar->GetRect().height();
	m_taskbar_index = addDrawable(m_taskbar);


	m_lmove_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/timebar_lmove_icon.png")), TRUE ) );
	//m_lmove_icon->SizeToImage();
	//m_lmove_icon->SetRect(RERect::MakeXYWH(0, m_taskbar_y, m_lmove_icon->GetRect().width(), m_lmove_icon->GetRect().height()) );
	m_lmove_icon->setRect(0, m_taskbar_y, 22, m_taskbar->GetRect().height() - 5);
	addDrawable(m_lmove_icon);	
	m_lmove_icon->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_lrmove_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/timebar_lrmove_icon.png")), TRUE ) );
	//m_lrmove_icon->SizeToImage();
	//m_lrmove_icon->SetRect(RERect::MakeXYWH(m_lmove_icon->GetRect().width() + 2, m_taskbar_y, m_lrmove_icon->GetRect().width(), m_lrmove_icon->GetRect().height()) );
	m_lrmove_icon->setRect(m_lmove_icon->GetRect().right() + 10, m_taskbar_y, 23, m_taskbar->GetRect().height() - 5);
	addDrawable(m_lrmove_icon);	
	m_lrmove_icon->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_rmove_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/timebar_rmove_icon.png")), TRUE ) );
	//m_rmove_icon->SizeToImage();
	//m_rmove_icon->SetRect(RERect::MakeXYWH( m_rect.width()- m_rmove_icon->GetRect().width(), m_taskbar_y, m_rmove_icon->GetRect().width(), m_rmove_icon->GetRect().height()) );
	m_rmove_icon->setRect(m_rect.width()- 27, m_taskbar_y, 22, m_taskbar->GetRect().height() - 5);
	addDrawable(m_rmove_icon);	
	m_rmove_icon->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_member_txt = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorBLACK, 14, REPaint::kLeft_Align) );
	m_member_txt->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_member_txt->SetRect(RERect::MakeXYWH(m_pm_icon->GetRect().width()+5, head+5, 500, 20) );
	m_member_txt->SetText(task->getName());
	addDrawable(m_member_txt);

	m_time_txt = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorBLACK, 12, REPaint::kLeft_Align) );
	m_time_txt->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_time_txt->SetRect(RERect::MakeXYWH(20, m_icon_drawable->GetRect().height()+head+5, 400, 20) );
	addDrawable(m_time_txt);

	m_p_task = task;
	m_timebar = timebar;

	//m_rmove_icon->SetShow(FALSE);
	//m_lmove_icon->SetShow(FALSE);
	//m_lrmove_icon->SetShow(FALSE);

	m_rmove_icon->setOpacity(m_icon_alpha);
	m_lrmove_icon->setOpacity(m_icon_alpha);
	m_lmove_icon->setOpacity(m_icon_alpha);
	refreshInfo();

}

void CTaskBarView::setMode(bool b)
{
	//if (m_mode == b)
	//{
	//	return;
	//}

	if (b == MODE_BIG)
	{//大模式
		m_pm_icon->SetShow(TRUE);
		m_member_txt->SetRect(RERect::MakeXYWH(m_pm_icon->GetRect().width()+5, 10, 500, 20) );
		m_member_txt->SetFontSize(14);
		m_taskbar->SetRect(RERect::MakeXYWH(0, m_taskbar_y-1, m_taskbar->GetRect().width(), m_bar_h) );
		m_lmove_icon->SetRect(RERect::MakeXYWH(0, m_taskbar_y, m_lmove_icon->GetRect().width(), m_taskbar->GetRect().height() - 5) );
		m_lrmove_icon->SetRect(RERect::MakeXYWH(m_lmove_icon->GetRect().right() + 10, m_taskbar_y, m_lrmove_icon->GetRect().width(), m_taskbar->GetRect().height()- 5));
		m_rmove_icon->SetRect(RERect::MakeXYWH( m_rect.width()- 27, m_taskbar_y, m_rmove_icon->GetRect().width(), m_taskbar->GetRect().height()- 5 ) );
		m_time_txt->SetRect(RERect::MakeXYWH(20, m_icon_drawable->GetRect().height()+10, 400, 20) );

	}
	else
	{
		m_pm_icon->SetShow(FALSE);
		m_member_txt->SetRect(RERect::MakeXYWH( 20, 15, 500, 20) );
		m_member_txt->SetFontSize(12);
		m_taskbar->SetRect(RERect::MakeXYWH(0, 0, m_taskbar->GetRect().width(), m_bar_h - 6 ) );
		m_lmove_icon->SetRect(RERect::MakeXYWH(0, 0, m_lmove_icon->GetRect().width(), m_taskbar->GetRect().height()  - 3));
		m_lrmove_icon->SetRect(RERect::MakeXYWH(m_lmove_icon->GetRect().right() + 10, 0, m_lrmove_icon->GetRect().width(), m_taskbar->GetRect().height()  - 3 ));
		m_rmove_icon->SetRect(RERect::MakeXYWH( m_rect.width()- 27, 0, m_rmove_icon->GetRect().width(), m_taskbar->GetRect().height()  - 3) );
		m_time_txt->SetRect(RERect::MakeXYWH(20, 3, 400, 20) );
	}
}


void CTaskBarView::refreshInfo()
{
	if (m_p_task)
	{
		kn_string str = m_p_task->getLeaderName();
		kn_string mem_str;
		mem_str = ( (CNProjectTask*)m_p_task)->getMembersNameStr(TRUE);
		if (!mem_str.empty())
		{
			str += _T(" (") + mem_str + _T(")");
		}
		
		m_member_txt->SetText(str);

		
		str = getTimeString(m_p_task->getStartTime());
		str += _T(" - ");
		str += getTimeString(m_p_task->getEndTime());

		str += _T("  ") + m_p_task->getName();

		m_time_txt->SetText(str);

	}

}

void CTaskBarView::syncTimeline(CTimeBarView_PTR tl)
{
	if (m_p_task)
	{
		int x1, x2;
		x1 = tl->getTimePosition( m_p_task->getStartTime() );
		x2 = tl->getTimePosition( m_p_task->getEndTime() );
		if (x2 - x1 < 50)
		{
			x2 = x1+50;
		}

		RERect rect = tl->GetRect();
		RERect rect2 = RERect::MakeXYWH(rect.left()+x1, m_rect.top(), x2-x1, m_rect.height());
		setBarWidth( rect2 );
	}

}

void CTaskBarView::setBarWidth(const RERect& rect)
{
	writeLock lock(m_lst_drawable_mutex);

	SetRect(rect);
	RERect rect2 = m_taskbar->GetRect();
	m_taskbar->SetRect(RERect::MakeXYWH(rect2.left(), rect2.top(), rect.width(), rect2.height()) );
	m_rmove_icon->SetRect(RERect::MakeXYWH(m_taskbar->GetRect().width()- m_rmove_icon->GetRect().width() - 5, m_rmove_icon->GetRect().y(), m_rmove_icon->GetRect().width(), m_rmove_icon->GetRect().height()) );
	m_lrmove_icon->SetRect(RERect::MakeXYWH( m_lmove_icon->GetRect().right() + 10, m_lrmove_icon->GetRect().top(), m_lrmove_icon->GetRect().width(), m_taskbar->GetRect().height() - 5) );

}

void CTaskBarView::OnWheel(KMessageMouseWheel* pMsg)
{

	if( pMsg->m_roll > 0)
	{

	}
	else
	{
	}
	
}

CNProjectTask* CTaskBarView::GetTask()
{
	return (CNProjectTask *)m_p_task;
}



kn_bool  CTaskBarView::isChange(kn_int para_id, kn_double v)
{
	if (para_id == PARA_ID_TASKBAR_APLHA)
	{
		kn_int i_v = v;
		return  (m_icon_alpha != i_v) ;
	}
	return KView::isChange(para_id, v);
}

void  CTaskBarView::doChange(kn_int para_id, kn_double v)
{
	KView::doChange(para_id, v);
	if (para_id == PARA_ID_TASKBAR_APLHA)
	{
		kn_int i_v = v;
		m_icon_alpha = (i_v);

		m_rmove_icon->setOpacity(m_icon_alpha);
		m_lmove_icon->setOpacity(m_icon_alpha);
		m_lrmove_icon->setOpacity(m_icon_alpha);

	}
}

kn_double  CTaskBarView::getParaValue(kn_int para_id)
{
	if (para_id == PARA_ID_TASKBAR_APLHA)
	{
		return m_icon_alpha;
	}
	return KView::getParaValue(para_id);
}

void CTaskBarView::setTaskView(CTaskView_WEAK_PTR p)
{
	m_task_view = p;
}

CTaskView_PTR CTaskBarView::getTaskView()
{
	return m_task_view.lock();
}


///////////////// CActionBarView  ////////////////////


CActionBarView::CActionBarView(void)
{

}


CActionBarView::~CActionBarView()
{


}


void CActionBarView::init(CNProjectTaskCardBase* task, CTimeBarView_PTR timebar)
{
	int head = 2; 

	m_taskbar_y = 0;

	kn_string icon_file =  _T("./resource/actionbar"); 
	icon_file += boost::lexical_cast<kn_string>( (task->getLeaderId() + 1)%3 +1);
	icon_file += _T(".9.png");
	m_taskbar = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( icon_file), TRUE) );
	m_taskbar->SizeToImage();
	m_taskbar->SetRect(RERect::MakeXYWH(0, m_taskbar_y, 100, m_taskbar->GetRect().height()) );
	m_taskbar_index = addDrawable(m_taskbar);


	m_lmove_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/actionbar_lmove_icon.png")), TRUE ) );
	m_lmove_icon->SizeToImage();
	m_lmove_icon->SetRect(RERect::MakeXYWH(2, 2, m_lmove_icon->GetRect().width(), m_lmove_icon->GetRect().height()) );
	addDrawable(m_lmove_icon);	

	m_lrmove_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/actionbar_lrmove_icon.png")), TRUE ) );
	m_lrmove_icon->SizeToImage();
	m_lrmove_icon->SetRect(RERect::MakeXYWH(m_lmove_icon->GetRect().right() + 10, 2, m_lrmove_icon->GetRect().width(), m_lrmove_icon->GetRect().height()) );
	addDrawable(m_lrmove_icon);	

	m_rmove_icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/actionbar_rmove_icon.png")), TRUE ) );
	m_rmove_icon->SizeToImage();
	m_rmove_icon->SetRect(RERect::MakeXYWH( m_rect.width()- m_rmove_icon->GetRect().width(), 2, m_rmove_icon->GetRect().width(), m_rmove_icon->GetRect().height()) );
	addDrawable(m_rmove_icon);	

	m_member_txt = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorWHITE, 14, REPaint::kLeft_Align) );
	m_member_txt->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_member_txt->SetFontSize(12);
	m_member_txt->SetRect(RERect::MakeXYWH(12, head, 500, 20) );
	m_member_txt->SetText(task->getName());
	addDrawable(m_member_txt);

	m_time_txt = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorBLACK, 12, REPaint::kLeft_Align) );
	m_time_txt->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_time_txt->SetFontSize(11);
	m_time_txt->setTextFrame(TRUE);
	m_time_txt->SetRect(RERect::MakeXYWH(12,head+13, 300, 20) );
	addDrawable(m_time_txt);

	m_p_task = task;
	m_timebar = timebar;

	//m_rmove_icon->SetShow(FALSE);
	//m_lmove_icon->SetShow(FALSE);
	//m_lrmove_icon->SetShow(FALSE);

	m_rmove_icon->setOpacity(m_icon_alpha);
	m_lrmove_icon->setOpacity(m_icon_alpha);
	m_lmove_icon->setOpacity(m_icon_alpha);
	refreshInfo();

}

void CActionBarView::refreshInfo()
{
	if (m_p_task)
	{
		kn_string str = m_p_task->getLeaderName();
		str += _T("  ") + m_p_task->getName();
		m_member_txt->SetText(str);


		str = getTimeString(m_p_task->getStartTime());
		str += _T(" - ");
		str += getTimeString(m_p_task->getEndTime());
		m_time_txt->SetText(str);

	}

}

void CActionBarView::syncTimeline(CTimeBarView_PTR tl)
{
	if (m_p_task)
	{
		int x1, x2;
		x1 = tl->getTimePosition( m_p_task->getStartTime() );
		x2 = tl->getTimePosition( m_p_task->getEndTime() );
		if (x2 - x1 < 50)
		{
			x2 = x1+50;
		}

		RERect rect = tl->GetRect();
		RERect rect2 = RERect::MakeXYWH(rect.left()+x1, m_rect.top(), x2-x1, m_rect.height());
		setBarWidth( rect2 );
	}

}
