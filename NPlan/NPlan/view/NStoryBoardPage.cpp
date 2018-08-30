//  File:        NStoryBoardPage.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/2/26
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NStoryBoardPage.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "boost/lexical_cast.hpp"
#include "KSurfaceManager.h"
#include "KTextMultiLineDrawable.h"
#include "KScreen.h"
#include "../NPlan.h"
#include "NMenu.h"
#include "NDragStaffView.h"
////////// CActionView //////////////


CActionView::CActionView(void)
{
	m_e_viewtype = (KVIEWTYPE)NPLAN_ACTION_VIEW;
	SetShieldMsg(false);
	
}
CActionView::~CActionView(void)
{

}
CNProjectTask* CActionView::getTask()
{
	if (m_p_action)
	{
		return (CNProjectTask*)(m_p_action->getTask());
	}
	return NULL;
}

int CActionView::getState()
{
	if (m_p_action)
	{
		return m_p_action->getState();
	}
	return -1;
}

void CActionView::setRow(kn_uint row)
{
	m_row = row;
}

kn_uint CActionView::getRow()
{
	return m_row;
}

void CActionView::SetPosition(kn_int iX, kn_int iY)
{
	KView::SetPosition(iX,iY);

}

void CActionView::setData( CNProjectTaskCard* task_card )
{
	m_p_action = task_card;
}

CNProjectTaskCard*  CActionView::getData()
{
	return m_p_action;
}

void CActionView::init(CNProjectTaskCard* p , kn_uint row)
{
	setRow(row);
	m_p_action = p;
	int left_w = 5;

	IRESurface* sur = getSurfaceManager()->GetSurface(getBgColorByID(p->getLeaderId()));
	if (!sur)
	{
		return;
	}
	
	//取得图片主题色
	REColor bk_clr = sur->getColor(sur->GetWidth()/2, 4);

	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( sur, TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);
	

	m_p_name = KTextMultiLineDrawable_PTR(new KTextMultiLineDrawable(SkColorSetARGBMacro(255,255,255,255), 12, REPaint::kLeft_Align) );
	m_p_name->SetRect(RERect::MakeXYWH(left_w, 7,  m_rect.width() - 10, 40 ) );

	m_p_name->SetText(m_p_action->getName().c_str() );
	if (m_p_name->getLine() == 1)
	{//为了美观，根据文字行数进行微调
		m_p_name->SetRect(RERect::MakeXYWH(left_w, 7,  m_rect.width() - 10, 40 ) );
	}
	else
	{
		m_p_name->SetRect(RERect::MakeXYWH(left_w, 7,  m_rect.width() - 10, 40 ) );
	}
	m_p_name->setRowInterval(1);
	setTextDrawable(m_p_name);

	//使用了icon的drawable [1]位置
	if (m_p_name->getLine() > 1)
	{
		KColorDrawable_PTR txt_bk_drawable = KColorDrawable_PTR(new KColorDrawable(bk_clr) );
		int h = min(44, m_p_name->getLine()* 16);
		txt_bk_drawable->setRect(1, 5, m_rect.width()-1, h);
		m_lst_drawable[1] = txt_bk_drawable;
	}
	int txt_bottom_y = 50;
	m_p_member = KTextDrawable_PTR(new  KTextDrawable( m_p_action->getLeaderName().c_str() , SkColorSetARGBMacro(255,0,0,0), 12, REPaint::kLeft_Align));
//	member->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	m_p_member->setRect(left_w,txt_bottom_y, m_rect.width()-24, 20 );
	addDrawable( m_p_member );	

	kn_string str;
	if (m_p_action->getPlanDays() > 0)
	{
		str +=boost::lexical_cast<kn_string>(m_p_action->getPlanDays());
		str += _T("d");
	}
	if (m_p_action->getPlanHours() > 0)
	{
		str += boost::lexical_cast<kn_string>(m_p_action->getPlanHours());
		str += _T("h");
	}

    m_txt_date = KTextDrawable_PTR(new KTextDrawable(str.c_str(), SkColorSetARGBMacro(255, 25,25,25), 12, REPaint::kRight_Align));
	//	member->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	m_txt_date->setRect(left_w,txt_bottom_y, m_rect.width()-2*left_w+3, 20 );
	addDrawable( m_txt_date );

	str =boost::lexical_cast<kn_string>(m_p_action->getScore());
	kn_string str_score = _T("[");
	str_score += str + _T("]");
    m_p_score = KTextDrawable_PTR(new  KTextDrawable(str_score.c_str(), SkColorSetARGBMacro(255, 200,0,0), 12, REPaint::kRight_Align));
	//	member->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	m_p_score->setRect(left_w ,txt_bottom_y - 15, m_rect.width()-2*left_w+3, 20 );
	addDrawable( m_p_score );

	m_p_score->SetShow( m_p_action->isDone() ); 
	
}

