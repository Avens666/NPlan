#include "NTimeBarPage.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "boost/lexical_cast.hpp"
#include "NTaskSourceView.h"
#include "NMenu.h"
#include "NProjectPage.h"
using namespace boost;

CCalendarPage::CCalendarPage(void)
{
	m_e_viewtype = KVIEW_MOVEABLE;

	

}
void CCalendarPage::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &CCalendarPage::onKeyUp);
}

CCalendarPage::~CCalendarPage()
{
	m_ani_thread.Stop();

}

void CCalendarPage::init(CNProjectData* p)
{
	Release();
	m_prj_data = p;

	//顶部背景高度
	m_title_height = 100;
	m_calendar_height = 108;
//
	KStaticView_PTR titlebk_view = KStaticView_PTR(new KStaticView);
	titlebk_view->Create(0, 0, m_rect.width(), m_title_height );
	titlebk_view->setBKGColor( ColorSetARGB(255,69,123,185) );
	AddView(titlebk_view);
	titlebk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//page 背景
	KStaticView_PTR bk_view = KStaticView_PTR(new KStaticView);
	bk_view->Create(0, m_title_height, m_rect.width(), m_rect.height() - m_title_height);
	bk_view->setBKGColor( ColorSetARGB(255, 29, 43, 62) );
	bk_view->showBK(TRUE);
	AddView(bk_view);
	bk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	KStaticView_PTR titlebk_txt = KStaticView_PTR(new KStaticView);
	titlebk_txt->Create(m_rect.width()/2 - 100, 0, 300, 60 );
	titlebk_txt->SetText( _T("Project Calendar") );
	titlebk_txt->SetFontSize(36);
	titlebk_txt->SetTextColor(  ColorSetARGB(255,245,245,245)  );
	AddView(titlebk_txt);
	titlebk_txt->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_calendar_day= NCalendarDayView_PTR(new NCalendarDayView);;
	m_calendar_day->Create(13, m_calendar_height, 140, 146);
	m_calendar_day->init();
	ptime pt(second_clock::local_time());  
	m_calendar_day->setDate(pt.date());
	m_calendar_day->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	AddView(m_calendar_day);

	m_calendar= NCalendarView_PTR(new NCalendarView);;
	m_calendar->Create(160, m_calendar_height, m_rect.width() - 164, m_rect.height() - m_calendar_height - 10);
	m_calendar->init(m_prj_data);
	m_calendar->setMonth(pt.date());
	m_calendar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	AddView(m_calendar);

//	//// addTask
//	KImgButtonView_PTR pView ;
//	createImgView9PatchIconHelper(&pView, _T("./resource/b_n.9.png"), _T("./resource/b_a.9.png"),_T("./resource/b_f.9.png"), _T("./resource/mission_add_icon.png"), 5, 100, 32,28, ICON_SENTER, ICON_SENTER);
//	pView->m_clicked_signal.connect(this, &CCalendarPage::onAddTask  );
//	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//日历类型选择
	m_calendar_mode =KRadioboxGroup_PTR( new KRadioboxGroup);
	m_calendar_mode->Create(10, m_calendar_height+150, 140, 50);
	KRadioboxView_PTR  radio;
	kn_string str;

	radio =KRadioboxView_PTR( new KRadioboxView);
	radio->Create(0, 0, 140, 20);
	radio->setResourse( getSurfaceManager()->GetSurface(_T("./resource/rdo_a.png")), getSurfaceManager()->GetSurface(_T("./resource/rdo_n.png")),  getStringManager()->GetStringByID(_T("normal_calendar")));
	radio->SetTextColor( ColorSetARGB(255,230,230,230) );
	m_calendar_mode->AddView(radio);

	radio =KRadioboxView_PTR( new KRadioboxView);
	radio->Create(0, 20, 140, 20);
	radio->setResourse( getSurfaceManager()->GetSurface(_T("./resource/rdo_a.png")), getSurfaceManager()->GetSurface(_T("./resource/rdo_n.png")),  getStringManager()->GetStringByID(_T("task_calendar")));
	radio->SetTextColor( ColorSetARGB(255,230,230,230) );
	m_calendar_mode->AddView(radio);

	radio =KRadioboxView_PTR( new KRadioboxView);
	radio->Create(0, 40, 140, 20);
	radio->setResourse( getSurfaceManager()->GetSurface(_T("./resource/rdo_a.png")), getSurfaceManager()->GetSurface(_T("./resource/rdo_n.png")),  getStringManager()->GetStringByID(_T("taskcard_calendar")));
	radio->SetTextColor( ColorSetARGB(255, 230,230,230) );
	m_calendar_mode->AddView(radio);

	m_calendar_mode->SetSelectedIndex( getConfigManager()->getIntConfig(_T("calendar_mode")) );
	AddView(m_calendar_mode);

	m_calendar_mode->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_calendar_mode->m_sign_index_changed.connect(this,&CCalendarPage::onModeChange);
	m_calendar_day->m_sign_dclick.connect(this,&CCalendarPage::OnDClickDayView);

	//月份按钮
	int month_h = 28;
	int month_w = m_calendar->GetRect().width()/12;
	int month_start_x = m_calendar->GetRect().left();
	int month_start_y = 70;

	KViewGroup_PTR month_group = KViewGroup_PTR(new KViewGroup);
	month_group->Create(month_start_x, month_start_y, month_w*12, month_h);
	month_group->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_month_fk = KStaticView_PTR(new KStaticView);
	m_month_fk->setBKGColor(ColorSetARGB(130, 250,250,250));
	m_month_fk->setRect((pt.date().month()-1)*month_w, 0, month_w, month_h );
	m_month_fk->SetViewChangeType(KVIEW_LRMethod_Parent_LRNone, KVIEW_LRMethod_Parent_LRNone, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	month_group->AddView(m_month_fk);
	
	months m(1);
	date dy1 = date(2000, 1, 1);//每月一号
		for ( int i=0; i<12; i++)
		{
			m_month_btn[i] = KTextView_PTR(new KTextView);

			KRectShape* p_rect_shape = new KRectShape(m_rect);
			REColor clr = ColorSetARGB(255, 240,240,240) ;
			p_rect_shape->setStrokeColor(clr );
			p_rect_shape->setFillOpacity(0);
			p_rect_shape->setStrokeWidth(1);
			p_rect_shape->initPaint();
			KShapeDrawable_PTR drawable  = KShapeDrawable_PTR(new KShapeDrawable(p_rect_shape));
			drawable->setRect(month_start_x + i*month_w , month_start_y, month_w, month_h );
			drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
			drawable->setAutoSize(TRUE);
			m_month_btn[i]->setBKDrawable(drawable);

			m_month_btn[i]->Create(i*month_w , 0, month_w, month_h);

			m_month_btn[i]->SetTextBound( RERect::MakeXYWH(0, 0, month_w, month_h ) );
			m_month_btn[i]->SetFontSize(12);
            m_month_btn[i]->setTextAlign(REPaint::kCenter_Align);
			clr = SkColorSetARGBInline(255,240,240,240);
			m_month_btn[i]->SetTextColor(clr);
			m_month_btn[i]->SetText( getMonthString(dy1) );

			m_month_btn[i]->setName(lexical_cast<kn_string>(i+1) );
			m_month_btn[i]->SetViewChangeType(KVIEW_LRMethod_Parent_LRNone, KVIEW_LRMethod_Parent_LRNone, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
			m_month_btn[i]->m_sign_up.connect(this,&CCalendarPage::OnMonthClick);
			month_group->AddView(m_month_btn[i]);

			dy1 += m;
		}
	AddView(month_group);
}

void  CCalendarPage::OnMonthClick(kn_int x,kn_int y , KMessageMouse* msg)
{
	int m = lexical_cast<int>( msg->m_p_view->getName() ) ;
	date d = date(m_calendar->getDate().year(), m, 1 );

	m_ani_thread.addAnimationHelper( m_month_fk, PropertyPARA_ID_POS_X, msg->m_p_view->GetRect().left(), 400,0, 0 );
	m_ani_thread.Start();
	m_calendar->switchMonth(d);
}

void CCalendarPage::OnDClickDayView( kn_int x,kn_int y , KMessageMouse* msg )
{
	if (m_ani_thread.isPlaying())
	{
		return;
	}
		
	if (m_calendar && m_calendar_day)
	{
		m_calendar->switchMonth( m_calendar_day->getDate() );

		m_ani_thread.addAnimationHelper( m_month_fk, PropertyPARA_ID_POS_X, m_month_btn[m_calendar_day->getDate().month()-1]->GetRect().left(), 300,0, 0 );
		m_ani_thread.Start();
	}
}


void CCalendarPage::onModeChange(int mode)
{
	getConfigManager()->setConfig(_T("calendar_mode"), mode);
	m_calendar->setMonth( m_calendar->getDate() );
	InvalidateView();
}

void CCalendarPage::viewSizeChange()
{
	KViewGroup::viewSizeChange();

}

void CCalendarPage::initAnimation()
{
}



void CCalendarPage::onKeyUp(KMessageKey* k)
{
	if ( k->m_key_id == 'A')
	{

	}
}

void CCalendarPage::exitView()
{


}

kn_bool  CCalendarPage::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == CALENDAR_DAY_CLICK)
	{
		NProjectDayView_PTR view = VIEW_PTR(NProjectDayView)( ((KMessageDrag*)pMsg)->m_p_drag_view);
		date view_date = view->getDate();
		date d = m_calendar->getDate();
		months m(1);
		date dy1 = date(d.year(), d.month(), 1);//每月一号
		date dy2 = dy1 + m;

		if (view_date < dy1 || view_date >= dy2)
		{
			m_calendar->switchMonth(view_date);
			m_ani_thread.addAnimationHelper( m_month_fk, PropertyPARA_ID_POS_X, m_month_btn[view_date.month()-1]->GetRect().left(), 300,0, 0 );
			m_ani_thread.Start();
		}

		return TRUE;
	}

	return FALSE;
}
