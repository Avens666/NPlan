#include "NCalendarDayView.h"
#include "boost/lexical_cast.hpp"
#include "KFontManager.h"
#include "KShapeDrawable.h"
#include "Calendar.h"
#include "KScreen.h"
#include "../NPlan.h"
#include "NMenu.h"
using namespace std;
using namespace boost;
using namespace boost::gregorian;

NCalendarDayView::NCalendarDayView(  )
{

}

NCalendarDayView::~NCalendarDayView(void)
{
}


void NCalendarDayView::init(  )
{
	m_month_h = 40;
	K9PatchImageDrawable_PTR p  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/calendar_bk.9.png")),true));
	p->setRect(0,0,m_rect.width(),m_rect.height() );
	addDrawable(p);

	m_txt_month_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), ColorSetARGB(255, 240, 240, 240), 26, REPaint::kCenter_Align) );
	m_txt_month_drawable->setRect(0,0, m_rect.width(), m_month_h+8);
	m_txt_month_drawable->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	addDrawable(m_txt_month_drawable);
	
	m_txt_day_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), ColorSetARGB(255, 30, 30, 30), 100, REPaint::kCenter_Align) );
	m_txt_day_drawable->setFont(GetFontManagerSingleton()->GetFontFromFile(_T("./resource/utsaahb.ttf") ));
	m_txt_day_drawable->setRect(0, m_month_h, m_rect.width(), m_rect.height()- m_month_h*1.5);
	addDrawable(m_txt_day_drawable);

	m_txt_week_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), ColorSetARGB(255, 42, 75, 112), 22, REPaint::kCenter_Align) );
	m_txt_week_drawable->setRect(0, m_rect.height() - m_month_h*0.9, m_rect.width(), m_month_h*0.8);
	m_txt_week_drawable->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	addDrawable(m_txt_week_drawable);
}

extern kn_string getDayOfWeekAll(const date& d);
extern kn_string getMonthString(const date &d);
extern kn_string getDayOfWeek(date& d);

void NCalendarDayView::setDate(const date& d )
{
	m_date = d;
	writeLock lock(m_lst_drawable_mutex);
	m_txt_month_drawable->SetText( getMonthString(d) );
	m_txt_week_drawable->SetText( getDayOfWeekAll(d) );
	m_txt_day_drawable->SetText( lexical_cast<kn_string>(d.day() ) );
	

}

date NCalendarDayView::getDate()
{
	return m_date;
}

//////////////////// NProjectDayView /////////////////////////////////



NProjectDayView::NProjectDayView(  )
{
	m_prj_data = NULL;
	m_b_in_month = TRUE;
	m_month_h = 30;
	m_date = date(2014,10,1);
}

NProjectDayView::~NProjectDayView(void)
{
}

void NProjectDayView::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_down.connect(this, &NProjectDayView::click );
	m_sign_rup.connect(this, &NProjectDayView::rclick );
}

void NProjectDayView::init(CNProjectData* p )
{
	m_prj_data = p;
	
	RERect rect = RERect::MakeXYWH(0,0, m_rect.width(), m_rect.height());

	KRectShape* p_rect_shape = new KRectShape(rect);
	REColor clr = ColorSetARGB(255, 238,238,238) ;
	p_rect_shape->setFillColor(clr);
	clr = ColorSetARGB(255, 120,120,120);
	p_rect_shape->setStrokeColor(clr );
	p_rect_shape->setStrokeWidth(2);
	p_rect_shape->initPaint();
	m_bk_drawable  = KShapeDrawable_PTR(new KShapeDrawable(p_rect_shape));
	m_bk_drawable->setRect(0,0,m_rect.width(),m_rect.height() );
	m_bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_bk_drawable->setAutoSize(TRUE);
	addDrawable(m_bk_drawable);

	m_txt_day_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), ColorSetARGB(255, 30, 30, 30), 70, REPaint::kCenter_Align) );
	m_txt_day_drawable->setFont(GetFontManagerSingleton()->GetFontFromFile(_T("./resource/utsaahb.ttf") ));
	m_txt_day_drawable->setRect(0, 0, m_rect.width(), m_rect.height());
	m_txt_day_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_txt_day_drawable->setAutoSize(TRUE);
	addDrawable(m_txt_day_drawable);



}