void  CActionView::updateData()
{
	if (!m_p_action )
	{
		return;
	}
	kn_string str =boost::lexical_cast<kn_string>(m_p_action->getScore());
	kn_string str_score = _T("[");
	str_score += str + _T("]");
	m_p_score->SetText(str_score );
	m_p_score->SetShow( m_p_action->isDone() ); 
	InvalidateView();
}

void CActionView::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		// 设置区域
	//	CPropertyPos* p = new CPropertyPos(this, x - m_rect.width() / 2, y - m_rect.height() / 2);
	//	GetScreen()->addProperty(p);
		SetPosition( x - m_rect.width() / 2, y - m_rect.height() / 2 );
		
		KMessageDrag* msg = new KMessageDrag;
		msg->m_p_drag_view = shared_from_this();
		
		int s_x, s_y;
		GetScreenXY(s_x, s_y);
		msg->m_pos_x = s_x + m_rect.width()/2;
		msg->m_pos_y = s_y + m_rect.height()/2;
		msg->m_drag_type = STORYPAGE_ACTION_DRAG;
		GetScreen()->sendNUIMessage( KMSG_DRAG, msg );
		UpdateUI();
	}

}
void CActionView::OnDown(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if(m_b_mouse_picked)
	{
		// 设置区域
	//	m_p_parent->changeViewLayerTop(this);
		m_p_parent.lock()->GetParent()->changeViewLayerTop(m_p_parent.lock());
		setViewFocus();
		pMsg->setIdle(KMSG_RETURN_DILE);

	}
}

void CActionView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//	unSetViewFocus();
	if(m_b_mouse_picked)
	{
		KMessageDrag* msg = new KMessageDrag;
		msg->m_p_drag_view = shared_from_this();

		int s_x, s_y;
		GetScreenXY(s_x, s_y);
		msg->m_pos_x = s_x + m_rect.width()/2;
		msg->m_pos_y = s_y + m_rect.height()/2;
		msg->m_drag_type = STORYPAGE_ACTION_DRAG_UP;
		GetScreen()->sendNUIMessage( KMSG_DRAG_UP, msg );


	}
}

void CActionView::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if ( m_p_action->isDone())
	{
		CMenuScore_PTR menu = CMenuScore_PTR(new CMenuScore );
		menu->Create(0,0, 32*6+12, 32*2+12);
		menu->init(shared_from_this(), m_p_action->getScore() );
		menu->m_sign_menu_click.connect(this, &CActionView::onMenu);
		//showmenu 传入view自身的相对坐标，这里取鼠标点击位置
		showMenu(menu, iScreenX - m_rect.x(), iScreenY - m_rect.y());
		InvalidateView();	
	}

}

void CActionView::onMenu(kn_int n)
{
	m_p_action->setScore(n);
	
	updateData();
	
}

kn_string CActionView::getBgColorByID( kn_int lead_id )
{
	kn_string icon_file =  _T("./resource/action_bk_"); 
	//icon_file += boost::lexical_cast<kn_string>( (p->getId() )%5 +1);
	if (lead_id <= 0)
	{
		icon_file +=_T("0");
	}
	else
	{
		icon_file += boost::lexical_cast<kn_string>( max( 1, lead_id%7 +1 ) );
	}
	icon_file += _T(".9.png");
	return icon_file;
}

