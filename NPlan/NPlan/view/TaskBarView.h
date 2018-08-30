//  File:        TaskBarView.h
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
#ifndef K_TASKBAR_VIEW_H__
#define K_TASKBAR_VIEW_H__

#include "KImageDrawable.h"
#include "KViewGroup.h"
#include "KTextView.h"
#include "K9PatchImageDrawable.h"
#include "TimeLine.h"
#include "../file/NProjectTask.h"
#include "../manager/KStringManager.h"
#define KVIEW_TASKVIEW 1001;

#define PARA_ID_TASKBAR_APLHA PropertyPARA_ID_USER+2

#define MODE_SMALL 0
#define MODE_BIG 1
enum KMOVETYPE
{
	MOVE_NORMAL,
	MOVE_ADJUST_START,
	MOVE_ADJUST_END,
	MOVE_ADJUST_POS,
};
class CTimeBarView;
typedef boost::shared_ptr<CTimeBarView> CTimeBarView_PTR;
class CTaskView;
typedef boost::shared_ptr<CTaskView> CTaskView_PTR;
typedef boost::weak_ptr<CTaskView> CTaskView_WEAK_PTR;

class CTaskBarView: public KTextView
{
public:
	CTaskBarView(void);
	virtual ~CTaskBarView(void);
	virtual void shared_ptr_inited();
	//virtual void OnMove(kn_int iScreenX, kn_int iScreenY);
	//virtual void OnDown(kn_int iScreenX, kn_int iScreenY);
	//virtual void OnUp(kn_int iScreenX, kn_int iScreenY);

	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 

	virtual void init(CNProjectTaskCardBase* task, CTimeBarView_PTR timebar);
	void setTaskBarBK(K9PatchImageDrawable_PTR );

	virtual void OnWheel(KMessageMouseWheel* pMsg);
	virtual void syncTimeline(CTimeBarView_PTR);

	virtual void refreshInfo();
	void setBarWidth(const RERect& rect);

	void taskbarUnActive();

	CNProjectTask* GetTask();

	////用于通用型动画参数，不新增属性类实现动画参数时使用
	virtual kn_bool isChange(kn_int para_id, kn_double v);
	virtual void doChange(kn_int para_id, kn_double v);
	virtual kn_double  getParaValue(kn_int para_id);

	void setTaskView(CTaskView_WEAK_PTR);
	CTaskView_PTR getTaskView();
	bool isActionShow();
	void setActionShow(bool);
	void setMode(bool); //
protected:
	K9PatchImageDrawable_PTR m_taskbar;
	kn_uint m_taskbar_index;//taskbar 可能会更换，记录drawable的索引
	KTextDrawable_PTR m_member_txt;
	KTextDrawable_PTR m_time_txt;
	KImageDrawable_PTR m_pm_icon;
	KImageDrawable_PTR m_lmove_icon;
	KImageDrawable_PTR m_lrmove_icon;
	KImageDrawable_PTR m_rmove_icon;

//	int m_timeline_offset;
	//左键按下时的差值
	int m_mouse_x_offset;

	CNProjectTaskCardBase* m_p_task;
	KMOVETYPE m_move_type;
	CTimeBarView_PTR m_timebar;
	//CTaskView 和 CTaskBarView会交叉引用，所以需要弱引用，否则智能指针交叉引用，都释放不掉
	CTaskView_WEAK_PTR	m_task_view;
	CAnimationThread m_ani_thread;

	int m_taskbar_y;
	int m_bar_h;
	int m_icon_alpha;
	kn_bool m_icon_show;
	kn_bool m_action_show;
	kn_bool m_mode;
};
typedef boost::shared_ptr<CTaskBarView> CTaskBarView_PTR;
//CTaskView 和 CTaskBarView会交叉引用，所以需要弱引用，否则智能指针交叉引用，都释放不掉
typedef boost::weak_ptr<CTaskBarView> CTaskBarView_WEAK_PTR;

class CActionBarView: public CTaskBarView
{
public:
	CActionBarView(void);
	virtual ~CActionBarView(void);
	virtual void init(CNProjectTaskCardBase* task, CTimeBarView_PTR timebar);
	virtual void refreshInfo();
	virtual void syncTimeline(CTimeBarView_PTR);
protected:
//	CNProjectTaskCard* m_p_action;
};

typedef boost::shared_ptr<CActionBarView> CActionBarView_PTR;

#endif