void NProjectDayView::click(kn_int x, kn_int y,KMessageMouse* p)
{

	KMessageDrag* msg = new KMessageDrag;
	msg->m_p_drag_view = shared_from_this();

	GetScreen()->sendNUIMessage( CALENDAR_DAY_CLICK, msg );
}

void NProjectDayView::rclick(kn_int x, kn_int y,KMessageMouse* p)
{
	CMenuDaySet_PTR menu = CMenuDaySet_PTR(new CMenuDaySet );
	int day_type;
	if ( m_prj_data->getCalendar().isWorkDay(m_date) )
	{
		day_type = DAY_TYPE_WORKDAY;
	}
	else
	{
		day_type = DAY_TYPE_HOLIDAY;
	}
	menu->Create(0,0, 120, 40);
	//注意传入点击的taskview
	menu->init(shared_from_this(), day_type );
	menu->m_sign_menu_view_click.connect(this, &NProjectDayView::onDayMenu);
	//showmenu 传入view自身的相对坐标，这里取鼠标点击位置
	showMenu(menu,  x - m_rect.x(), y - m_rect.y());
	InvalidateView();
}

void NProjectDayView::onDayMenu(KView_PTR v, kn_int n)
{
	//NProjectDayView_PTR day_view = VIEW_PTR(NProjectDayView)(v) ;
	//time_duration td1(duration_from_string("08:30:00.000")); 
	//time_duration td2(duration_from_string("18:00:00.000")); 
	if (n == DAY_TYPE_WORKDAY)
	{
		m_prj_data->getCalendar().setWorkDay(m_date);
	}
	else
	{
		m_prj_data->getCalendar().setHoliday(m_date);
	}

	updateColor();
}

void NProjectDayView::setDate( date& d )
{
	m_date = d;
	writeLock lock(m_lst_drawable_mutex);
	m_txt_day_drawable->SetText( lexical_cast<kn_string>(d.day() ) );
	m_txt_day_drawable->SetFontSize(70);
	m_txt_day_drawable->setRect(0,0, m_rect.width(), m_rect.height());
	lock.unlock();
	ptime pt(second_clock::local_time()); 
	if ( d == pt.date() )
	{//当天日期
		if (!m_today_drawable)
		{
			KRectShape* p_rect_shape = new KRectShape(m_rect);
			REColor clr = ColorSetARGB(255, 69,123,185) ;
			p_rect_shape->setStrokeColor(clr );
			p_rect_shape->setFillOpacity(0);
			p_rect_shape->setStrokeWidth(10);
			p_rect_shape->initPaint();
			m_today_drawable  = KShapeDrawable_PTR(new KShapeDrawable(p_rect_shape));
			m_today_drawable->setRect(0,0,m_rect.width(),m_rect.height() );
			m_today_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
			m_today_drawable->setAutoSize(TRUE);
			addDrawable(m_today_drawable);
		}
		m_today_drawable->SetShow(TRUE);
	}
	else
	{
		if(m_today_drawable) m_today_drawable->SetShow(FALSE);
	}

	if ( getConfigManager()->getLanguage() == LAN_CN)
	{//中国版显示农历
		if (!m_chinese_day_drawable)
		{
			m_chinese_day_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), ColorSetARGB(255, 110, 110, 110), 16, REPaint::kCenter_Align) );
			m_chinese_day_drawable->setFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
			m_chinese_day_drawable->setRect(0, m_rect.height() - 25, m_rect.width(), 25);
			m_chinese_day_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
			m_chinese_day_drawable->setAutoSize(TRUE);	
			addDrawable(m_chinese_day_drawable);
		}
		Calendar cal;
		m_chinese_day_drawable->SetText(cal.SolarToLunar( d.year(),d.month(),d.day()));
		m_chinese_day_drawable->SetShow(TRUE);
	}

	updatePrjInfo();
	updateColor();
}