void CActionView::updateLeaderInfo()
{
	IRESurface* sur = getSurfaceManager()->GetSurface(getBgColorByID(m_p_action->getLeaderId()));
	if (!sur)
	{
		return;
	}

	//取得图片主题色
	REColor bk_clr = sur->getColor(sur->GetWidth()/2, 4);

	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( sur, TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	setBKDrawable(pDrawable);

	//使用了icon的drawable [1]位置
	if (m_p_name->getLine() > 1)
	{
		KColorDrawable_PTR txt_bk_drawable = KColorDrawable_PTR(new KColorDrawable(bk_clr) );
		int h = min(44, m_p_name->getLine()* 16);
		txt_bk_drawable->setRect(1, 5, m_rect.width()-1, h);
		m_lst_drawable[1] = txt_bk_drawable;
	}

	m_p_member->SetText(m_p_action->getLeaderName());
}

/////////////////////////////////////

/////////// CBoardView //////////////////
CBoardView::CBoardView(void)
{

}
CBoardView::~CBoardView(void)
{

}

void CBoardView::setRow(kn_uint row)
{
	m_row = row;
}

kn_uint CBoardView::getRow()
{
	return m_row;
}

void CBoardView::init(CNProjectTask* task, int state,  kn_uint row, kn_bool b_point)
{
	m_row = row;
	m_p_task = task;
	m_state = state;

	m_bk = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/taskboard_wbk.9.png")), TRUE ) );
	m_bk->setRect(0,0, m_rect.width(), m_rect.height() );
	addDrawable(m_bk);

	m_bk_a = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/taskboard_bka.9.png")), TRUE ) );
	m_bk_a->setRect(0,0, m_rect.width(), m_rect.height() );
	addDrawable(m_bk_a);
	m_bk_a->SetShow(FALSE);
	int aw = 98;
	int ah = 68;	
	if (b_point)
	{
		CNProjectTaskCard_Point_LST &lst_action = task->getTaskCardsByMan();

		CNProjectTaskCard_Point_LST::iterator it;

		for ( it = lst_action.begin(); it!= lst_action.end(); it++ )
		{
			if( ((CNProjectTaskCard*)(*it))->getState() == state )
			{
				CActionView_PTR task_bar = CActionView_PTR(new CActionView() );
				task_bar->Create(RERect::MakeXYWH(0, 0 , aw, ah));
				task_bar->init( (*it), m_row );
				AddView(task_bar);
				task_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
			}

		}
	} 
	else
	{
		CNProjectTaskCard_LST &lst_action = task->getTaskCards();

		CNProjectTaskCard_LST::iterator it;

		for ( it = lst_action.begin(); it!= lst_action.end(); it++ )
		{
			if( ((CNProjectTaskCard)(*it)).getState() == state )
			{
				CActionView_PTR task_bar = CActionView_PTR(new CActionView);
				task_bar->Create(RERect::MakeXYWH(0, 0 , aw, ah));
				task_bar->init( &(*it), m_row );
				AddView(task_bar);
				task_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
			}

		}
	}

}

void CBoardView::SetRect(const RERect& rect)
{
	KView::SetRect(rect);

	if (m_bk.get() != NULL)
	{
		m_bk->setRect(0,0,rect.width(), rect.height() );
	}

	if (m_bk_a.get() != NULL)
	{
		m_bk_a->setRect(0,0,rect.width(), rect.height() );
	}

	calcActionPos();
}

void CBoardView::calcActionPos()
{
	RERect rect;
	int n = 0;
	if (m_lst_view.size() == 0)
	{
	}
	else
	{
		int count = m_rect.width() / m_lst_view[0]->GetRect().width();
		if (count == 0)
		{
			return;
		}

		for (kn_int i = 0;i < m_lst_view.size();i ++)
		{

			KView_PTR v = m_lst_view[i];
			if (v->getType() ==  (KVIEWTYPE) NPLAN_ACTION_VIEW )
			{
				rect = v->GetRect();
				int wblank = (m_rect.width() - count*(rect.width()+4)) / 2 + 2;
				int x = n%count * (rect.width()+4)  + wblank;
				int y = n/count * (rect.height()+3) + 4;
				v->SetPosition(x,y);
				( VIEW_PTR(CActionView)(v) )->updateData();
				n++;
			}

		}	
	}


}

//添加
void CBoardView::AddView(KView_PTR v)
{
	KViewGroup::AddView(v);
	int n =m_lst_view.size() - 1;
	RERect  rect = v->GetRect();
	int x = n%2 * (rect.width()) +3;
	int y = n/2 * (rect.height()) + 3;
	v->SetPosition(x,y);
}

void CBoardView::onDragDirect(KMessageDrag* msg)
{
	kn_bool b_in = isPointInView(msg->m_pos_x, msg->m_pos_y );
	if (msg->m_drag_type == STORYPAGE_ACTION_DRAG && b_in )
	{
		//if (m_p_task == ((CActionView*)(msg->m_p_drag_view))->getTask() && m_state != ((CActionView*)(msg->m_p_drag_view))->getState() )
		if (m_row == ( VIEW_PTR(CActionView)(msg->m_p_drag_view))->getRow() && m_state != ( VIEW_PTR(CActionView)(msg->m_p_drag_view))->getState() )
		{
			m_bk_a->SetShow(TRUE);
		}
		
	}
	else if (msg->m_drag_type == STORYPAGE_ACTION_DRAG_UP )
	{
		m_bk_a->SetShow(FALSE);

		if (b_in )
		{
			//if( m_p_task == ((CActionView*)(msg->m_p_drag_view))->getTask() 
			if( m_row == ( VIEW_PTR(CActionView)(msg->m_p_drag_view))->getRow() 
				&& m_state != ( VIEW_PTR(CActionView)(msg->m_p_drag_view))->getState() )
			{//在区域内，状态不等，重新赋值  交换view
				CNProjectTaskCard* p_data = ( VIEW_PTR(CActionView)(msg->m_p_drag_view))->getData();
				if (p_data)
				{
					p_data->setState(m_state);

					msg->m_p_drag_view->GetParent()->eraseView(msg->m_p_drag_view);
					AddView(msg->m_p_drag_view);
					( VIEW_PTR(CStoryBoardPage)(m_p_parent.lock()) )->calcBoardArea();
					InvalidateView();
					GetScreen()->sendNUIMessage( STORYPAGE_DATA_UPDATE );
				}
			}
			else 
			{
				( VIEW_PTR(CStoryBoardPage)(m_p_parent.lock()) )->calcBoardArea();
				InvalidateView();
			}
			msg->setIdle(KMSG_RETURN_DILE);

		}
		else
		{

		}
	}
	else
	{
		m_bk_a->SetShow(FALSE);
	}
}

/////////////////////////////////

void CStoryMsgView::onDragDirect(KMessageDrag* msg)
{
	if (msg->m_drag_type == STORYPAGE_ACTION_DRAG_UP )
	{
		( VIEW_PTR(CStoryBoardPage)(m_p_parent.lock() ) )->calcBoardArea();
		InvalidateView();
		msg->setIdle(KMSG_RETURN_DILE);

	}
}

////////////////////////////


CStoryBoardPage::CStoryBoardPage(void)
{
	m_e_viewtype = KVIEWTYPE(1001);

	

	m_p_prj_data = NULL;
	m_lst_task = NULL;

}

void CStoryBoardPage::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &CStoryBoardPage::onKeyUp);
}


