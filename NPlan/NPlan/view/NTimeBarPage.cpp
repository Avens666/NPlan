//  File:        NTimeBarPage.cpp
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
#include "NTimeBarPage.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "boost/lexical_cast.hpp"
#include "NTaskSourceView.h"
#include "NMenu.h"
#include "NProjectPage.h"
extern HWND g_hWnd; 
CTimeBarPage::CTimeBarPage(void)
{
	m_e_viewtype = KVIEW_MOVEABLE;
	m_action_h = 34;
	m_task_height = 75; //任务按钮高度
	m_task_width = 150;
	m_mode = MODE_BIG;

	m_drag_dir = DRAG_UNKNOWN;
	m_nDragViewIndex = -1;
	m_nDragChangeIndex = -1;
	m_nWheelOffset = 0;

	// 运行缩放手势
	enableGesture(true);

}

void CTimeBarPage::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &CTimeBarPage::onKeyUp);
}


CTimeBarPage::~CTimeBarPage()
{
	m_ani_thread.Stop();

}

void CTimeBarPage::OnMoveSilderWholeProject()
{
	if (m_time_bar && m_silder_bar)
	{
		ptime start_t = m_prj_data->getProBeginTime() ;
		ptime end_t = m_prj_data->getProEndTime() ;

		if (start_t.is_not_a_date_time() || end_t.is_not_a_date_time() )
		{
			ptime pt = second_clock::local_time();

			start_t =  ptime( date(pt.date().year(), 1, 1), time_duration(8, 0, 0));
			end_t =  ptime( date(pt.date().year(), 12, 31), time_duration(18, 0, 0));
		
		}

			UINT tw = m_time_bar->getTimeDurationWidth(start_t, end_t);

			m_time_bar->setTimelineOneDayWidth( tw,true);
			int pos = m_time_bar->getTimePosition(start_t);
			m_time_bar->setTimeLineOffset(m_time_bar->getTimeLineOffset() - pos + 10 );
		

	}

}


void CTimeBarPage::OnTaskbarDClick(int x, int y, KMessageMouse* v)
{
	if (m_action_bk->isShow())
	{
		if ( !hideActionEdit() )
	  {
		  return;
	  }
	}
	 GetScreen()->sendNUIMessage( TIMEPAGE_TASK_EDIT );

	CTaskBarView_PTR p_task_v = VIEW_PTR(CTaskBarView)(v->m_p_view);
	CNProjectTask* p = p_task_v->GetTask();

#ifdef WIN32
	tagRECT rect;
	GetWindowRect(g_hWnd,&rect);
#else
    tagRECT rect = {0,0,1024,768};
#endif
	CNProjectTask task_back = *p;
	NTaskSourceView_PTR task_dlg = NTaskSourceView_PTR(new NTaskSourceView( &task_back ) );
	int w = GetScreen()->GetWidth();
	int h = GetScreen()->GetHeight();

	task_dlg->Create(RERect::MakeXYWH((w-800)/2, (h-620)/2 , 800, 620));
	task_dlg->init();

	GetParent()->AddView(task_dlg);
	

	if (task_dlg->doModal() == KN_REUSLT_OK)
	{
//		CNProjectTask task = task_back; //这个＝可以调用拷贝构造函数
//		*p = task;//这个不行
		*p = task_back;
	}

	p_task_v->refreshInfo();
	p_task_v->getTaskView()->refreshInfo();
}


void CTimeBarPage::OnTaskbarRightClick(kn_int x, kn_int y, KMessageMouse* v)
{
	//if( !isPointInView( pMsg->m_pos_x, pMsg->m_pos_y))
	//{
	//	return;
	//}
	if (m_action_bk->isShow())
	{
		hideActionEdit();
	}
	else
	{
		showActionEdit( VIEW_PTR(CTaskBarView)(v->m_p_view) );
	}
}

void CTimeBarPage::showActionEdit(CTaskBarView_PTR v)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	CNProjectTaskCard_LST& action_lst = v->GetTask()->getTaskCards();

	if (action_lst.size() == 0)
	{
		return;
	}

	m_action_h = 34;
	int h = action_lst.size()*m_action_h; //要展开的高度
	CTaskBarView_PTR p_task_v = v;

	//计算界面展开位置，优先往下展开，位置不够再往上
	int up_y = 0; //向上移动 是负数
	int down_y = 0; //向下移动
	if (v->GetRect().bottom() + h < m_rect.height())
	{//空间够 只往下展开即可
		down_y = h;
	}
	else if ( ( h ) < m_rect.height() - m_task_y - m_task_height )
	{//往上够
		down_y = m_rect.height() - v->GetRect().bottom() -20;
		up_y = down_y - h;
	}
	else
	{//往上不够
		up_y = m_task_y - v->GetRect().top() ;
		down_y = h + up_y;
	}
	int action_y = up_y+v->GetRect().bottom();
	////计算完毕///

	///展开动画
	REPoint point;
	std::list<CTaskBarView_PTR>::iterator it_task;
	RERect rect = m_time_bar->getTimelineArea(); 
	rect.offset(0, m_event_height);

	bool b_up = TRUE;//判断在之前还是之后
	int offset;
	for ( it_task = m_task_bar.begin(); it_task != m_task_bar.end(); it_task++ )
	{
		point = (*it_task)->GetPosition();
		if (b_up)
		{
			offset = up_y;
		}
		else
		{
			offset = down_y;
		}
		m_ani_thread.addAnimationHelper((*it_task), PropertyPARA_ID_POS_Y, point.y()+offset, 500,0,0 );
		m_ani_thread.addAnimationHelper(( VIEW_PTR(CTaskBarView)(*it_task) )->getTaskView(), PropertyPARA_ID_POS_Y, point.y()+offset, 500,0,0 );

		if ((*it_task)  == p_task_v)
		{
			b_up= FALSE;
		}
	}

	//置顶
	changeViewLayerTop(m_action_bk);
	m_action_bk->SetRect( RERect::MakeXYWH(0, v->GetRect().bottom(), m_rect.width(), 0 ));
	m_ani_thread.addRectAnimationHelper(m_action_bk, RERect::MakeXYWH(0,action_y, m_rect.width(), h), 500,0,0 );
	m_action_bk->SetShow(TRUE);

	//生成action view
	RERect timeline_rect = m_time_bar->getTimelineArea(); 
	CNProjectTaskCard_LST::iterator it;
	int i = 0;
	int count = action_lst.size();
	for (it = action_lst.begin(); it != action_lst.end(); ++it )
	{
		CActionBarView_PTR action_bar = CActionBarView_PTR(new CActionBarView);
		action_bar->Create(RERect::MakeXYWH(0,action_y + m_action_h*i, 100, m_action_h ));
		action_bar->init( &(*it), m_time_bar);
		action_bar->syncTimeline(m_time_bar);
		action_bar->SetClip(RERect::MakeXYWH( m_task_width , m_task_y , timeline_rect.width(), timeline_rect.height()));
		m_action_bar.push_back(action_bar);
		AddView(action_bar);
		action_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top,KVIEW_BTMethod_Parent_Top);
		action_bar->setOpacity(0);
		m_ani_thread.addAnimationHelper( action_bar, PropertyPARA_ID_Opacity, 255, 250, 400+ i*80,0 );
		i++;
	}

	v->setActionShow(TRUE);
	m_ani_thread.Start();

}