void NProjectDayView::updatePrjInfo()
{
	//注意这里释放了所有的view，如果增加非m_prj_info_view_list 的view成员要注意处理
	KViewGroup::Release();
	m_prj_info_view_list.clear();

	m_mode = getConfigManager()->getIntConfig(_T("calendar_mode"));

	if (m_mode == 0)
	{//常规日历
		return;
	}
	// 初始化项目信息
	//过滤里程碑
	KImageDrawable_PTR icon;
	CNProjectMileStone_LST& ms = m_prj_data->getMilestones();
	CNProjectMileStone_LST::iterator it;
	int count = 0;
	for ( it = ms.begin(); it != ms.end(); ++it )
	{
		if ( (*it).getTime().date() == m_date )
		{
			icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/calendar_star.png")), TRUE) );
		}
		else
		{
			continue;
		}

		KStaticView_PTR view = KStaticView_PTR(new KStaticView);

		view->Create(4, count*14+24, m_rect.width()-8, 14);
		icon->setRect(0,0, 14, 14);
		view->setIconDrawable(icon);
		view->SetTextBound( RERect::MakeXYWH(16, 0, m_rect.width(), 13 ) );
		view->SetFontSize(12);
        view->setTextAlign(REPaint::kLeft_Align);
		REColor clr = SkColorSetARGBInline(255,30,30,30);
		view->SetTextColor(clr);
		view->SetText( (*it).getName() );
		AddView(view);
		m_prj_info_view_list.push_back(view);
//		view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		count++;
	}

	//过滤任务卡
	TaskCard_LST p_v_task = m_prj_data->getActionGroupByDate(m_date, m_mode);

	TaskCard_LST::iterator iter;
	kn_string task_str;
	for ( iter = p_v_task.begin(); iter != p_v_task.end(); ++iter )
	{
		if ( (*iter)->getStartTime().date() == m_date )
		{
			icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/calendar_start.png")), TRUE) );
		}
		else
		{
			icon = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/calendar_stop.png")), TRUE) );
		}

		KStaticView_PTR view = KStaticView_PTR(new KStaticView);

		view->Create(4, count*14+24, m_rect.width()-8, 14);
		icon->setRect(0,0, 13, 13);
		view->setIconDrawable(icon);
		view->SetTextBound( RERect::MakeXYWH(16, 0, m_rect.width(), 13 ) );
		view->SetFontSize(12);
        view->setTextAlign(REPaint::kLeft_Align);
		REColor clr = SkColorSetARGBInline(255,30,30,30);
		view->SetTextColor(clr);
		task_str = (*iter)->getLeaderName();
		task_str += _T(" ");
		task_str += (*iter)->getName();
		view->SetText( task_str );
		AddView(view);
		m_prj_info_view_list.push_back(view);
		view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		count++;
	}

	updatePrjInfoPos();

}