CNProjectData* CStoryBoardPage::getPrjData()
{
	return m_p_prj_data;
}

CStoryBoardPage::~CStoryBoardPage()
{
	m_ani_thread.Stop();

}


void CStoryBoardPage::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{

}


void CStoryBoardPage::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void CStoryBoardPage::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void CStoryBoardPage::moveTaskY(int offset)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}

	REPoint point;

	std::list<CBoardView_PTR>::iterator it;
	RERect rect = RERect::MakeXYWH( m_rect.left(), m_rect.top()+m_title_height, m_rect.width(), m_rect.height()-m_title_height ); 
	
	if (m_lst_board.size() * m_task_height < rect.height()/2 )
	{//元素太少　不用上下移动
		return;
	}

	it = m_lst_board.begin();
	point = (*it)->GetPosition();
	if (point.y() + offset > rect.top())
	{
		offset =  rect.top() - point.y();
	}

	it = m_lst_board.end();
	it--;
	point = (*it)->GetPosition();
	if (point.y() + offset < rect.top() + rect.height()/2 )
	{
		return;
	}


	for ( it = m_lst_board.begin(); it != m_lst_board.end(); it++ )
	{
		point = (*it )->GetPosition();

		CPropertyPos* pPoint = new CPropertyPos((*it), point.x(), point.y()+offset);
		GetScreen()->addProperty(pPoint);
	}

	std::list<KStaticView_PTR>::iterator itt;
	for ( itt = m_task_btn.begin(); itt != m_task_btn.end(); itt++ )
	{
		point = (*itt)->GetPosition();
	//	((CTaskView*)(*itt) )->SetPosition(point.x(), point.y()+offset);

		CPropertyPos* pPoint = new CPropertyPos((*itt), point.x(), point.y()+offset);
		GetScreen()->addProperty(pPoint);
	}	
}

void CStoryBoardPage::onWheelDirect(KMessageMouseWheel* pMsg)
{
	RERect rect = m_rect;
	if (rect.contains(pMsg->m_pos_x, pMsg->m_pos_y ) )
	{
		if( pMsg->m_roll > 0)
		{
			moveTaskY(50);
		}
		else
		{
			moveTaskY(-50);
		}
		InvalidateView();
	}
	pMsg->setIdle(KMSG_RETURN_DILE);
}

void CStoryBoardPage::viewSizeChange()
{
//	if (isShow())
	{
		KViewGroup::viewSizeChange();
		calcBoardArea();
	}

}