bool CTimeBarPage::hideActionEdit()
{
	if ( m_action_bk->isShow() == FALSE)
	{
		return TRUE;
	}
	if (m_ani_thread.isPlaying())
	{
		return FALSE;
	}
	m_ani_thread.clearAnimation();

	//action edit
		std::list<CActionBarView_PTR>::iterator it_action;
		for ( it_action = m_action_bar.begin(); it_action != m_action_bar.end(); it_action++ )
		{
			m_ani_thread.addAnimationHelper( *it_action, PropertyPARA_ID_Opacity, 0, 300, 0,0 );
		}

	RERect rect = m_action_bk->GetRect();
	int offset = -rect.height();

	m_ani_thread.addRectAnimationHelper(m_action_bk, RERect::MakeXYWH(rect.left(),rect.top(), rect.width(), 0), 500,250,0 );
	bool b_up = FALSE;//判断在之前还是之后
	REPoint point;
	std::list<CTaskBarView_PTR>::iterator it_task;
	for ( it_task = m_task_bar.begin(); it_task != m_task_bar.end(); it_task++ )
	{
		if (b_up)
		{
			point = (*it_task)->GetPosition();
			m_ani_thread.addAnimationHelper((*it_task), PropertyPARA_ID_POS_Y, point.y()+offset, 500,250,0 );
			m_ani_thread.addAnimationHelper((VIEW_PTR(CTaskBarView)(*it_task) )->getTaskView(), PropertyPARA_ID_POS_Y, point.y()+offset, 500,250,0 );
		}

		if ( (VIEW_PTR(CTaskBarView)(*it_task) )->isActionShow())
		{
			(VIEW_PTR(CTaskBarView)(*it_task) )->setActionShow(FALSE);
			b_up= TRUE;
		}
	}

	m_ani_thread.setStopMsg(  TIMEPAGE_TASK_EDIT_ANI_HIDE);
	m_ani_thread.Start();

		return TRUE;
}

void CTimeBarPage::OnTaskViewClick(KView_PTR v)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}

	m_ani_thread.clearAnimation();

	CTaskView_PTR p_task_v = VIEW_PTR(CTaskView)(v);

	int pos = m_time_bar->getTimePosition( p_task_v->GetTask()->getStartTime() ) - 20;

	m_ani_thread.addAnimationHelper(m_time_bar, PARA_ID_TimeBar, m_time_bar->getTimeLineOffset() - pos, 500,0,0 );
	m_ani_thread.Start();
}

void CTimeBarPage::timelineScaleTo(int v , int pos)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}

	m_ani_thread.clearAnimation();
	m_ani_thread.addAnimationHelper(m_time_bar, PARA_ID_TimeBar_Scale, v, 400,0,0 );
	m_ani_thread.addAnimationHelper(m_time_bar, PARA_ID_TimeBar, m_time_bar->getTimeLineOffset() - pos, 400,0,0 );
	m_ani_thread.Start();
}

void CTimeBarPage::OnTaskViewRClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	CMenuColor_PTR menu = CMenuColor_PTR(new CMenuColor );
	menu->Create(0,0, 38*3+12, 38*4+12);
	//注意传入点击的taskview
	menu->init(pMsg->m_p_view );
	menu->m_sign_menu_view_click.connect(this, &CTimeBarPage::onColorMenu);
	//showmenu 传入view自身的相对坐标，这里取鼠标点击位置
	showMenu(menu, iScreenX - m_rect.x(), iScreenY - m_rect.y());
	InvalidateView();	
}

void CTimeBarPage::onColorMenu(KView_PTR v, kn_int n)
{
	if (n>=1 && n <=9)
	{//换颜色
		kn_string bk_file =  _T("./resource/0"); 
		bk_file += boost::lexical_cast<kn_string>( n );
		bk_file += _T(".9.png");
		K9PatchImageDrawable_PTR bk_drawable(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( bk_file), TRUE ));
		//	((K9PatchImageDrawable_PTR)bk_drawable)->SizeToImage();
		bk_drawable->SetRect(RERect::MakeXYWH(0, 0, v->GetRect().width(), v->GetRect().height()) );
		CTaskView_PTR task_view = VIEW_PTR(CTaskView)(v);
		task_view->setIconDrawable(bk_drawable);
		task_view->GetTask()->setColorType(n);

		//////// del task bar
		CTaskBarView_PTR task_bar = task_view->getTaskBar();
		if (!task_bar)
		{
			return;
		}

		bk_file =  _T("./resource/timebar"); 
		bk_file += boost::lexical_cast<kn_string>( n );
		bk_file += _T(".9.png");

		K9PatchImageDrawable_PTR bk_bar(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( bk_file), TRUE ));
		task_bar->setTaskBarBK(bk_bar);
		//	((K9PatchImageDrawable_PTR)bk_drawable)->SizeToImage();
//		bk_drawable->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()) );
//		(*it_task_bar)->setBKDrawable(bk_drawable);
		
	}
	else if (n == 0)
	{//del task
		onDelTask( VIEW_PTR(CTaskView)(v) );
	}

}

void CTimeBarPage::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{

}


void CTimeBarPage::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void CTimeBarPage::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}



kn_int  CTimeBarPage::getTaskY()
{
	if (m_task_btn.size() ==0)
	{
		return 0;
	}
	std::list<CTaskView_PTR>::iterator it;
	it = m_task_btn.begin();
	return (*it)->GetPosition().y();

}

void CTimeBarPage::setTaskY(kn_int y)
{

	if (m_task_btn.size() ==0)
	{
		return;
	}

	REPoint point;

	std::list<CTaskView_PTR>::iterator it;

	it = m_task_btn.begin();
	point = (*it)->GetPosition();

	int offset = y - point.y();
	moveTaskY(offset,FALSE);
}