//仅仅计算排版位置
void NProjectDayView::updatePrjInfoPos()
{
	//根据数量计算排版
	int num_task_info = m_prj_info_view_list.size();

	if (num_task_info == 0)
	{
		return;
	}
	if (m_chinese_day_drawable)
	{
		m_chinese_day_drawable->SetShow(FALSE);
	}

	int big_size_row =  (m_rect.height()/2 - 18 )/14; //normal状态字符，底部最多容纳行数
	int max_row =  (m_rect.height() -30 )/15; //当前位置最多容纳行数

	int fontsize = (m_rect.height() - num_task_info*15);
	int info_y = 0; //信息起始位置
	if (num_task_info <= big_size_row)
	{
		//日期维持标准模式不变
		m_txt_day_drawable->SetFontSize(70);
		m_txt_day_drawable->setRect(0,0, m_rect.width(), m_rect.height());
		info_y = m_rect.height()/2 + 18;
	}
	else if (num_task_info <= max_row)
	{ //日期变小，字体能够全部容纳
        int fontsize = min((REScalar)70, (m_rect.height() - num_task_info*15) );
		m_txt_day_drawable->setRect(0,0, m_rect.width(),fontsize);
		m_txt_day_drawable->SetFontSize(fontsize);
		info_y = fontsize;
	}
	else
	{
		m_txt_day_drawable->setRect(0,0, m_rect.width(),34);
		m_txt_day_drawable->SetFontSize(30);
		info_y = 25;
	}
	
	LST_PRJINFO_VIEW::iterator iter;

	int count = 0;
	for ( iter = m_prj_info_view_list.begin(); iter != m_prj_info_view_list.end(); ++iter )
	{
		(*iter)->setRect(4,info_y + count*15,  m_rect.width(),15);
		count++;
	}

}

void NProjectDayView::viewSizeChange()
{
	KViewGroup::viewSizeChange();

	//重新计算尺寸
	updatePrjInfoPos();
}

date NProjectDayView::getDate()
{
	return m_date;
}

void NProjectDayView::setInMonth(bool b)
{
	m_b_in_month = b;
	updateColor();
}

void  NProjectDayView::updateColor()
{
	if (m_b_in_month)
	{
		m_txt_day_drawable->setColor( ColorSetARGB(255,30,30,30) );
		if (m_prj_data)
		{
			if ( !m_prj_data->getCalendar().isWorkDay(m_date))
			{
				m_txt_day_drawable->setColor( ColorSetARGB(255, 223,45,45) );
			}
		}
	}
	else
	{
		m_txt_day_drawable->setColor( ColorSetARGB(255,150,150,150) );
		if (m_prj_data)
		{
			if ( !m_prj_data->getCalendar().isWorkDay(m_date))
			{
				m_txt_day_drawable->setColor( ColorSetARGB(255, 240,150,150) );
			}
		}
	}
}
//////////////////// NDayGroupView /////////////////////////////////
NDayGroupView::NDayGroupView(  )
{
		m_prj_data = NULL;
}

NDayGroupView::~NDayGroupView(void)
{
}

void NDayGroupView::init( CNProjectData* p )
{
	m_prj_data = p;
	Release();
	int left = 3;
	int right = 3;
	int w = (m_rect.width() - 6-left - right)/7; //第一个6 间隔， 第二个6 两边预留
	int h = (m_rect.height() - 5)/6; 
	int i,j, n;
	for (i=0; i < 6; i++)
	{
		for (j=0; j < 7; j++)
		{
			n = i*7+j;
			m_day_group[n] = NProjectDayView_PTR(new NProjectDayView);
			m_day_group[n]->Create(left+j*(w+1), i*(h+1),w,h);
			m_day_group[n]->init(m_prj_data);
			m_day_group[n]->SetClip( RERect::MakeXYWH( left+j*(w+1)-1, i*(h+1)-1,w+1,h+1) );
			AddView(m_day_group[n]);
		}
	}

//	for (j=12; j < 42; j++) m_day_group[j]->SetShow(FALSE);
}

void NDayGroupView::setMonth(const date& d )
{
	m_date = d;
	writeLock lock(m_lst_drawable_mutex);

	months m(1);
	date dy1 = date(d.year(), d.month(), 1);//每月一号
	date dy2 = dy1 + m;
	std::string  s =to_simple_string(dy1);
	std::string  s2 =to_simple_string(dy2);
	days td(7);
	//一号是星期几
	int week_n = dy1.day_of_week();
	if (week_n == 0 ) //周日放最前面处理
	{
		td = days( 7 );
	}
	else
	{
		td = days( week_n );
	}

	date t_d = dy1 - td;;
	days dd1(1); 
	for (int n=0; n < 42; n++)
	{
		if (m_day_group[n])
		{
			m_day_group[n]->setDate( t_d );

			if (t_d < dy1 || t_d >= dy2)
			{//不在本月内
				m_day_group[n]->setInMonth(FALSE);
			}
			else
			{
				m_day_group[n]->setInMonth(TRUE);
			}
		}

		t_d += dd1;
	}
}