void CStoryBoardPage::init(CNProjectData* p)
{
	Release();

	m_p_prj_data = p;
	m_task_height = 160;

	m_task_width = 170;
	m_title_height = 100;

	KStaticView_PTR titlebk_view = KStaticView_PTR(new KStaticView() );
	titlebk_view->Create(0, 0, m_rect.width(), m_title_height );
	titlebk_view->setBKGColor( ColorSetARGB(255,130,198,181) );
	AddView(titlebk_view);
	titlebk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	////左侧背景
	m_left_view = KStaticView_PTR( new KStaticView) ;
	m_left_view->Create(0, m_title_height, m_rect.width(), m_rect.height() - m_title_height);
	m_left_view->setBKGColor( ColorSetARGB(255,234,234,234) );
	m_left_view->showBK(TRUE);
	AddView(m_left_view);
	m_left_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);


	KStaticView_PTR titlebk_txt = KStaticView_PTR( new KStaticView() );
	titlebk_txt->Create(m_rect.width()/2 - 100, 0, 300, 60 );
	titlebk_txt->SetText( _T("Task Board") );
	titlebk_txt->SetFontSize(36);
	titlebk_txt->SetTextColor(  ColorSetARGB(255,255,255,255)  );
	AddView(titlebk_txt);
	titlebk_txt->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//m_msg_view = CStoryMsgView_PTR(new CStoryMsgView() );
	//m_msg_view->Create(0,0, m_rect.width(), m_rect.height());
	//AddView(m_msg_view);
	//m_msg_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	std::vector<CNProjectTaskBoardItem> &items = p->getTaskBoardItems();
	
	m_board_count = items.size();
	
	//故事版 一个块宽度
	int task_width = ( m_rect.width() - m_task_width ) /m_board_count;
	//任务条
	int i = 0;
	int j = 0;
	m_lst_task = m_p_prj_data->getpTasks();
	CNProjectTask_LST::iterator it_task;

	m_action_size.set(100, 70);
	for ( it_task = m_lst_task->begin(); it_task != m_lst_task->end(); it_task++ )
	{
		for (i = 0; i < m_board_count; i++)
		{
			CBoardView_PTR task_bar = CBoardView_PTR( new CBoardView() );
			task_bar->Create(RERect::MakeXYWH(m_task_width + task_width*i, m_title_height + m_task_height*j , task_width-2, m_task_height));
			task_bar->init( &(*it_task), i, j, FALSE);
//			task_bar->SetClip( RERect::MakeXYWH(m_task_width, m_title_height , m_rect.width(), m_rect.height() - m_title_height ) );
			AddView(task_bar);
			task_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
			m_lst_board.push_back(task_bar);
		}
		j++;
	}


	//任务按钮
	i = 0;
	for ( it_task = m_lst_task->begin(); it_task != m_lst_task->end(); it_task++ )
	{
		CTaskView_PTR task_view = CTaskView_PTR(new CTaskView);
		task_view->Create(RERect::MakeXYWH(0, m_title_height+m_task_height*i , m_task_width, 75));
		task_view->init( &(*it_task));
		task_view->SetClip(RERect::MakeXYWH(0, m_title_height , m_task_width, m_rect.height() - m_title_height  ));
		m_task_btn.push_back(task_view);
		AddView(task_view);
		task_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		i++;
	}


	//标题背景
	//KStaticView* bk_view = new KStaticView();
	//bk_view->Create(0, 0, m_rect.width(), m_title_height);
	//bk_view->setBKGColor(ColorSetARGB(255,50,50,50) );
	//AddView(bk_view);


	///故事版标题
	std::vector<CNProjectTaskBoardItem>::iterator it;
	int title_h  = 36;
	int title_w = 3;
	i = 0;
	//处理屏幕缩放，增加一个层级
	KViewGroup_PTR  m_title_group = KViewGroup_PTR(new KViewGroup);
	m_title_group->Create(m_task_width, m_title_height - title_h, m_rect.width() - m_task_width, title_h);
	AddView(m_title_group);
	m_title_group->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	for ( it = items.begin(); it != items.end(); it++ )
	{
		KStaticView_PTR view = KStaticView_PTR(new KStaticView);
		view->Create(i*task_width, 0, task_width - title_w*2, title_h);
		K9PatchImageDrawable_PTR pDrawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/task_board_title_bk.9.png")), TRUE) );
		pDrawable->setRect(0,0, task_width, title_h );
		view->setBKDrawable(pDrawable);

		view->SetTextBound( RERect::MakeXYWH(0, 5, task_width, title_h ) );
        view->setTextAlign(REPaint::kCenter_Align);
		REColor clr = SkColorSetARGBInline(255,250,250,250);
		view->SetTextColor(clr);
		view->SetFontSize(20);

		//view->SetText( (*it).getName() );
		m_title_group->AddView(view);
		m_board_title.push_back(view);
		view->SetViewChangeType(KVIEW_LRMethod_Parent_LRNone, KVIEW_LRMethod_Parent_LRNone, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		i++;
	}

	calcBoardArea();
}

