//  File:        TaskView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/16
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "TimeBarView.h"
#include "boost/lexical_cast.hpp"
#include "AnimationThread.h"
#include "KSurfaceManager.h"
#include "KFontManager.h"
#include "TaskView.h"
#include "KTextMultiLineDrawable.h"
#include "boost/lexical_cast.hpp"
#include "MilestoneEditView.h"
#include "NMenu.h"
#include "KScreen.h"
#include "../NPlan.h"
using namespace std;
using namespace boost;
const static kn_int click_area_value = 10;

#ifdef WIN32
//句柄
extern HWND g_hWnd; 
#endif 

CTaskView::CTaskView(void)
{
	m_p_task = NULL;
	m_b_click = true;
	m_b_move = true;
	SetShieldMsg(false);
}


CTaskView::~CTaskView()
{
}

void  CTaskView::setTaskBar(CTaskBarView_WEAK_PTR p)
{
	m_p_taskbar = p;
}

CTaskBarView_PTR  CTaskView::getTaskBar()
{
	return m_p_taskbar.lock();
}

void CTaskView::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if (!m_p_parent.expired())
	{
		m_down_point.set(iScreenX, iScreenY);
		m_down_point_offset.set(iScreenX - m_rect.left(), iScreenY - m_rect.top());
		m_b_mouse_picked = true;
		m_b_click = true;
		setViewFocus();
	}

}

void CTaskView::OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked && m_b_move)
	{
		kn_int move_x_offset = iScreenX - m_down_point.x();
		kn_int move_y_offset = iScreenY -m_down_point.y();
		if ((abs(move_x_offset) > click_area_value) || (abs(move_y_offset) > click_area_value))
		{
			m_b_click = false;
			m_p_parent.lock()->changeViewLayerTop(shared_from_this());
			KMessageDrag* msg = new KMessageDrag;
			msg->m_p_drag_view = shared_from_this();
			msg->m_drag_type = TASK_VIEW_DRAG;
			msg->m_pos_x = iScreenX - m_down_point_offset.x();
			msg->m_pos_y = iScreenY - m_down_point_offset.y();	
			if (move_y_offset > 0)
			{
				msg->m_wParam = DRAG_DOWNWARD;
			}
			else
			{
				msg->m_wParam = DRAG_UPWARD;
			}
			msg->m_lParam = move_y_offset;
			GetScreen()->sendNUIMessage( KMSG_DRAG, msg );
			m_down_point.set(iScreenX, iScreenY);
		}
	}
}

void CTaskView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		if (m_b_click)
		{
			m_task_click_sign.emit(shared_from_this());
		}
		else
		{
			kn_int m_move_lengthY = iScreenY - m_down_point.y();
			KMessageDrag* msg = new KMessageDrag;
			msg->m_p_drag_view = shared_from_this();
			msg->m_drag_type = TASK_VIEW_DRAG_UP;
			msg->m_pos_x = iScreenX;
			msg->m_pos_y = iScreenY;
			msg->m_lParam = m_move_lengthY;
			GetScreen()->sendNUIMessage( KMSG_DRAG_UP, msg );
		}
		
	}
}


CNProjectTask* CTaskView::GetTask()
{
	return m_p_task;
}


void CTaskView::init(CNProjectTask* task)
{
	m_p_task = task;

	//////背景
	kn_string bk_file =  _T("./resource/0"); 

	int color = m_p_task->getColorType() ;
	if (color >= 1 && color <=9)
	{
		bk_file += boost::lexical_cast<kn_string>( color);
	}
	else
	{//不合法自动分配
		kn_int color_type = (task->getVectorId() + 1)%9 + 1;
		bk_file += boost::lexical_cast<kn_string>(color_type);
		m_p_task->setColorType(color_type);
	}

	bk_file += _T(".9.png");

	K9PatchImageDrawable_PTR bk_drawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( bk_file), TRUE ));
//	((K9PatchImageDrawable_PTR)bk_drawable)->SizeToImage();
	bk_drawable->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()) );
	setIconDrawable(bk_drawable);

	/////icon
	//kn_string icon_file =  _T("./resource/task_icon"); 
	//icon_file += boost::lexical_cast<kn_string>( (task->getVectorId() + 1)%8 +1);
	//icon_file += _T(".png");
	//KImageDrawable_PTR icon_da (new KImageDrawable( getSurfaceManager()->GetSurface( icon_file), TRUE  ) );
	//icon_da->SizeToImage();
	//icon_da->SetRect(RERect::MakeXYWH(6, 6, icon_da->GetRect().width(), icon_da->GetRect().height()) );
	//setIconDrawable(icon_da);

	m_text_name = KTextMultiLineDrawable_PTR(new KTextMultiLineDrawable(RE_ColorWHITE, 16, REPaint::kLeft_Align) );
//	m_text_name->SetRect(RERect::MakeXYWH(10 +icon_da->GetRect().width() , 10,  m_rect.width() - 14 - icon_da->GetRect().width(), m_rect.height() ) );
//	m_text_name->setBold(TRUE);
	m_text_name->SetRect(RERect::MakeXYWH(36 , 12,  m_rect.width() - 55, m_rect.height() - 32 ) );
	m_text_name->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_text_name->setMaxLine(3);
	setTextDrawable(m_text_name);
	