void NDayGroupView::viewSizeChange()
{
	KViewGroup::viewSizeChange();
	int left = 3;
	int right = 3;
	int w = (m_rect.width() - 6-left - right)/7; //第一个6 间隔， 第二个6 两边预留
	int h = (m_rect.height() - 5)/6; 
	int i,j, n;
	for (i=0; i < 6; i++)
	{
		for (j=0; j < 7; j++)
		{
			n = i*7+j;
			m_day_group[n]->setRect(left+j*(w+1), i*(h+1),w,h);
		}
	}

}

date NDayGroupView::getDate()
{
	return m_date;
}

//////////////////////////  NCalendarView  ////////////////////////

NCalendarView::NCalendarView(  )
{
	m_prj_data = NULL;
}

NCalendarView::~NCalendarView(void)
{
}


void NCalendarView::init( CNProjectData* prj)
{
	m_prj_data = prj;
	m_month_h = 40;
	m_week_h = 30;
	K9PatchImageDrawable_PTR p  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/calendar_big_bk.9.png")),true));
	p->setRect(0,0,m_rect.width(),m_rect.height() );
	p->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	addDrawable(p);

	m_txt_month_view = KTextView_PTR (new KTextView );
	m_txt_month_view->setRect(m_rect.width()/2-80, 0, 160, m_month_h);
	REColor clr1 = ColorSetARGB(255, 250, 250, 250);
	REColor clr2 = ColorSetARGB(255, 250, 250, 50);
	m_txt_month_view->SetTextColor(clr1, clr2,clr2,clr2);
	m_txt_month_view->setStateChangeEnable(true);
	m_txt_month_view->SetTextBound(RERect::MakeXYWH(0,0, 160, m_month_h));
	m_txt_month_view->GetTextPaint()->setTextSize(26);
	m_txt_month_view->GetTextPaint()->setTextAlign(SkPaint::kCenter_Align);
	m_txt_month_view->setCursorChangeEnable(true);
	m_txt_month_view->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	AddView(m_txt_month_view);

	int i=0;
	date week_day(2009,1,11); // 找个周日
	days day1(1);
	int week_w = m_rect.width()/7;
	for(i = 0; i < 7; i++)
	{
		m_txt_week[i] = KTextDrawable_PTR (new KTextDrawable(getDayOfWeek(week_day).c_str(), ColorSetARGB(255, 30, 30, 30), 22, REPaint::kCenter_Align) );
		m_txt_week[i]->setRect(i*week_w, m_month_h + 3, week_w, m_week_h);
		m_txt_week[i]->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei", RETypeface::kBold) );
		m_txt_week[i]->SetViewChangeType(KVIEW_LRMethod_Parent_LRNone, KVIEW_LRMethod_Parent_LRNone, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
		m_txt_week[i]->setAutoSize(TRUE);
		if (i == 0 || i == 6)
		{//周六周日哟
			m_txt_week[i]->setColor( ColorSetARGB(255, 240, 0, 0) );
		}
		addDrawable(m_txt_week[i]);
		week_day += day1;
	}

	m_month_view = NDayGroupView_PTR(new NDayGroupView);
	
	m_month_view->Create(0, m_month_h+m_week_h, m_rect.width(), m_rect.height() - m_month_h-m_week_h);
	m_month_view->SetClip( RERect::MakeXYWH( 0, m_month_h+m_week_h, m_rect.width(), m_rect.height() - m_month_h-m_week_h ) );
	m_month_view->init(m_prj_data);
	m_month_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	AddView(m_month_view);

	m_month_view_bak = NDayGroupView_PTR(new NDayGroupView);
	m_month_view_bak->Create(-m_rect.width(), m_month_h+m_week_h, m_rect.width(), m_rect.height() - m_month_h-m_week_h);
	m_month_view_bak->SetClip( RERect::MakeXYWH( 0, m_month_h+m_week_h, m_rect.width(), m_rect.height() - m_month_h-m_week_h ) );
	m_month_view_bak->init(m_prj_data);
	m_month_view_bak->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_month_view_bak->SetShow(FALSE);
	AddView(m_month_view_bak);
	
	m_txt_month_view->m_sign_up.connect(this, &NCalendarView::yearClick);
}

void NCalendarView::setMonth(const date& d )
{
	m_date = d;
	writeLock lock(m_lst_drawable_mutex);
	kn_string str = lexical_cast<kn_string>(d.year() ) ;
	str += _T("  ");
	m_txt_month_view->SetText(  str + getMonthString(d) );


	m_month_view->setMonth(d);
}

date  NCalendarView::getDate()
{
	return m_date;
}

void  NCalendarView::switchMonth(const date& d )
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	m_ani_thread.clearAnimation();



	date d2 = m_month_view->getDate();
	months m(1);
	date dy1 = date(d2.year(), d2.month(), 1);//每月一号
	date dy2 = dy1 + m;

	if (d < dy1)
	{
		m_month_view_bak->setRect(0, -m_rect.height(), m_rect.width(), m_rect.height() - m_month_h-m_week_h) ;
		m_ani_thread.addAnimationHelper(m_month_view, PropertyPARA_ID_POS_Y, 2*m_rect.height(), 350,0,0);
//		m_month_view_bak->setRect(-m_rect.width(), m_month_h+m_week_h, m_rect.width(), m_rect.height() - m_month_h-m_week_h) ;
//		m_ani_thread.addAnimationHelper(m_month_view, PropertyPARA_ID_POS_X, m_rect.width(), 350,0,0);
	}
	else if (d >= dy2)
	{
		m_month_view_bak->setRect(0, 2*m_rect.height(), m_rect.width(), m_rect.height() - m_month_h-m_week_h);
		m_ani_thread.addAnimationHelper(m_month_view, PropertyPARA_ID_POS_Y, -m_rect.height(), 350,0,0);
//		m_month_view_bak->setRect(m_rect.width()*2, m_month_h+m_week_h, m_rect.width(), m_rect.height() - m_month_h-m_week_h);
//		m_ani_thread.addAnimationHelper(m_month_view, PropertyPARA_ID_POS_X, -m_rect.width(), 350,0,0);
	}
	else
	{
		return;
	}

	m_ani_thread.addAnimationHelper(m_month_view_bak, PropertyPARA_ID_POS_Y,   m_month_h+m_week_h, 420,0,0);
//	m_ani_thread.addAnimationHelper(m_month_view_bak, PropertyPARA_ID_POS_X, 0, 500,0,0);

	writeLock lock(m_lst_drawable_mutex);
	kn_string str = lexical_cast<kn_string>(d.year() ) ;
	str += _T("  ");
	m_txt_month_view->SetText(  str + getMonthString(d) );
	m_month_view_bak->setMonth(d);

	m_month_view_bak->SetShow(TRUE);
	//交换位置
	NDayGroupView_PTR tmp = m_month_view_bak;
	m_month_view_bak = m_month_view;
	m_month_view = tmp;

	m_ani_thread.setStopMsg(SWITCH_MONTH_ANI_STOP);
	m_ani_thread.Start();
	m_date = d;
}