void CStoryBoardPage::refreshTitle()
{
	list<KStaticView_PTR>::iterator it_btn;
	int n = 0;
	std::vector<CNProjectTaskBoardItem> &items = m_p_prj_data->getTaskBoardItems();
	for ( it_btn = m_board_title.begin(); it_btn != m_board_title.end(); it_btn++ )
	{
		kn_string str = boost::lexical_cast<kn_string>( m_p_prj_data->getTaskCardCount(n) );
		(*it_btn)->SetText( items[n].getName() + _T("(") + str + _T(")") );
		n++;
	}

}

void CStoryBoardPage::calcBoardArea()
{
	if (m_p_prj_data == NULL || m_board_count == 0)
	{
		return;
	}

	CNProjectTask_LST &lst_task = m_p_prj_data->getTasks();
	int count = lst_task.size() ;
	if (count!= m_task_btn.size() || count != m_lst_board.size() / m_board_count || count <1)
	{//三个值应该相等
		return;
	}
	refreshTitle();
	std::vector<CNProjectTaskBoardItem> &board_items = m_p_prj_data->getTaskBoardItems();

	int task_y = m_title_height;
	REPoint point;
	list<KStaticView_PTR>::iterator it_btn = m_task_btn.begin();
	list<CBoardView_PTR>::iterator it_board = m_lst_board.begin();
	int max_action_count = 0;
	int num;
	RERect rect;
	int y_offset;
	if (*it_btn)
	{
		point = (*it_btn)->GetPosition();
		y_offset = point.y() - m_title_height;
	}

	//action_num 一格中一排action数量
	int board_width = (m_rect.width() - m_task_width) / m_board_count;
	int action_num = board_width /m_action_size.x();
	for (CNProjectTask_LST::iterator it = lst_task.begin(); it != lst_task.end(); it++ )
	{
		//设置按钮位置
		point = (*it_btn)->GetPosition();
		(*it_btn)->SetPosition(point.x(), task_y + y_offset);
		(*it_btn)->SetClip(RERect::MakeXYWH(0 ,m_title_height , m_task_width, m_rect.height() - m_title_height ));
		it_btn++;

		//计算故事版最多的action值,最少为1
		max_action_count = 1;
		for (int j=0; j < m_board_count; j++)
		{
			num = (*it).getTaskCardCount(j);
			if (num > max_action_count)
			{
				max_action_count = num;
			}
		}
		int row = max_action_count/action_num;
		if (max_action_count % action_num >0)
		{
			row++;
		}
		int board_height = row*(m_action_size.y() ) + 10;
		for (int j=0; j < m_board_count; j++)
		{
	//		rect = (*it_board)->GetRect();
			//上下边缘留5个
			rect.setXYWH( m_task_width + board_width*j, task_y + y_offset, board_width, board_height );
			(*it_board)->SetRect(rect);
			//(*it_board)->calcActionPos();
			(*it_board)->SetClip(RERect::MakeXYWH(m_task_width ,m_title_height , m_rect.width()- m_task_width, m_rect.height() - m_title_height ));
			it_board++;
		}

		task_y += board_height;		
	}
	//	MyTrace(_T("calcBoardArea"));
}

void CStoryBoardPage::initAnimation()
{

}

void CStoryBoardPage::onKeyUp(KMessageKey* k)
{

}

void CStoryBoardPage::Release()
{
	KViewGroup::Release();
	m_lst_board.clear();
	m_task_btn.clear();
	m_board_title.clear();
	m_p_prj_data = NULL;
	m_lst_task = NULL;
}

////////////////////////////////////////////////

CStoryBoardManPage::CStoryBoardManPage(void)
{
}


CStoryBoardManPage::~CStoryBoardManPage()
{
}

kn_string getSourceIconFile(res_type type )
{
	kn_string file = _T("./resource/img_boy.png");
	switch(type)
	{
	case res_man:
		break;
	case res_woman:
		file = _T("./resource/img_girl.png");
		break;
	case res_device:
		file = _T("./resource/img_device.png");
		break;
	}

	return file;
}