void CTimeBarPage::moveTaskY(int offset, bool b_check_ani)
{
	if (m_ani_thread.isPlaying() && b_check_ani)
	{//是否检查动画，动画驱动方式不检查，事件驱动时检查
		return;
	}
	if (m_task_btn.size() ==0)
	{
		return;
	}

	REPoint point;
	std::list<CTaskBarView_PTR>::iterator it_task;
	std::list<CTaskView_PTR>::iterator it;
	RERect rect = m_time_bar->getTimelineArea(); 
	rect.offset(0, m_event_height);
	
	it = m_task_btn.begin();
	point = (*it)->GetPosition();

	if (m_task_btn.size() * m_task_height + m_action_bar.size()*m_action_h < rect.height()/2 && point.y() == rect.top())
	{//元素太少　不用上下移动
		return;
	}

	if (point.y() + offset > rect.top())
	{
		offset =  rect.top() - point.y();
	}

	it = m_task_btn.end();
	it--;
	point = (*it)->GetPosition();
	if (point.y() + offset < rect.top() + rect.height()/2 && point.y() >= rect.top() + rect.height()/2 )
	{
		return;
	}


	for ( it_task = m_task_bar.begin(); it_task != m_task_bar.end(); it_task++ )
	{
		point = (*it_task)->GetPosition();
		(*it_task)->SetPosition(point.x(), point.y()+offset);
	}

	
	for ( it = m_task_btn.begin(); it != m_task_btn.end(); it++ )
	{
		point = (*it)->GetPosition();
		(*it)->SetPosition(point.x(), point.y()+offset);
	}
	m_nWheelOffset = m_nWheelOffset +  offset;

	if (m_action_bk->isShow() )
	{//action edit
		point = m_action_bk->GetPosition();
		m_action_bk->SetPosition(point.x(), point.y()+offset);
		std::list<CActionBarView_PTR>::iterator it_action;
		for ( it_action = m_action_bar.begin(); it_action != m_action_bar.end(); it_action++ )
		{
			point = (*it_action)->GetPosition();
			(*it_action)->SetPosition(point.x(), point.y()+offset);
		}
	}
}

void CTimeBarPage::onWheelDirect(KMessageMouseWheel* pMsg)
{
	RERect rect = m_time_bar->getTimelineArea();
	if (rect.contains(pMsg->m_pos_x, pMsg->m_pos_y ) )
	{
		if( pMsg->m_roll > 0)
		{
			moveTaskY(50,TRUE);
		}
		else
		{
			moveTaskY(-50,TRUE);
		}
		InvalidateView();
	}
}
void CTimeBarPage::setMode(bool b)
{
	if (m_action_bk->isShow())
	{
		hideActionEdit();
		return;
	}

	if (m_mode == b)
	{
		return;
	}

	RERect rect;
	int i = 0;
	int start_y = 0; //第一个的起始位置
	if (b == MODE_BIG)
	{//大模式
		m_task_height = 75;
	}
	else
	{
		m_task_height = 38;
	}

	m_unfold_btn->SetShow(m_mode);
	m_fold_btn->SetShow(!m_mode);
	if (m_mode)
	{
		list<CTaskView_PTR>::iterator it_task_btn;
		for ( it_task_btn = m_task_btn.begin(); it_task_btn != m_task_btn.end(); it_task_btn++ )
		{
			changeViewLayerTop(*it_task_btn);
		}
	}

		std::list<CTaskView_PTR>::iterator it_task;
		i = 0;
		for ( it_task = m_task_btn.begin(); it_task != m_task_btn.end(); it_task++ )
		{
			rect = (*it_task)->GetRect();
			if (i == 0)
			{
				start_y = rect.top();
			}
			(*it_task)->setRect(rect.left(),start_y+i*m_task_height, rect.width(),m_task_height);
			(*it_task)->setMode(b);
			i++;
		}
		
		std::list<CTaskBarView_PTR>::iterator it_action;
		i = 0;
		for ( it_action = m_task_bar.begin(); it_action != m_task_bar.end(); it_action++ )
		{
			rect = (*it_action)->GetRect();
				if (i == 0)
				{
					start_y = rect.top();
				}
				(*it_action)->setRect(rect.left(),start_y+i*m_task_height, rect.width(),m_task_height);
				(*it_action)->setMode(b);
				i++;
		}

	m_mode = b;
	InvalidateView();
}

void CTimeBarPage::OnTimelineChange(kn_bool b)
{
	std::list<CTaskBarView_PTR>::iterator it_task;
	for ( it_task = m_task_bar.begin(); it_task != m_task_bar.end(); it_task++ )
	{
		(*it_task)->syncTimeline(m_time_bar);
	}

	std::list<CEventView_PTR>::iterator it_event;
	for ( it_event = m_lst_event.begin(); it_event != m_lst_event.end(); it_event++ )
	{
		(*it_event)->syncTimeline(m_time_bar);
	}

	std::list<CActionBarView_PTR>::iterator it_action;
	for ( it_action = m_action_bar.begin(); it_action != m_action_bar.end(); it_action++ )
	{
		(*it_action)->syncTimeline(m_time_bar);
	}
	if (b)
	{
		m_silder_bar->SetValue(m_time_bar->getTimelineOneDayWidth());
	}
	InvalidateView();

}

void CTimeBarPage::init(CNProjectData* p)
{
	m_prj_data = p;

	m_task_height = 75; //任务按钮高度
	m_task_width = 170;
	m_event_height = 30;
	m_task_y = 130;
	int silder_bar_width = 460;
	int silder_bar_height = 14;
	int silder_bar_Y = 23;

	//时间轴group
	//m_timebar_group = new KViewGroup();
	//m_timebar_group->Create(m_task_width, 0, m_rect.width()-m_task_width, m_rect.height() );
	//AddView(m_timebar_group);
	REColor bk_clr = ARGB(255,53,62,91);

	KColorDrawable_PTR lt_bk_drawable = KColorDrawable_PTR(new KColorDrawable(bk_clr ) );
	lt_bk_drawable->setRect(0,0, m_rect.width(), m_task_y);
	lt_bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	lt_bk_drawable->setAutoSize(TRUE);
	addDrawable( lt_bk_drawable);

	////左侧背景
	m_left_view = KStaticView_PTR( new KStaticView);
	m_left_view->Create(0, m_task_y-30, m_task_width, m_rect.height());
	m_left_view->setBKGColor( ColorSetARGB(255,234,234,234) );
	AddView(m_left_view);
	m_left_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	//action编辑时的背景
	m_action_bk =  KStaticView_PTR( new KStaticView);
	m_action_bk->Create(0, 0, m_rect.width(), 0);
	K9PatchImageDrawable_PTR action_bk = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface( _T("./resource/actionbar_bk.9.png")), TRUE) );
	m_action_bk->setBKDrawable(action_bk);
