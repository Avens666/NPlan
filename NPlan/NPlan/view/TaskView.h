//  File:        TaskView.h
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
#ifndef K_TASK_VIEW_H__
#define K_TASK_VIEW_H__

#include "KImageDrawable.h"
#include "KTextMultiLineDrawable.h"
#include "KViewGroup.h"
#include "KTextView.h"
#include "TimeLine.h"
#include "TaskBarView.h"
#include "../file/NProjectMileStone.h"
#define KVIEW_TASKVIEW 1001;

enum TASK_DRAG_DIR
{
	DRAG_DOWNWARD,	  ///<向下滑动
	DRAG_UPWARD,	  ///<向上滑动
	DRAG_UNKNOWN  ///<Unknown
};

class CTaskView: public KTextView
{
public:
	CTaskView(void);
	virtual ~CTaskView(void);
	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void init(CNProjectTask* task);
	void onMenu(kn_int n);
	CNProjectTask* GetTask();
	void setTaskBar(CTaskBarView_WEAK_PTR p);
	CTaskBarView_PTR getTaskBar();
	void refreshInfo();
	void setMode(bool); 
	kn_int getTaskID();
	void setNameText(const kn_string& name_text);
	kn_string getNameText();
	REColor getBkColor();
	void setBMove(kn_bool bMove);
	// click信号以便与drag区分
	sigslot::signal1<KView_PTR> m_task_click_sign;
protected:
	// 记录down的位置
	REPoint m_down_point;
	// 记录down离pos的偏移量
	REPoint m_down_point_offset;
	kn_bool m_b_click;
	REColor m_bk_color;
	kn_bool m_b_move;

	CNProjectTask* m_p_task;

	KTextDrawable_PTR m_text_id;
	KTextMultiLineDrawable_PTR m_text_name;
	//CTaskView 和 CTaskBarView会交叉引用，所以需要弱引用，否则智能指针交叉引用，都释放不掉
	CTaskBarView_WEAK_PTR m_p_taskbar;
};
typedef boost::shared_ptr<CTaskView> CTaskView_PTR;
//CTaskView 和 CTaskBarView会交叉引用，所以需要弱引用，否则智能指针交叉引用，都释放不掉
typedef boost::weak_ptr<CTaskView> CTaskView_WEAK_PTR;

class CEventView: public KTextView
{
public:
	CEventView(void);
	virtual ~CEventView(void);
	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);

	void init(CNProjectMileStone* task, CTimeBarView_PTR timebar);

	CNProjectMileStone* getMileStone();
	void refreshInfo();
	void syncTimeline(CTimeBarView_PTR tl);


	sigslot::signal1<KView_PTR> m_sign_btn_del;
protected:

	CNProjectMileStone* m_p_data;
	KTextMultiLineDrawable_PTR m_info;
	KTextDrawable_PTR m_time;
	KTextDrawable_PTR m_event_id;
	CTimeBarView_PTR m_timebar;

	kn_bool m_b_move;
	int m_mouse_x_offset;
	int m_w;
};
typedef boost::shared_ptr<CEventView> CEventView_PTR;

#endif

