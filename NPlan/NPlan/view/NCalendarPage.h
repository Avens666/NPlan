#ifndef K_CCalendarPage_VIEW_H__
#define K_CCalendarPage_VIEW_H__

#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KCheckboxView.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "TaskView.h"
#include "NScaleSilderView.h"
#include "../file/NProjectData.h"
#include "NStoryBoardPage.h"
#include "NCalendarDayView.h"

class CCalendarPage: public KViewGroup
{
public:
	CCalendarPage(void);
	virtual ~CCalendarPage(void);
	virtual void shared_ptr_inited();
	void onKeyUp(KMessageKey*);

	void init(CNProjectData* p);
	virtual kn_bool OnUserMsg(KMessage* pMsg);
	void initAnimation();
//	void moveTaskY(int offset, bool b_check_ani); //是否检查动画，动画驱动方式不检查，事件驱动时检查
	void exitView();


	virtual void viewSizeChange();
	void onModeChange(int);
	void OnDClickDayView(kn_int x,kn_int y , KMessageMouse* msg);
	void OnMonthClick(kn_int x,kn_int y , KMessageMouse* msg);


protected:
//	CTimeBarView_PTR m_time_bar;
	KTextView_PTR m_month_btn[12];
	KStaticView_PTR m_month_fk;
	NCalendarDayView_PTR m_calendar_day;
	NCalendarView_PTR m_calendar;
	CAnimationThread m_ani_thread;
//	list<CTaskBarView_PTR> m_task_bar;
	int m_title_height;
	int m_calendar_height;
	KRadioboxGroup_PTR m_calendar_mode;

	CNProjectData* m_prj_data;

	
//	KViewGroup* m_timebar_group;
};

typedef boost::shared_ptr<CCalendarPage> CCalendarPage_PTR;
#endif