//	m_action_bk->setBKGColor( ColorSetARGB(255,55,55,55) );
	m_action_bk->SetClip(RERect::MakeXYWH(0, m_task_y, m_rect.width(), m_rect.height() - m_task_y));
	m_action_bk->SetShow(FALSE);
	AddView(m_action_bk);
	m_action_bk->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);


	////时间轴
	m_time_bar =CTimeBarView_PTR(new CTimeBarView);
	m_time_bar->Create(RERect::MakeXYWH(m_task_width,0 , m_rect.width()-m_task_width, m_rect.height()));
	m_time_bar->init();
	AddView(m_time_bar);
	m_time_bar->m_timeline_change_signal.connect(this, &CTimeBarPage::OnTimelineChange);
	m_time_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_time_bar->setCalendar( &( m_prj_data->getCalendar()) );
	m_time_bar->m_sign_dclick.connect(this, &CTimeBarPage::OnPrjDClick);

	// taskbar都在时间轴的area区域
	RERect rect = m_time_bar->getTimelineArea(); 
	// 时间轴区域，不含event
	int task_top = rect.top()+m_event_height;

	//白色透明条
	KStaticView_PTR event_bk_view = KStaticView_PTR( new KStaticView);
	event_bk_view->Create(0, 100, m_rect.width(), 30);
	event_bk_view->setBKGColor( ColorSetARGB(200,255,255,255) );
	AddView(event_bk_view);
	event_bk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);


	//任务条
	int i = 0;
	m_lst_task = m_prj_data->getpTasks();
	CNProjectTask_LST::iterator it_task;
	for ( it_task = m_lst_task->begin(); it_task != m_lst_task->end(); it_task++ )
	{
		addTask( &(*it_task), 0 );
	}

	//里程碑
	CNProjectMileStone_LST  &lst_mile_stone = m_prj_data->getMilestones();
	CNProjectMileStone_LST::iterator it_ms;
	for ( it_ms = lst_mile_stone.begin(); it_ms != lst_mile_stone.end(); it_ms++ )
	{
		addEvent( &(*it_ms) );
	}

	//滑动条
	m_silder_bar = NNormalSilderView_PTR( new NNormalSilderView);
	m_silder_bar->Create(RERect::MakeWH(silder_bar_width,silder_bar_height));
	m_silder_bar->InitCtrl();
	m_silder_bar->SetRollDir(DIR_X);
	m_silder_bar->SetSilderBG(_T("./resource/title_bar.9.png"),
		RERect::MakeXYWH((m_rect.width() - silder_bar_width)/2,silder_bar_Y,silder_bar_width,silder_bar_height));
	m_silder_bar->SetSilderVernier(_T("./resource/silder_cursor_opacity_n.png"),_T("./resource/silder_cursor_opacity_a.png"));;
	vector<SilderValue> vecText;
	vecText.push_back(SilderValue(_T("年"),32));
	vecText.push_back(SilderValue(_T("月"),120));
	vecText.push_back(SilderValue(_T("日"),3125));
	vecText.push_back(SilderValue(_T("时"),36000));
	m_silder_bar->SetShowText(vecText,silder_bar_width,
		_T("./resource/silder_cursor_l_n.png"),
		_T("./resource/silder_cursor_l_a.png"),
		_T("./resource/silder_cursor_m_n.png"),
		_T("./resource/silder_cursor_m_a.png"),
		_T("./resource/silder_cursor_r_n.png"),
		_T("./resource/silder_cursor_r_a.png"));