kn_bool  NCalendarView::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == SWITCH_MONTH_ANI_STOP)
	{
		m_month_view_bak->SetShow(FALSE);

		return TRUE;
	}

	return FALSE;
}

void NCalendarView::yearClick(kn_int, kn_int,KMessageMouse* )
{
	NYearSeleteView_PTR menu = NYearSeleteView_PTR(new NYearSeleteView );
	int w = 60+70*3-10;
	menu->Create(0,0, w, 20+40*3);
	//注意传入点击的taskview
	menu->init( m_date.year() );
	menu->m_sign_year_click.connect(this, &NCalendarView::onYearChange);
	//showmenu 传入view自身的相对坐标，这里取鼠标点击位置
	showMenu(menu, m_rect.width()/2 -w/2 , m_month_h);
	InvalidateView();	
}

void NCalendarView::onYearChange(int year)
{
	if (year != m_date.year())
	{
		date d = date(year,m_date.month(),m_date.day());
		switchMonth(d);
	}
}

///////////////// NYearSeleteView /////////////////////////
NYearSeleteView::NYearSeleteView()
{
	m_current_year=2014;
}

NYearSeleteView::~NYearSeleteView(void)
{

}

void NYearSeleteView::init(int year )
{
	m_current_year = year;

	
	KColorDrawable_PTR bk_drawable = KColorDrawable_PTR(new KColorDrawable(RE_ColorWHITE) );
	bk_drawable->setRect(0,0,m_rect.width(), m_rect.height());
	setBKDrawable(bk_drawable);

	
	int y =  m_current_year - 4;
	for (int i = 0; i < 9; i++)
	{
		m_txt_year1[i] = KTextView_PTR(new KTextView);

		m_txt_year1[i]->Create(i%3*70+30 , 10 + 40*(i/3), 64, 32);
		m_txt_year1[i]->SetFontSize(24);
		REColor clr = SkColorSetARGBInline(255,30,30,30);
		REColor clr_f = SkColorSetARGBInline(255,30,30,30);
		if (y == m_current_year)
		{
			clr = SkColorSetARGBInline(255,185,74,74);
			clr_f = SkColorSetARGBInline(255,255,0,0);
		}
		else
		{
			clr_f = SkColorSetARGBInline(255,69, 123, 185);
		}
		m_txt_year1[i]->SetTextColor(clr, clr_f, clr_f, clr_f);
		kn_string str = lexical_cast<kn_string>(y);
		m_txt_year1[i]->SetText( str );
		m_txt_year1[i]->setStateChangeEnable(true);
		m_txt_year1[i]->setCursorChangeEnable(true);
		m_txt_year1[i]->m_sign_up.connect(this,&NYearSeleteView::click);
		m_txt_year1[i]->SetClip( RERect::MakeXYWH( 30,0, m_rect.width()-60, m_rect.height()) );
		AddView(m_txt_year1[i]);
		y++;
	}
	m_current_id =1;

	for (int i = 0; i < 9; i++)
	{
		m_txt_year2[i] = KTextView_PTR(new KTextView);

		m_txt_year2[i]->Create(i%3*70+30 + m_rect.width() , 10 + 40*(i/3), 64, 32);
		m_txt_year2[i]->SetFontSize(24);
		REColor clr = SkColorSetARGBInline(255,30,30,30);
		REColor clr_f = SkColorSetARGBInline(255,69, 123, 185);
		m_txt_year2[i]->SetTextColor(clr, clr_f, clr_f, clr_f);
		m_txt_year2[i]->setStateChangeEnable(true);
		m_txt_year2[i]->setCursorChangeEnable(true);
		m_txt_year2[i]->m_sign_up.connect(this,&NYearSeleteView::click);
	//	m_txt_year2[i]->SetShow(FALSE);
		m_txt_year2[i]->SetClip(  RERect::MakeXYWH( 30,0, m_rect.width()-60, m_rect.height()) );
		AddView(m_txt_year2[i]);
		y++;
	}

	kn_string str = _T("./resource/actionbar_lmove_icon.png");
	createTextViewHelper(&m_btn_prev, str, 5, m_rect.height()/2 - 16 );
	m_btn_prev->m_sign_up.connect(this,&NYearSeleteView::prevClick);
	str = _T("./resource/actionbar_rmove_icon.png");
	createTextViewHelper(&m_btn_next, str, m_rect.width() - 20, m_rect.height()/2 - 16 );
	m_btn_next->m_sign_up.connect(this,&NYearSeleteView::nextClick);
}

