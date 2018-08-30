//  File:        TimeBarView.cpp
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
#include "TimeBarView.h"
#include "boost/lexical_cast.hpp"
#include "AnimationThread.h"
using namespace std;
using namespace boost;

CTimeBarView::CTimeBarView(void)
{

}


CTimeBarView::~CTimeBarView()
{


}

void CTimeBarView::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		// 设置区域
		setTimeLineOffset(m_timeline_offset + x - m_mouse_x);
		//CPropertyPos* p = new CPropertyPos(this, x - m_rect.width() / 2, y - m_rect.height() / 2);
		//GetScreen()->addProperty(p);
		UpdateUI();
	}
}


void CTimeBarView::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	m_timeline_offset = getTimeLineOffset();
	m_mouse_x = iScreenX;
}

void CTimeBarView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//CNUIAnimation* pNUIAnimation = new CNUIAnimation (1000, 0, 0); //iDaleyTime * i);
	//
	//CPropertyTimeLineOffset* pPosDst = new CPropertyTimeLineOffset(this, m_timeline->getTimeLineOffset() + 500);
	//CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPosDst);
	//pNUIAnimation->addAnimationData(pAnimationData);
	//ani_thread.addAnimation(pNUIAnimation);
	//ani_thread.Start();
}

void CTimeBarView::init()
{
	m_e_viewtype = (KVIEWTYPE)KVIEW_TIMEBAR;
	m_timeline_height = 100;
	m_timeline = CTimeLineDrawable_PTR(new CTimeLineDrawable() );
	m_timeline->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_timeline_height) );
	m_timeline->setTimeArea(RERect::MakeXYWH(0, m_timeline_height, m_rect.width(), m_rect.height() - m_timeline_height )  );
	m_timeline->setHourType(12);
	addDrawable(m_timeline);
	m_timeline->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top,KVIEW_BTMethod_Parent_Top);
	//m_text_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorBLACK, 16, REPaint::kLeft_Align) );
	////	m_text_drawable->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	//m_text_drawable->SetRect(RERect::MakeXYWH(0, 5, m_timeline_height, 50) );
	//addDrawable(m_text_drawable);
}

void CTimeBarView::SetRect(const RERect& rect)
{
	KView::SetRect(rect);
	m_timeline->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_timeline_height) );
	m_timeline->setTimeArea(RERect::MakeXYWH(0, m_timeline_height, m_rect.width(), m_rect.height() - m_timeline_height )  );

}

void CTimeBarView::OnWheel(KMessageMouseWheel* pMsg)
{
	if (pMsg->m_pos_y > m_timeline_height)
	{
		pMsg->setIdle(KMSG_RETURN_CONTINUE);
		return;
	}

	int w = getTimelineOneDayWidth();
	if( pMsg->m_roll > 0)
	{
		w *=1.05;
		//setTimelineOneDayWidth(w, pMsg->m_pos_x - m_rect.left());
		setTimelineOneDayWidth(w,true);

	}
	else
	{
		w /=1.05;
		setTimelineOneDayWidth(w,true);
	}
	
//	m_text_drawable->SetText(lexical_cast<kn_string>(w));
	InvalidateView();
}

//设置时间轴比例长度像素尺寸
void  CTimeBarView::setTimelineOneDayWidth(int v,kn_bool b)
{
	writeLock lock(m_lst_drawable_mutex);

	m_timeline->setTimelineOneDayWidth(v);
	m_timeline_change_signal.emit(b);
}

int  CTimeBarView::getTimelineOneDayWidth()
{
	return m_timeline->getTimelineOneDayWidth();
}

//获得时间轴相对标准点的偏移，像素单位
int  CTimeBarView::getTimeLineOffset()
{
	return m_timeline->getTimeLineOffset();
}

//设置时间轴相对标准点的偏移，像素单位
void  CTimeBarView::setTimeLineOffset(int v)
{
	m_timeline->setTimeLineOffset(v);
	m_timeline_change_signal.emit(false);

}

//根据时间返回时间轴像素坐标
float CTimeBarView::getTimePosition(const ptime &p)
{
	if (p.is_not_a_date_time() )
	{
		return 0;
	}
	float x = m_timeline->getTimePosition(p);

	return x;
}

ptime CTimeBarView::getPositisonTime(int x)
{
	return  m_timeline->getPositisonTime( x - m_timeline->GetRect().left() );
}

ptime CTimeBarView::getPositisonTimeInt(int x, int step_hour)
{
	return  m_timeline->getPositisonTimeInt( x - m_timeline->GetRect().left(), step_hour );
}

//根据时间段 相对于显示宽度尺寸 返回显示比例, 
int CTimeBarView::getTimeDurationWidth(ptime &start, ptime &end)
{
	return  m_timeline->getTimeDurationWidth(start, end);
}

RERect CTimeBarView::getTimelineArea()
{
	return RERect::MakeXYWH(m_rect.left(), m_rect.top()+m_timeline_height, m_rect.width(), m_rect.height() - m_timeline_height);
}


kn_bool  CTimeBarView::isChange(kn_int para_id, kn_double v)
{
	if (para_id == PARA_ID_TimeBar)
	{
		kn_int i_v = v;
		return ( getTimeLineOffset() != i_v);
	}

	if (para_id == PARA_ID_TimeBar_Scale)
	{
		kn_int i_v = v;
		return (getTimelineOneDayWidth() != i_v);
	}
	return KView::isChange(para_id, v);
}

void  CTimeBarView::doChange(kn_int para_id, kn_double v)
{
	KView::doChange(para_id, v);
	if (para_id == PARA_ID_TimeBar)
	{
		kn_int i_v = v;
		setTimeLineOffset(i_v);
	}
	if (para_id == PARA_ID_TimeBar_Scale)
	{
		kn_int i_v = v;
		setTimelineOneDayWidth(i_v);
	}
}

kn_double  CTimeBarView::getParaValue(kn_int para_id)
{
	if (para_id == PARA_ID_TimeBar)
	{
		return getTimeLineOffset();
	}

	if (para_id == PARA_ID_TimeBar_Scale)
	{
		return getTimelineOneDayWidth();
	}
	return KView::getParaValue(para_id);
}

void CTimeBarView::setCalendar(CNProjectCalendar* p)
{
	m_timeline->setCalendar(p);
}