//	m_silder_bar->AddArea(30,_T("./resource/silder_cursor_n.png"),_T("./resource/silder_cursor_a.png"));
	m_silder_bar->m_silder_value_change_sign.connect(this, &CTimeBarPage::OnMoveSilder);
	m_silder_bar->m_signal_custom_area.connect(this, &CTimeBarPage::OnMoveSilderWholeProject);
	m_silder_bar->m_signal_change_end.connect(this, &CTimeBarPage::OnRefershSilder);
	

	AddView(m_silder_bar);
	m_silder_bar->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	//// addTask
	KImgButtonView_PTR pView ;
	createImgView9PatchIconHelper(&pView, _T("./resource/b_n.9.png"), _T("./resource/b_a.9.png"),_T("./resource/b_f.9.png"), _T("./resource/mission_add_icon.png"), 5, 100, 32,28, ICON_SENTER, ICON_SENTER);
	pView->m_clicked_signal.connect(this, &CTimeBarPage::onAddTask  );

	pView->setTip(getStringManager()->GetStringByID(_T("add_action")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	createImgView9PatchIconHelper(&pView, _T("./resource/b_n.9.png"), _T("./resource/b_a.9.png"),_T("./resource/b_f.9.png"), _T("./resource/milepost_add_icon.png"), 40, 100, 32,28, ICON_SENTER, ICON_SENTER);
	pView->m_clicked_signal.connect(this, &CTimeBarPage::onAddEvent  );
	pView->setTip(getStringManager()->GetStringByID(_T("add_milestone")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

//	createImgViewHelper(&pView, _T("./resource/addstone_n.png"), _T("./resource/addstone_a.png"),_T("./resource/addstone_f.png"), 45, 100);
	createImgView9PatchIconHelper(&pView, _T("./resource/b_n.9.png"), _T("./resource/b_a.9.png"),_T("./resource/b_f.9.png"), _T("./resource/milepost_left_icon.png"), 75, 100, 32,28, ICON_SENTER, ICON_SENTER);
	kn_string str=_T("prev");
	pView->setName(str);
	pView->m_clicked_signal.connect(this, &CTimeBarPage::onEventPrev  );

	pView->setTip(getStringManager()->GetStringByID(_T("prev_milestone")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	createImgView9PatchIconHelper(&pView, _T("./resource/b_n.9.png"), _T("./resource/b_a.9.png"),_T("./resource/b_f.9.png"), _T("./resource/milepost_right_icon.png"), 110, 100, 32,28, ICON_SENTER, ICON_SENTER);
	str = _T("forward");
	pView->setName(str);
	pView->m_clicked_signal.connect(this, &CTimeBarPage::onEventPrev  );
	pView->setTip(getStringManager()->GetStringByID(_T("next_milestone")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	createImgViewHelper(&m_fold_btn, _T("./resource/collapse_n.png"), _T("./resource/collapse_a.png"),_T("./resource/collapse_f.png"), 108, 15);
	m_fold_btn->m_clicked_signal.connect(this, &CTimeBarPage::onFoldClick );

	m_fold_btn->setTip(getStringManager()->GetStringByID(_T("fold_prj")),NO_TIMER,4000);
	m_fold_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	createImgViewHelper(&m_unfold_btn, _T("./resource/unfold_n.png"), _T("./resource/unfold_a.png"),_T("./resource/unfold_f.png"), 108, 15);
	m_unfold_btn->m_clicked_signal.connect(this, &CTimeBarPage::onUnfoldClick );
	m_unfold_btn->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_unfold_btn->setTip(getStringManager()->GetStringByID(_T("unfold_prj")),NO_TIMER,4000);
	m_unfold_btn->SetShow(FALSE);
	
	//根据项目时间设置时间轴
	OnMoveSilderWholeProject();

}

void CTimeBarPage::addTask(CNProjectTask* p_task, int task_x)
{
	RERect rect = m_time_bar->getTimelineArea(); 

	//计算高度
	int task_top = rect.top()+m_event_height;
	int task_y = task_top;
	if (m_task_bar.size() > 0)
	{
		list<CTaskBarView_PTR>::reverse_iterator it_task_bar = m_task_bar.rbegin() ;
		task_y = (*it_task_bar)->GetPosition().y() + m_task_height;
	}

	CTaskBarView_PTR task_bar = CTaskBarView_PTR(new CTaskBarView);
	task_bar->Create(RERect::MakeXYWH(0,task_y , 100, m_task_height));
	task_bar->init( p_task, m_time_bar);
	task_bar->syncTimeline(m_time_bar);
	task_bar->SetClip(RERect::MakeXYWH(rect.left() ,task_top , rect.width(), rect.height()));
	m_task_bar.push_back(task_bar);
	AddView(task_bar);
	task_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top,KVIEW_BTMethod_Parent_Top);
	task_bar->m_sign_dclick.connect(this, &CTimeBarPage::OnTaskbarDClick);
	task_bar->m_sign_rup.connect(this, &CTimeBarPage::OnTaskbarRightClick);


	CTaskView_PTR task_view =CTaskView_PTR(new CTaskView);
	task_view->Create(RERect::MakeXYWH(task_x, task_y , m_task_width, m_task_height));
	task_view->init( p_task );
	task_view->SetClip(RERect::MakeXYWH(0, task_top, m_task_width, rect.height()));
	m_task_btn.push_back(task_view);
	task_view->m_task_click_sign.connect(this, &CTimeBarPage::OnTaskViewClick);
	task_view->m_sign_rup.connect(this, &CTimeBarPage::OnTaskViewRClick);
	//task_view->m_sign_rdclick.connect(this, &CTimeBarPage::onDelTask);

	//好基友 互相指向，方便后续处理
	task_view->setTaskBar(task_bar);
	task_bar->setTaskView(task_view);

	AddView(task_view);
	task_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top,KVIEW_BTMethod_Parent_Top);
}

void CTimeBarPage::addEvent(CNProjectMileStone* event)
{
	CEventView_PTR v = CEventView_PTR( new CEventView);
	RERect rect = m_time_bar->getTimelineArea(); 
	v->Create(RERect::MakeXYWH(0, rect.top(), 25, m_event_height + rect.height() ));
	v->init( event, m_time_bar);
	v->syncTimeline(m_time_bar);
	v->SetClip(RERect::MakeXYWH(rect.left(),rect.top() - m_event_height, rect.width(), m_event_height + rect.height()));
	m_lst_event.push_back(v);
	v->m_sign_btn_del.connect(this,&CTimeBarPage::delMileStone);
	AddView(v);
	v->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top,KVIEW_BTMethod_Parent_Bottom);
}

void CTimeBarPage::viewSizeChange()
{
	KViewGroup::viewSizeChange();
	RERect rect = m_time_bar->getTimelineArea(); 
	int task_top = rect.top()+m_event_height;
	//裁剪区需要手动恢复设置
	list<CTaskBarView_PTR>::iterator it_task_bar;
	for ( it_task_bar = m_task_bar.begin(); it_task_bar != m_task_bar.end(); it_task_bar++ )
	{
		(*it_task_bar)->SetClip(RERect::MakeXYWH(rect.left() ,task_top , rect.width(), rect.height()));
	}

	list<CTaskView_PTR>::iterator it_task_btn;
	for ( it_task_btn = m_task_btn.begin(); it_task_btn != m_task_btn.end(); it_task_btn++ )
	{
		(*it_task_btn)->SetClip(RERect::MakeXYWH(0 ,task_top , m_task_width, rect.height()));
	}

	list<CEventView_PTR>::iterator it_event;
	for ( it_event = m_lst_event.begin(); it_event != m_lst_event.end(); it_event++ )
	{
		(*it_event)->SetClip(RERect::MakeXYWH(rect.left() ,rect.top()  - m_event_height, rect.width(),  m_event_height + rect.height()));
	}

	list<CActionBarView_PTR>::iterator it_action;
	for ( it_action = m_action_bar.begin(); it_action != m_action_bar.end(); it_action++ )
	{
		(*it_action)->SetClip(RERect::MakeXYWH(rect.left() ,task_top , rect.width(), rect.height()));
	}
	m_action_bk->SetClip(RERect::MakeXYWH(0, task_top,  m_rect.width(), rect.height()) );

}

void CTimeBarPage::initAnimation()
{
	//构造动画
	if (m_task_bar.size() == 0)
	{
		return;
	}
	 
	list<CTaskBarView_PTR>::iterator it_task_bar;
	int i = 0;
	for ( it_task_bar = m_task_bar.begin(); it_task_bar != m_task_bar.end(); it_task_bar++ )
	{
		CNUIAnimation* pNUIAnimation = new CNUIAnimation (300+i*80, 700, 0);
		(*it_task_bar)->setOpacity(0);
		CPropertyOpacity* pPosDst = new CPropertyOpacity((*it_task_bar), 255);

		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPosDst);
		pNUIAnimation->addAnimationData(pAnimationData);
		m_ani_thread.addAnimation(pNUIAnimation);
		if (i >= 10)
		{
			break;
		}
		i++;
	}

	i=0;
	list<CTaskView_PTR>::iterator it_task_btn;
	for ( it_task_btn = m_task_btn.begin(); it_task_btn != m_task_btn.end(); it_task_btn++ )
	{
		(*it_task_btn)->setOpacity(0);
		(*it_task_btn)->SetPositionX(-150);

		//CNUIAnimation* pNUIAnimation = new CNUIAnimation (200+i*100,700, 0);
		//CPropertyOpacity* pPosDst = new CPropertyOpacity((*it_task_btn), 255);
		//CPropertyPos* pPos = new CPropertyPos((*it_task_btn), 0, (*it_task_btn)->GetPosition().y());
		//CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPosDst);
		//CPropertyAnimationData* pAnimationData2 = new CPropertyAnimationData(pPos);
		//pNUIAnimation->addAnimationData(pAnimationData);
		//pNUIAnimation->addAnimationData(pAnimationData2);
		//m_ani_thread.addAnimation(pNUIAnimation);

		m_ani_thread.addAnimationHelper( (*it_task_btn), PropertyPARA_ID_Opacity, 255, 200+i*100,700, 0 );
		m_ani_thread.addAnimationHelper( (*it_task_btn), PropertyPARA_ID_POS_X, 0, 200+i*100,700, 0 );

		if (i >= 10)
		{
			break;
		}
		i++;
	}
	m_ani_thread.Start();
}

void CTimeBarPage::OnMoveSilder(kn_int i_value)
{
	m_time_bar->setTimelineOneDayWidth(i_value);
	InvalidateView();
}

void CTimeBarPage::OnRefershSilder()
{
	m_silder_bar->SetValue(m_time_bar->getTimelineOneDayWidth());
	InvalidateView();
}

void CTimeBarPage::OnPrjDClick(kn_int , kn_int , KMessageMouse*)
{
	showWholeProject();
}

void CTimeBarPage::onKeyUp(KMessageKey* k)
{
	if ( k->m_key_id == 'A')
	{
		//		switchPage(STORYBOARD_PAGE);
		if (m_ani_thread.isPlaying())
		{
			return;
		}

		if (m_time_bar && m_silder_bar)
		{//移动到项目中间
			showWholeProject();

		}


	}

	if ( k->m_key_id == 'Z')
	{
		setMode(!m_mode);
	}
}

void  CTimeBarPage::showWholeProject()
{
	ptime start_t = m_prj_data->getProBeginTime() ;
	ptime end_t = m_prj_data->getProEndTime() ;

	int s_pos = m_time_bar->getTimePosition(start_t);
	int pos = s_pos + (m_time_bar->getTimePosition(end_t) - s_pos)/2 - m_time_bar->GetRect().width()/2;
	m_ani_thread.clearAnimation();
	m_ani_thread.addAnimationHelper(m_time_bar, PARA_ID_TimeBar, m_time_bar->getTimeLineOffset() - pos, 400,0,0 );
	m_ani_thread.setStopMsg(  TIMEBAR_PROJ_SCALE_END);
	m_ani_thread.Start();
}

void CTimeBarPage::exitView()
{


}

void CTimeBarPage::onAddTask(KView_PTR)
{
	if (m_action_bk->isShow())
	{
		if ( !hideActionEdit() )
		{
			return;
		}
	}

	CNProjectTask task;
	task.setDataPointer(m_prj_data);
	setNewTaskInfo(&task);
}
void CTimeBarPage::setNewTaskInfo(CNProjectTask* pTask)
{	
	NTaskSourceView_PTR task_dlg = NTaskSourceView_PTR(new NTaskSourceView( pTask ) );
	int w = GetScreen()->GetWidth();
	int h = GetScreen()->GetHeight();
	task_dlg->Create(RERect::MakeXYWH((w-800)/2, (h-620)/2, 800, 620));
	task_dlg->init();
	GetParent()->AddView(task_dlg);
	InvalidateView();
	task_dlg->setViewFocus();
	//zhic task 增加了缺省时间
//	pTask->setStartTime(  m_time_bar->getPositisonTime(50));
//	pTask->setEndTime(  m_time_bar->getPositisonTime(250));
	if (task_dlg->doModal() == 0)
	{
		m_prj_data->addTask( *pTask);

		addTask( &(*(m_lst_task->rbegin() ) ), 0 );

		moveToTask( m_task_btn.size());
	}
}
void CTimeBarPage::moveToTask(int index)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	int count = m_task_btn.size();
	if (count ==0) return;
	
	list<CTaskView_PTR>::iterator it_task_btn = m_task_btn.begin();
	int first_y = (*it_task_btn)->GetPosition().y();
	if (index <=0)
	{
	}
	else if( index >= count)
	{
		for (int i=0; i<count-1; i++) 	it_task_btn++;
	}
	else
	{
		for (int i=0; i<index-1; i++) 	it_task_btn++;
	}

	int offset = m_rect.height()/2 + 65 - (*it_task_btn)->GetPosition().y();

	first_y += offset;
	m_ani_thread.addAnimationHelper( shared_from_this(), PARA_ID_TimeBar_Y, first_y, 600, 0, 0 );
	m_ani_thread.Start();
}

void CTimeBarPage::onAddEvent(KView_PTR)
{
	CNProjectMileStone task;
	task.setName( getStringManager()->GetStringByID(_T("mile_stone")).c_str() );
	task.setTime(  m_time_bar->getPositisonTime( m_time_bar->GetRect().width()/2 ));
	m_prj_data->addMilestone( task);
	addEvent( &(*(m_prj_data->getMilestones().rbegin() ) ) ); 
	InvalidateView();
}

void CTimeBarPage::onEventPrev(KView_PTR p)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}	
	
	ptime pt ;

	CNProjectMileStone* p_ms = NULL;
	if (p->getName() == _T("forward"))
	{
		pt = m_time_bar->getPositisonTime( m_time_bar->GetRect().width()/2 +10);
		p_ms = m_prj_data->getNearMilestone(pt, 1);
	}
	else
	{
		pt = m_time_bar->getPositisonTime( m_time_bar->GetRect().width()/2 -10);
		p_ms = m_prj_data->getNearMilestone(pt, 0);
	}
	 
	if (p_ms == NULL)
	{//没找到
		return ;
	}

	//移动
	ptime start_t = p_ms->getTime() ;
	int pos = m_time_bar->getTimePosition(start_t)- m_time_bar->GetRect().width()/2;
	m_ani_thread.clearAnimation();
	m_ani_thread.addAnimationHelper(m_time_bar, PARA_ID_TimeBar, m_time_bar->getTimeLineOffset() - pos, 500,0,0 );
	m_ani_thread.Start();
}

