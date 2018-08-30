#ifndef _NCalendarDayView_H_
#define _NCalendarDayView_H_
#include "KViewGroup.h"
#include "AnimationThread.h"
#include "DataSync.h"
#include "KTimer.h"
#include "K9PatchImageDrawable.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "KSurfaceManager.h"
#include "../file/NProjectData.h"
#include "KMenuView.h"
using namespace boost::posix_time;
using namespace boost::gregorian;

class NCalendarDayView: public KView
{
public:
	NCalendarDayView();
	~NCalendarDayView(void);

	void init();

    void setDate(const date& );
	date getDate();

protected:
	date m_date;
	int m_month_h;
	KDrawable_PTR		m_bk_drawable;
	KTextDrawable_PTR	m_txt_month_drawable;
	KTextDrawable_PTR	m_txt_day_drawable;
	KTextDrawable_PTR	m_txt_week_drawable;

};
typedef boost::shared_ptr<NCalendarDayView> NCalendarDayView_PTR;



//月份日历中的天对象，含有项目在当天的信息
#define DAYVIEW_NORMAL_MODE 0
#define DAYVIEW_PROJECT_MODE 1
typedef vector<KStaticView_PTR> LST_PRJINFO_VIEW;
class NProjectDayView: public KViewGroup
{
public:
	NProjectDayView();
	~NProjectDayView(void);
	virtual void shared_ptr_inited(); 

	virtual void viewSizeChange();
	void init(CNProjectData*);
	void setFrameColor(REColor&);
	void setBkColor(REColor&);

	void setDate(date& );
	//是否是当月日期
	void setInMonth(bool);
	date getDate();
	void updateColor();
	void updatePrjInfo();
	void updatePrjInfoPos();
	void click( kn_int, kn_int,KMessageMouse* );
	void rclick( kn_int, kn_int,KMessageMouse* );

	void onDayMenu(KView_PTR , kn_int n);
protected:
	date m_date;
	int m_mode; //0 标准日历模式  1 项目任务信息模式  2 子任务
	int m_month_h;
	bool m_b_in_month;
	int m_show_type; //0 常规日历 1 项目数据
	CNProjectData* m_prj_data;
	KDrawable_PTR		m_bk_drawable;
	KDrawable_PTR		m_today_drawable; //当天日期显示外框
	KTextDrawable_PTR	m_txt_day_drawable;
//	KTextDrawable_PTR	m_txt_prj_day_drawable;
	KTextDrawable_PTR	m_chinese_day_drawable; //农历

	LST_PRJINFO_VIEW	m_prj_info_view_list;
};
typedef boost::shared_ptr<NProjectDayView> NProjectDayView_PTR;


class NDayGroupView: public KViewGroup
{
public:
	NDayGroupView();
	~NDayGroupView(void);

	virtual void viewSizeChange();

	void init(CNProjectData*);
    void setMonth(const date& );
	date getDate();	

protected:
	CNProjectData* m_prj_data;
	date m_date;
	NProjectDayView_PTR m_day_group[42];
};
typedef boost::shared_ptr<NDayGroupView> NDayGroupView_PTR;

class NYearSeleteView: public KMenuBase
{
public:
	NYearSeleteView();
	~NYearSeleteView(void);

	void init(int year );

	void switchYear(int dir); // 0 往后  1往前
	void click( kn_int, kn_int,KMessageMouse* );
	void prevClick( kn_int, kn_int,KMessageMouse* );
	void nextClick( kn_int, kn_int,KMessageMouse* );

	sigslot::signal1<kn_int > m_sign_year_click;
protected:

	KTextView_PTR	m_txt_year1[9];
	KTextView_PTR	m_txt_year2[9];
	//标记当前是m_txt_year1还是 m_txt_year2
	int m_current_id; 
	int m_current_year;

	KTextView_PTR	m_btn_prev;
	KTextView_PTR	m_btn_next;

	CAnimationThread m_ani_thread;
};
typedef boost::shared_ptr<NYearSeleteView> NYearSeleteView_PTR;



class NCalendarView: public KViewGroup
{
public:
	NCalendarView();
	~NCalendarView(void);

	void init(CNProjectData* );

    void setMonth(const date& );
	date getDate();
    void switchMonth(const date& ); // true 往后一个月  false 往前一个月
	virtual kn_bool  OnUserMsg(KMessage* pMsg);
	void yearClick(kn_int, kn_int,KMessageMouse* );
	void onYearChange(int);
protected:
	CNProjectData* m_prj_data;
	date m_date;
	int m_month_h;
	int m_week_h;
	KDrawable_PTR		m_bk_drawable;
	KTextView_PTR		m_txt_month_view;
	KTextDrawable_PTR	m_txt_week[7];

	NDayGroupView_PTR	m_month_view;
	NDayGroupView_PTR	m_month_view_bak;

	CAnimationThread m_ani_thread;
};
typedef boost::shared_ptr<NCalendarView> NCalendarView_PTR;

#endif	//_KMAINVIEW_H_