void NYearSeleteView::switchYear(int dir)// 0 往后  1往前
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
	KTextView_PTR* src_v = m_txt_year1;
	KTextView_PTR* des_v = m_txt_year2;
	if (m_current_id == 2)
	{
		src_v = m_txt_year2;
		des_v = m_txt_year1;

		m_current_id = 1;
	}
	else
	{
		m_current_id = 2;
	}
	int x,y;
	m_ani_thread.clearAnimation();
	if (dir == 0)
	{
	  	y =  lexical_cast<int>( src_v[8]->getText() ) +1;
		for (int i = 0; i < 9; i++)
		{//移除src
			des_v[i]->SetText( lexical_cast<kn_string>(y) );
			REColor clr,clr_f ;
			if (y == m_current_year)
			{
				clr = SkColorSetARGBInline(255,185,74,74);
				clr_f = SkColorSetARGBInline(255,255,0,0);
			}
			else
			{
				clr = SkColorSetARGBInline(255,30,30,30);
				clr_f = SkColorSetARGBInline(255,69, 123, 185);
			}
			des_v[i]->SetTextColor(clr, clr_f, clr_f, clr_f);
			x = src_v[i]->GetRect().x() -m_rect.width();
			m_ani_thread.addAnimationHelper( src_v[i], PropertyPARA_ID_POS_X, x, 400, i*20,0);
			x = i%3*70+30 + m_rect.width();
			m_ani_thread.addAnimationHelper( des_v[i], PropertyPARA_ID_POS_X, x, i%3*70+30, 400, i*30,0);
			y++;
		}
	}
	else
	{
		y =  lexical_cast<int>( src_v[0]->getText() ) -9;
		for (int i = 0; i < 9; i++)
		{//移除src
			des_v[i]->SetText( lexical_cast<kn_string>(y) );
			REColor clr,clr_f ;
			if (y == m_current_year)
			{
				clr = SkColorSetARGBInline(255,185,74,74);
				clr_f = SkColorSetARGBInline(255,255,0,0);
			}
			else
			{
				clr = SkColorSetARGBInline(255,30,30,30);
				clr_f = SkColorSetARGBInline(255,69, 123, 185);
			}
			des_v[i]->SetTextColor(clr, clr_f, clr_f, clr_f);

			m_ani_thread.addAnimationHelper( src_v[i], PropertyPARA_ID_POS_X, src_v[i]->GetRect().x() +m_rect.width(), 400,0,0);
			m_ani_thread.addAnimationHelper( des_v[i], PropertyPARA_ID_POS_X, i%3*70+30 - m_rect.width(), i%3*70+30, 400,0,0);
			y++;
		}
	}

	m_ani_thread.Start();

}

void NYearSeleteView::click( kn_int, kn_int,KMessageMouse* p)
{
	kn_string str =  VIEW_PTR(KTextView)(p->m_p_view)->getText();
	int y = lexical_cast<int>(str);
	if (y != m_current_year)
	{
		m_sign_year_click.emit(y);
	}
}

void NYearSeleteView::prevClick( kn_int, kn_int,KMessageMouse* p)
{
	//不隐藏菜单
	switchYear(1);
	p->m_wParam = NO_HIDE_MENU;
}

void NYearSeleteView::nextClick( kn_int, kn_int,KMessageMouse* p)
{
	//不隐藏菜单
	switchYear(0);
	p->m_wParam = NO_HIDE_MENU;
}