void CTimeBarPage::onEventForward(KView_PTR)
{

	InvalidateView();
}

void CTimeBarPage::onDelTask(CTaskView_PTR p)
{//删除一个任务很复杂
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	if (m_action_bk->isShow())
	{
		hideActionEdit();
		return;
	}

	CTaskView_PTR task_view = VIEW_PTR(CTaskView)(p);
	//先找到taskview在列表中的位置，taskbarview和task都根据位置来删除
	int pos =0;
	bool b_find = FALSE;
	for( list<CTaskView_PTR>::iterator it = m_task_btn.begin(); it != m_task_btn.end(); it++ )
	{
		if (task_view == (*it))
		{
			b_find = TRUE;
			break;
		}
		pos++;
	}

	if (!b_find)
	{
		return;
	}


	//// del task btn
	AddViewToDel(task_view );

	list<CTaskView_PTR>::iterator it_task_btn = m_task_btn.begin();
	for (int i=0; i<pos; i++) 	it_task_btn++;
	//注意，执行后it_task_bar 为下一个有效值，用于后续动画
	it_task_btn = m_task_btn.erase( it_task_btn );

	//////// del task bar
	list<CTaskBarView_PTR>::iterator it_task_bar = m_task_bar.begin();
	for (int i=0; i<pos; i++) 	it_task_bar++;

	AddViewToDel( *it_task_bar );
	//注意，执行后it_task_bar 为下一个有效值，用于后续动画
	it_task_bar = m_task_bar.erase( it_task_bar );

	///// del task data
	CNProjectTask_LST &lst_task = m_prj_data->getTasks();
	CNProjectTask_LST::iterator it_task = lst_task.begin();
	for (int i=0; i<pos; i++) 	it_task++;
	lst_task.erase( it_task );

	//下面的task上移，动画过程
	//构造动画
	if (it_task_bar == m_task_bar.end())
	{
		return;
	}
	int i = 0;
	for ( it_task_bar ; it_task_bar != m_task_bar.end(); it_task_bar++ )
	{
		(*it_task_bar)->refreshInfo();
		CNUIAnimation* pNUIAnimation = new CNUIAnimation (200+i*100, 200, 0);
		CPropertyPos* pPosDst = new CPropertyPos((*it_task_bar), (*it_task_bar)->GetPosition().x(), (*it_task_bar)->GetPosition().y()- m_task_height);
		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPosDst);
		pNUIAnimation->addAnimationData(pAnimationData);
		m_ani_thread.addAnimation(pNUIAnimation);
		i++;
	}

	i=0;
	for ( it_task_btn ; it_task_btn != m_task_btn.end(); it_task_btn++ )
	{
		(*it_task_btn)->refreshInfo();
		CNUIAnimation* pNUIAnimation = new CNUIAnimation (200+i*100, 200, 0);
		CPropertyPos* pPos = new CPropertyPos((*it_task_btn), (*it_task_btn)->GetPosition().x(), (*it_task_btn)->GetPosition().y()- m_task_height );
		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPos);
		pNUIAnimation->addAnimationData(pAnimationData);
		m_ani_thread.addAnimation(pNUIAnimation);
		i++;
	}
	m_ani_thread.Start();

}