//	SetTextColor(SkColorSetARGBMacro(255,0,0,0), SkColorSetARGBMacro(255, 255, 255, 255), SkColorSetARGBMacro(255,0,0, 0), SkColorSetARGBMacro(255,0,0,0));

	m_text_id = KTextDrawable_PTR(new KTextDrawable(_T(""), RE_ColorWHITE, 16,  REPaint::kCenter_Align) );
	//m_text_id->setBold(TRUE);
	m_text_id->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	m_text_id->SetRect(RERect::MakeXYWH(5 , 14,  20, 20 ) );

	addDrawable(m_text_id);

	//REPoint p1, p2;
	//p1.set( )
	//KLineShape* shape = new KLineShape(m_rect.width(), )
	

	refreshInfo();

}


void CTaskView::setMode(bool b)
{
	//if (m_mode == b)
	//{
	//	return;
	//}

	if (b == MODE_BIG)
	{//大模式
		m_text_name->SetFontSize(16);
		m_text_name->setMaxLine(3);
	}
	else
	{
		m_text_name->SetFontSize(12);
		m_text_name->setMaxLine(2);
	}

	if (m_text_name->isOverroad())
	{
		setTip(m_text_name->GetText(),NO_TIMER,4000);
	}
	else
	{
		enableTip(FALSE);
	}
}



void CTaskView::refreshInfo()
{
	if (m_p_task)
	{
		kn_string str = m_p_task->getName();
		m_text_name->SetText(str.c_str() );
		m_text_name->autoMLine();

		if (m_text_name->isOverroad())
		{
			setTip(m_text_name->GetText(),NO_TIMER,4000);
		}
		else
		{
			enableTip(FALSE);
		}
		if (m_p_task->getVectorId() < 9)
		{
			str = _T("0");
			str +=   boost::lexical_cast<kn_string>(m_p_task->getVectorId() + 1); 
		}
		else
		{
			str =   boost::lexical_cast<kn_string>(m_p_task->getVectorId() + 1); 
		}
		m_text_id->SetText(str.c_str() );
	}

}

void CTaskView::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//if ( m_p_task)
	//{
	//	CMenuColor_PTR menu = CMenuColor_PTR(new CMenuColor );
	//	menu->Create(0,0, 38*3+12, 38*4+12);
	//	menu->init(this );
	//	menu->m_sign_menu_click.connect(this, &CTaskView::onMenu);
	//	//showmenu 传入view自身的相对坐标，这里取鼠标点击位置
	//	showMenu(menu, iScreenX - m_rect.x(), iScreenY - m_rect.y());
	//	InvalidateView();	
	//}

}

void CTaskView::onMenu(kn_int n)
{
	//if (n>=1 && n <=9)
	//{//换颜色
	//	kn_string bk_file =  _T("./resource/0"); 
	//	bk_file += boost::lexical_cast<kn_string>( n );
	//	bk_file += _T(".9.png");

	//	K9PatchImageDrawable_PTR bk_drawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( bk_file), TRUE ));
	//	//	((K9PatchImageDrawable_PTR)bk_drawable)->SizeToImage();
	//	bk_drawable->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()) );
	//	setIconDrawable(bk_drawable);
	//}
}

kn_int CTaskView::getTaskID()
{
	return m_p_task->getVectorId();
}

void CTaskView::setNameText( const kn_string& name_text )
{
	m_text_name->SetText(name_text);
}

REColor CTaskView::getBkColor()
{
	kn_int color_type = m_p_task->getColorType();
	switch (color_type)
	{
	case 1:
		m_bk_color = ColorSetARGB(255, 63, 177, 225);
		break;
	case 2:
		m_bk_color = ColorSetARGB(255, 104, 167, 155);
		break;
	case 3:
		m_bk_color = ColorSetARGB(255, 201, 107, 107);
		break;
	case 4:
		m_bk_color = ColorSetARGB(255, 203, 144, 68);
		break;
	case 5:
		m_bk_color = ColorSetARGB(255, 95, 121, 184);
		break;
	case 6:
		m_bk_color = ColorSetARGB(255, 63, 141, 163);
		break;
	case 7:
		m_bk_color = ColorSetARGB(255, 177, 116, 85);
		break;
	case 8:
		m_bk_color = ColorSetARGB(255, 115, 163, 107);
		break;
	case 9:
		m_bk_color = ColorSetARGB(255, 174, 128, 187);
		break;
	default:
		m_bk_color = ColorSetARGB(0, 0, 0, 0);
		break;
	}
	return m_bk_color;
}

void CTaskView::setBMove( kn_bool bMove )
{
	m_b_move = bMove;
}

kn_string CTaskView::getNameText()
{
	return m_text_name->GetText();
}

/////////////////////   CEventView   ////////////////////////////

CEventView::CEventView(void)
{
	m_p_data = NULL;
	m_b_move = FALSE;
	m_w = 25;
	checkAlpha(TRUE);
}


CEventView::~CEventView()
{
}