void CStoryBoardManPage::init(CNProjectData* p)
{
	Release();

	m_p_prj_data = p;
	m_task_height = 160;

	m_task_width = 170;
	m_title_height = 100;

	////标题背景
	m_left_view = KStaticView_PTR(new KStaticView);
	m_left_view->Create(0, m_title_height, m_task_width, m_rect.height() - m_title_height);
	m_left_view->setBKGColor( ColorSetARGB(255, 234, 234, 234) );
	m_left_view->showBK(TRUE);
	AddView(m_left_view);
	m_left_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	KStaticView_PTR board_bk_view = KStaticView_PTR(new KStaticView);
	board_bk_view->Create(m_task_width, m_title_height, m_rect.width()-m_task_width, m_rect.height() - m_title_height);
	board_bk_view->setBKGColor( ColorSetARGB(255, 220, 220, 220) );
	board_bk_view->showBK(TRUE);
	AddView(board_bk_view);
	board_bk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	KStaticView_PTR titlebk_view = KStaticView_PTR(new KStaticView);
	titlebk_view->Create(0, 0, m_rect.width(), m_title_height );
	titlebk_view->setBKGColor( ColorSetARGB(255,230,185,127) );
	AddView(titlebk_view);
	titlebk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	KStaticView_PTR titlebk_txt = KStaticView_PTR(new KStaticView);
	titlebk_txt->Create(m_rect.width()/2 - 100, 0, 300, 60 );
	titlebk_txt->SetText( _T("Resource Board") );
	titlebk_txt->SetFontSize(36);
	titlebk_txt->SetTextColor(  ColorSetARGB(255,255,255,255)  );
	AddView(titlebk_txt);
	titlebk_txt->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_msg_view = CStoryMsgView_PTR( new CStoryMsgView) ;
	m_msg_view->Create(0,0, m_rect.width(), m_rect.height());
	AddView(m_msg_view);
	m_msg_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	std::vector<CNProjectTaskBoardItem> &items = p->getTaskBoardItems();

	m_board_count = items.size();

	//故事版 一个块宽度
	int task_width = ( m_rect.width() - m_task_width ) /m_board_count;
	//任务条
	int i = 0;
	int j = 0;
	m_lst_task = m_p_prj_data->getpManTasks();
	CNProjectTask_LST::iterator it_task;

	m_action_size.set(100, 70);



	for ( it_task = m_lst_task->begin(); it_task != m_lst_task->end(); it_task++ )
	{
		for (i = 0; i < m_board_count; i++)
		{
			CBoardView_PTR task_bar = CBoardView_PTR(new CBoardView);
			task_bar->Create(RERect::MakeXYWH(m_task_width + task_width*i, m_title_height + m_task_height*j , task_width-2, m_task_height));
			task_bar->init( &(*it_task), i, j, TRUE);
			task_bar->SetClip( RERect::MakeXYWH(m_task_width, m_title_height , m_rect.width(), m_rect.height() - m_title_height ) );
			AddView(task_bar);
			task_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
			m_lst_board.push_back(task_bar);
		}
		j++;
	}

	//资源按钮
	i = 0;
	for ( it_task = m_lst_task->begin(); it_task != m_lst_task->end(); it_task++ )
	{
		KStaticView_PTR task_view = KStaticView_PTR( new KStaticView() );
		task_view->Create(RERect::MakeXYWH(0, m_title_height+m_task_height*i , m_task_width, 75));
		task_view->SetClip(RERect::MakeXYWH(0, m_title_height , m_task_width, m_rect.height() - m_title_height  ));

		//BK
		K9PatchImageDrawable_PTR bk_drawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/man_btn_bk.9.png")), TRUE ));
		task_view->setBKDrawable(bk_drawable);
		task_view->SetRect( RERect::MakeXYWH(0, m_title_height+m_task_height*i , m_task_width, 75) );

		//Icon
		CNProjectTask* ptask =  &(*it_task);
		res_type type = m_p_prj_data->getResourcePool().getResourceType( ptask->getLeaderId() );
		KImageDrawable_PTR icon = KImageDrawable_PTR(  new KImageDrawable(getSurfaceManager()->GetSurface( getSourceIconFile(type) ),true) );
		icon->setRect(10, 10, 40, 40);
		icon->SizeToImage();
		task_view->setIconDrawable(icon);
		// Name
		task_view->SetText(  ptask->getLeaderName() );
		task_view->SetTextBound(  RERect::MakeXYWH( 65, 15, 100, 40 ) );
		m_task_btn.push_back(task_view);
		//	task_view->m_clicked_signal.connect(this, &CTimeBarPage::OnTaskViewClick);
		AddView(task_view);
		task_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		i++;
	}


	//标题背景
	//KStaticView* bk_view = new KStaticView();
	//bk_view->Create(0, 0, m_rect.width(), m_title_height);
	//bk_view->setBKGColor(ColorSetARGB(255,50,50,50) );
	//AddView(bk_view);


	///故事版标题
	std::vector<CNProjectTaskBoardItem>::iterator it;
	int title_w = 3;
	int title_h  = 40;
	i = 0;

	//处理屏幕缩放，增加一个层级
	KViewGroup_PTR  m_title_group =KViewGroup_PTR(new KViewGroup);
	m_title_group->Create(title_w + m_task_width, m_title_height - title_h, m_rect.width() - m_task_width - title_w, title_h);
	AddView(m_title_group);
	m_title_group->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);


	for ( it = items.begin(); it != items.end(); it++ )
	{
		KStaticView_PTR view = KStaticView_PTR(new KStaticView);
		view->Create(i*task_width, 0, task_width - title_w*2, title_h);
		K9PatchImageDrawable_PTR pDrawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/man_board_title_bk.9.png")), TRUE) );
		pDrawable->setRect(0,0, task_width - title_w*2, title_h );
		view->setBKDrawable(pDrawable);

		view->SetTextBound( RERect::MakeXYWH(0, 8, task_width - title_w*2, title_h ) );
        view->setTextAlign(REPaint::kCenter_Align);
		REColor clr = SkColorSetARGBInline(255,255,255,255);
		view->SetTextColor(clr);
		view->SetFontSize(20);
		//view->SetText( (*it).getName() );
		m_title_group->AddView(view);
		m_board_title.push_back(view);
		view->SetViewChangeType(KVIEW_LRMethod_Parent_LRNone, KVIEW_LRMethod_Parent_LRNone, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		i++;
	}

	calcBoardArea();
}