void CTimeBarPage::delMileStone( KView_PTR p )
{
	if (p)
	{
		CNProjectMileStone_LST&  lst_mile_stone = m_prj_data->getMilestones();
		for (CNProjectMileStone_LST::iterator it = lst_mile_stone.begin();
			it != lst_mile_stone.end() ; it++)
		{
			if ((*it).getId() == ( VIEW_PTR(CEventView)(p))->getMileStone()->getId())
			{
				lst_mile_stone.erase(it);
				AddViewToDel(p);
				list<CEventView_PTR>::iterator it = std::find(m_lst_event.begin(),m_lst_event.end(),p);
				if (it != m_lst_event.end())
				{
					m_lst_event.erase(it);	
				}
				break;
			}
		}
		InvalidateView();
	}
}


kn_bool  CTimeBarPage::isChange(kn_int para_id, kn_double v)
{
	if (para_id == PARA_ID_TimeBar_Y)
	{
		kn_int i_v = v;

		return ( getTaskY() != i_v);
	}

	return KView::isChange(para_id, v);
}

void  CTimeBarPage::doChange(kn_int para_id, kn_double v)
{
	KView::doChange(para_id, v);
	if (para_id == PARA_ID_TimeBar_Y)
	{
		kn_int i_v = v;
		setTaskY(i_v);
	}
}

kn_double  CTimeBarPage::getParaValue(kn_int para_id)
{
	if (para_id == PARA_ID_TimeBar_Y)
	{
		return getTaskY();
	}
	return KView::getParaValue(para_id);
}

kn_bool  CTimeBarPage::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == TIMEPAGE_TASK_EDIT_ANI_HIDE)
	{
		if (m_action_bk->isShow())
		{	
			m_action_bk->SetShow(FALSE);
			std::list<CActionBarView_PTR>::iterator it_action;
			for ( it_action = m_action_bar.begin(); it_action != m_action_bar.end(); it_action++ )
			{
				AddViewToDel(  (*it_action) );
			}
			m_action_bar.clear();
			m_ani_thread.setStopMsg(-1);
		}
		return TRUE;
	}

	if (pMsg->m_msg_type == TIMEBAR_PROJ_SCALE_END)
	{//缩放
		ptime start_t = m_prj_data->getProBeginTime() ;
		ptime end_t = m_prj_data->getProEndTime() ;
		UINT tw = m_time_bar->getTimeDurationWidth(start_t, end_t);
		if (tw != m_time_bar->getTimelineOneDayWidth() )
		{
			m_silder_bar->SetValue(tw);
			m_ani_thread.clearAnimation();
			m_ani_thread.addAnimationHelper(m_time_bar, PARA_ID_TimeBar_Scale, tw, 500,0,0 );
			m_ani_thread.setStopMsg(-1);
			m_ani_thread.Start();
		}


	}
	return FALSE;
}

void CTimeBarPage::updateModifyData()
{
	if (m_action_bk->isShow())
	{	
		std::list<CActionBarView_PTR>::iterator it_action;
		for ( it_action = m_action_bar.begin(); it_action != m_action_bar.end(); it_action++ )
		{
			(*it_action)->refreshInfo();
			(*it_action)->syncTimeline(m_time_bar);
		}
	}
}


void CTimeBarPage::onFoldClick(KView_PTR)
{
	setMode(FALSE);
}

void CTimeBarPage::onUnfoldClick(KView_PTR)
{
	setMode(TRUE);
}


void CTimeBarPage::OnGesture(KGesture* pGesture)
{
	//MyTraceA("OnGesture OnGesture pGesture type %d, span %.2f", pGesture->m_type, pGesture->m_zoom_span);

	 
	static int s_iStartWide = 0;
	static float s_fStartPointDistance = 1;
	static float s_fLastPointSDitance = 0;

	const float MOVE_STEP_DIS = 10;

	if(pGesture->m_type == GESTURE_ZOOM_BEGIN) 
	{
		s_iStartWide = m_time_bar->getTimelineOneDayWidth(); 
		s_fStartPointDistance = pGesture->m_zoom_span;
		if(s_fStartPointDistance < 1.0f)
		{
			s_fStartPointDistance = 1.0f;
		}

		s_fLastPointSDitance = s_fStartPointDistance;
	}
	else if(pGesture->m_type == GESTURE_ZOOM || pGesture->m_type == GESTURE_ZOOM_END)
	{
		//	避免频繁缩放
		if(fabs (s_fLastPointSDitance - pGesture->m_zoom_span) > MOVE_STEP_DIS )
		{
			float fWide = s_iStartWide * pGesture->m_zoom_span / s_fStartPointDistance;
			m_time_bar->setTimelineOneDayWidth((int)fWide, true);

			s_fLastPointSDitance = pGesture->m_zoom_span;

		}
	}
// 	else if(pGesture->m_type == GESTURE_ZOOM_END)
// 	{
// 		float fWide = s_iStartWide * pGesture->m_zoom_span / s_fStartPointDistance;
// 		m_time_bar->setTimelineOneDayWidth((int)fWide, true);
// 	}
	 
	 

}

void CTimeBarPage::onDragDirect( KMessageDrag* mag )
{
	if (mag->m_drag_type == TASK_VIEW_DRAG)
	{
		OnTaskBtnDrag(mag);	
	}
	else if (mag->m_drag_type == TASK_VIEW_DRAG_UP)
	{
		OnTaskBtnDragUp(mag);
	}
	else
	{
		return;
	}
	
}

void CTimeBarPage::setTaskDragDirection( TASK_DRAG_DIR eDrag_dir )
{
	if (eDrag_dir != m_drag_dir)
	{
		m_drag_dir = eDrag_dir;
	}
}

void CTimeBarPage::setViewPositionY(KView_PTR pView, kn_int iPositionY)
{
	if (pView)
	{
		CPropertyPos* pPos = new CPropertyPos(pView, pView->GetRect().left(), iPositionY);
		GetScreen()->addProperty(pPos);
	}
}

void CTimeBarPage::updateTaskBarAndDataList()
{
	// 处理task bar
	list<CTaskBarView_PTR>::iterator it_task_bar = m_task_bar.begin();
	list<CTaskBarView_PTR>::iterator next_task_bar = m_task_bar.begin();
	for (int i=0; i<m_nDragViewIndex; i++) 	{it_task_bar++;next_task_bar++;}
	list<CTaskBarView_PTR> temp_task_bar_list;
	temp_task_bar_list.splice(temp_task_bar_list.begin(), m_task_bar, it_task_bar, ++next_task_bar);
	list<CTaskBarView_PTR>::iterator insert_task_bar = m_task_bar.begin();
	for (int i=0; i<m_nDragChangeIndex; i++) 	insert_task_bar++;
	m_task_bar.splice(insert_task_bar, temp_task_bar_list);
	temp_task_bar_list.clear();

	// 处理task data
	CNProjectTask_LST& lst_task = m_prj_data->getTasks();
	CNProjectTask_LST::iterator it_task_data = lst_task.begin();
	CNProjectTask_LST::iterator next_task_data = lst_task.begin();
	for (int i=0; i<m_nDragViewIndex; i++) 	{it_task_data++;next_task_data++;}
	CNProjectTask_LST temp_task_data_list;
	temp_task_data_list.splice(temp_task_data_list.begin(), lst_task, it_task_data, ++next_task_data);
	CNProjectTask_LST::iterator insert_task_data = lst_task.begin();
	for (int i=0; i<m_nDragChangeIndex; i++) 	insert_task_data++;
	lst_task.splice(insert_task_data, temp_task_data_list);
	temp_task_data_list.clear();
	m_prj_data->setModify(true);
}