CNProjectMileStone* CEventView::getMileStone()
{
	return m_p_data;
}


void CEventView::init(CNProjectMileStone* data, CTimeBarView_PTR timebar)
{
	//////背景
	//K9PatchImageDrawable_PTR bk_drawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/task_btn_bk.9.png")), TRUE ));
	//((K9PatchImageDrawable_PTR)bk_drawable)->SizeToImage();
	//bk_drawable->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()) );
	//setBKDrawable(bk_drawable);

	/////icon
	//kn_string icon_file =  _T("./resource/task_icon"); 
	//icon_file += boost::lexical_cast<kn_string>( (task->getVectorId() + 1)%8 +1);
	//icon_file += _T(".png");
	K9PatchImageDrawable_PTR icon_da (new K9PatchImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/event.9.png")), TRUE  ) );
	icon_da->SizeToImage();
	RERect rect = icon_da->GetRect();
	m_w = rect.width();
	icon_da->SetRect(RERect::MakeXYWH(0, 0, rect.width(), m_rect.height()) );
	setBKDrawable(icon_da);
	checkAlpha(TRUE);
	setCheckAlphaDrawable(icon_da);

	m_info = KTextMultiLineDrawable_PTR(new KTextMultiLineDrawable( RE_ColorBLACK, 12, REPaint::kLeft_Align) );
//	int line = m_info->getLine();
	m_info->SetRect(RERect::MakeXYWH(rect.width()/2+3 + m_w/2, 15, 100, 14*2 ) );
	m_info->setTextFrame(TRUE);
	setTextDrawable(m_info);

	m_time = KTextDrawable_PTR(new KTextDrawable(_T(""), RE_ColorBLACK, 12,  REPaint::kLeft_Align) );
	m_time->SetRect(RERect::MakeXYWH(rect.width()/2+3 + m_w/2, 1,  100, 14 ) );
	m_time->setTextFrame(TRUE);
	addDrawable(m_time);

	
	m_event_id = KTextDrawable_PTR(new KTextDrawable(_T(""), ARGB(255,255,255,255), 12,  REPaint::kLeft_Align) );
	m_event_id->SetRect(RERect::MakeXYWH(10, 10,  16, 12 ) );
	m_event_id->setBold(TRUE);
	m_event_id->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
//	m_event_id->setTextFrame(TRUE);
	addDrawable(m_event_id);

	m_p_data = data;
	m_timebar = timebar;
	refreshInfo();
//	enableMessage(TRUE);
}

void CEventView::refreshInfo()
{
	if (m_p_data)
	{
		writeLock lock(m_lst_drawable_mutex);

		kn_string str = m_p_data->getName();
		m_info->SetText(str);
	//	m_info->autoMLine();
		str = getTimeString(m_p_data->getTime(), FALSE);
		m_time->SetText(str);
		str = boost::lexical_cast<kn_string>(m_p_data->getId());
		m_event_id->SetText(str);
	}

}

void CEventView::syncTimeline(CTimeBarView_PTR tl)
{
	if (m_p_data)
	{
		float x1;
		x1 = tl->getTimePosition( m_p_data->getTime() );

		RERect rect = tl->GetRect();
		RERect rect2 = RERect::MakeXYWH(rect.left()+x1 - m_w/2, m_rect.top(), m_rect.width(), m_rect.height());
		SetRect( rect2 );
	}

}

void CEventView::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{
		kn_bool b_update = FALSE;
	if(m_b_mouse_picked)
	{
		if(m_b_move )
		{
			m_p_data->setTime( m_timebar->getPositisonTimeInt( x - m_timebar->GetRect().left() + m_mouse_x_offset, 10 ) );
			b_update = TRUE;
		}
	}

	if (b_update)
	{
		refreshInfo();
		syncTimeline(m_timebar);
		InvalidateView();
		pMsg->setIdle(KMSG_RETURN_DILE);	
	}
}

void CEventView::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if ( iScreenX - m_rect.left() <m_w && iScreenY - m_rect.top() < m_w)
	{//拖移
		m_b_move = TRUE;
		//m_mouse_x_offset 在于使bar和鼠标相对位置不动
		m_mouse_x_offset =  m_rect.left() + m_w/2 - iScreenX;
		pMsg->setIdle(KMSG_RETURN_DILE);	
	}

}

void CEventView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	m_b_move = FALSE;
}
void CEventView::OnDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//tagRECT rect;
	//GetWindowRect(g_hWnd,&rect);
	//RERect rect;
	int w = GetScreen()->GetWidth();
	int h = GetScreen()->GetHeight();
	MilestoneEditView_PTR pView = MilestoneEditView_PTR( new MilestoneEditView);
	pView->Create(RERect::MakeXYWH((w-400)/2, (h-250)/2 , 400, 250));
	pView->init(m_p_data );
	GetParent()->AddView( pView );
	InvalidateView();
	kn_int i_result = pView->doModal();
	if (i_result == KN_REUSLT_OK)
	{
		refreshInfo();
	}
	else if (i_result == KN_REUSLT_USER_DEL)
	{
		m_sign_btn_del.emit( shared_from_this() );
	}
}