void CStoryBoardManPage::calcBoardArea()
{
	if (m_p_prj_data == NULL|| m_board_count == 0)
	{
		return;
	}



	CNProjectTask_LST* lst_task = m_p_prj_data->getpManTasks();
	int count = lst_task->size() ;
	if ( (count!= m_task_btn.size() || count != m_lst_board.size() / m_board_count || count <1))
	{//三个值应该相等
		return;
	}


	refreshTitle();

	std::vector<CNProjectTaskBoardItem> &board_items = m_p_prj_data->getTaskBoardItems();

	int task_y = m_title_height;
	REPoint point;
	list<KStaticView_PTR>::iterator it_btn = m_task_btn.begin();
	list<CBoardView_PTR>::iterator it_board = m_lst_board.begin();
	int max_action_count = 0;
	int num;
	RERect rect;
	int y_offset;
	if (*it_btn)
	{
		point = (*it_btn)->GetPosition();
		y_offset = point.y() - m_title_height;
	}

	//action_num 一格中一排action数量
	int board_width = (m_rect.width() - m_task_width) / m_board_count;


	int action_num = board_width /m_action_size.x();

	if(action_num == 0) // 避免除0
	{
		action_num = 1;
	}


	list<KStaticView_PTR>::iterator it_man_view = m_task_btn.begin();
	for (CNProjectTask_LST::iterator it = lst_task->begin(); it != lst_task->end(); it++ )
	{
		//设置按钮位置
		point = (*it_btn)->GetPosition();
		(*it_btn)->SetPosition(point.x(), task_y + y_offset);
		(*it_btn)->SetClip(RERect::MakeXYWH(0 ,m_title_height , m_task_width, m_rect.height() - m_title_height ));
		it_btn++;

		//计算故事版最多的action值,最少为1
		max_action_count = 1;
		for (int j=0; j < m_board_count; j++)
		{
			num = (*it).getManCardCount(j);
			if (num > max_action_count)
			{
				max_action_count = num;
			}
		}
		int row = max_action_count/action_num;
		if (max_action_count % action_num >0)
		{
			row++;
		}
		int board_height = row*(m_action_size.y() ) + 10;
		for (int j=0; j < m_board_count; j++)
		{
			rect = (*it_board)->GetRect();
			//上下边缘留5个
			//rect.setXYWH( rect.left(), task_y + y_offset, rect.width(), board_height );
			rect.setXYWH( m_task_width + board_width*j, task_y + y_offset, board_width, board_height );
			(*it_board)->SetRect(rect);
			(*it_board)->calcActionPos();
			(*it_board)->SetClip(RERect::MakeXYWH(m_task_width ,m_title_height , m_rect.width()- m_task_width, m_rect.height() - m_title_height ));
			it_board++;
		}

		//设置左边资源
		//KStaticView* man_view = (*it_man_view);
		//man_view->SetRect( RERect::MakeXYWH( rect.left(), task_y + y_offset, rect.width(), board_height ) );
		//RERect txt_rect = man_view->GetTextBound();
		//man_view->SetTextBound( RERect::MakeXYWH( txt_rect.left(), board_height - 15, m_task_width, board_height) );
		//man_view->setIconPos( 5, board_height - 20 );
		//it_man_view++;

		task_y += board_height;		
	}



//	MyTrace(_T("calcBoardArea"));
}