void CTimeBarPage::aniMoveAllTask()
{
	while (m_ani_thread.isPlaying())
	{
		Sleep(50);
	}
	if (!m_mode)
	{
		// 改变在lst_view位置，避免收缩的时候出现压盖
		list<CTaskView_PTR>::iterator it_task_btn;
		for ( it_task_btn = m_task_btn.begin(); it_task_btn != m_task_btn.end(); it_task_btn++ )
		{
			changeViewLayerTop(*it_task_btn);
		}
	}
	// tast btn位置调整
	kn_int i=0;
	list<CTaskView_PTR>::iterator it_task_btn;
	for ( it_task_btn = m_task_btn.begin(); it_task_btn != m_task_btn.end(); it_task_btn++ )
	{
		(*it_task_btn)->refreshInfo();
		m_ani_thread.addAnimationHelper( (*it_task_btn), PropertyPARA_ID_POS_Y, m_task_y + i*m_task_height + m_nWheelOffset, 100,200, 0 );
		i++;
	}

	// tast bar位置调整
	i =0;
	list<CTaskBarView_PTR>::iterator it_task_bar;
	for ( it_task_bar = m_task_bar.begin(); it_task_bar != m_task_bar.end(); it_task_bar++ )
	{
		(*it_task_bar)->refreshInfo();
		m_ani_thread.addAnimationHelper( (*it_task_bar), PropertyPARA_ID_POS_Y, m_task_y + i*m_task_height + m_nWheelOffset, 100,200, 0 );
		i++;
	}
	m_ani_thread.Start();
}

void CTimeBarPage::OnTaskBtnDrag( KMessageDrag* mag )
{
	if (m_action_bk->isShow())
	{
		hideActionEdit();
		return;
	}
	// 改变drag view的位置
	CTaskView_PTR task_view = VIEW_PTR(CTaskView)(mag->m_p_drag_view);
	setViewPositionY(task_view, mag->m_pos_y);
	// 刷新界面
	InvalidateView();

	// 调整其他task btn的位置
	// 如果动画正在进行则返回
	while (m_ani_thread.isPlaying())
	{       
		Sleep(10);
	}
	// 设置拖拽的方向
	setTaskDragDirection((TASK_DRAG_DIR)mag->m_wParam);
	// 根据拖拽的位置调整task btn列表
	updateTaskBtnList(task_view, task_view->getTaskID(), mag->m_pos_y);
	// 将task btn移动到调整后的位置
	aniMoveTaskBtn(task_view);
}

void CTimeBarPage::OnTaskBtnDragUp( KMessageDrag* mag )
{
	// 将task btn的修改同步到task bar 和 task data中
	updateTaskBarAndDataList();
	// 调整task btn和task bar的位置
	aniMoveAllTask();
	// 重置成员变量
	m_drag_dir = DRAG_UNKNOWN;
	m_nDragViewIndex = -1;
	m_nDragChangeIndex = -1;

}

void CTimeBarPage::updateTaskBtnList(CTaskView_PTR task_view, kn_int drag_id, kn_int drag_y_pos)
{
	if (m_nDragViewIndex != drag_id)
	{
		m_nDragViewIndex = drag_id;
		m_nDragChangeIndex = m_nDragViewIndex;
	}
	kn_int	insert_pos = -m_nWheelOffset/m_task_height;
	if (drag_y_pos > (m_task_y+ m_nWheelOffset))
	{
		insert_pos = (drag_y_pos - (m_task_y + m_nWheelOffset))/m_task_height + 1;
	}
	if (insert_pos > m_task_btn.size())
	{
		insert_pos = m_task_btn.size();
	}
	if ((insert_pos != m_nDragChangeIndex) && (insert_pos != (m_nDragChangeIndex + 1)))
	{
		if (m_drag_dir == DRAG_DOWNWARD)
		{
			changeTastBtnList(m_nDragChangeIndex, insert_pos -1);
			m_nDragChangeIndex = insert_pos - 1;
		}
		else if (m_drag_dir == DRAG_UPWARD)
		{
			changeTastBtnList(m_nDragChangeIndex, insert_pos);
			m_nDragChangeIndex = insert_pos;
		}
	}
}

void CTimeBarPage::aniMoveTaskBtn(CTaskView_PTR task_view)
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	kn_int i =0;
	kn_bool bPlay = false;
	list<CTaskView_PTR>::iterator it_task_btn;
	for ( it_task_btn = m_task_btn.begin(); it_task_btn != m_task_btn.end(); it_task_btn++ )
	{
		// 当前拖拽的view不移动
		if ((*it_task_btn) == task_view)
		{
			++i;
			continue;
		}
		kn_int it_task_pos = m_task_y + i*m_task_height + m_nWheelOffset;
		if((*it_task_btn)->GetPosition().y() != it_task_pos  &&  
			( (*it_task_btn)->GetPosition().y() == it_task_pos - m_task_height ||  (*it_task_btn)->GetPosition().y() == it_task_pos + m_task_height ) )
		{
			m_ani_thread.addAnimationHelper(*it_task_btn, PropertyPARA_ID_POS_Y, it_task_pos, 100, 0, 0, KEasingCurve::InCirc);
			bPlay = true;
		}
		++i;
	}
	if (bPlay)
	{
		m_ani_thread.Start();
	}
}

void CTimeBarPage::changeTastBtnList( kn_int old_id, kn_int new_id )
{
	list<CTaskView_PTR>::iterator it_task_btn = m_task_btn.begin();
	list<CTaskView_PTR>::iterator next_task_btn = m_task_btn.begin();
	for (int i=0; i<old_id; i++) 	{it_task_btn++;next_task_btn++;}
	list<CTaskView_PTR> temp_task_list;
	temp_task_list.splice(temp_task_list.begin(), m_task_btn, it_task_btn, ++next_task_btn);
	list<CTaskView_PTR>::iterator insert_task_btn = m_task_btn.begin();
	for (int i=0; i<new_id; i++) 	insert_task_btn++;
	m_task_btn.splice(insert_task_btn, temp_task_list);
	temp_task_list.clear();
}

void CTimeBarPage::Release()
{
	KViewGroup::Release();
	m_task_bar.clear();
	m_task_btn.clear();
	m_lst_event.clear();
	m_action_bar.clear();
	m_prj_data = NULL;
	m_lst_task = NULL;
}
