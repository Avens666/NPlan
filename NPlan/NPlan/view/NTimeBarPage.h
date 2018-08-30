//  File:        NTimeBarPage.h
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
#ifndef K_CTimeBarPage_VIEW_H__
#define K_CTimeBarPage_VIEW_H__

#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KCheckboxView.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "TaskView.h"
#include "NNormalSilderView.h"
#include "../file/NProjectData.h"
#include "NStoryBoardPage.h"

#define PARA_ID_TimeBar_Y PropertyPARA_ID_USER+4

class CTimeBarPage: public KViewGroup
{
public:
	CTimeBarPage(void);
	virtual ~CTimeBarPage(void);
	virtual void shared_ptr_inited(); 

	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual kn_bool OnUserMsg(KMessage* pMsg);
	virtual void onDragDirect(KMessageDrag* mag);

	////用于通用型动画参数，不新增属性类实现动画参数时使用
	virtual kn_bool isChange(kn_int para_id, kn_double v);
	virtual void doChange(kn_int para_id, kn_double v);
	virtual kn_double  getParaValue(kn_int para_id);

	virtual void OnTaskViewClick(KView_PTR);
	virtual void OnTaskViewRClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void onColorMenu(KView_PTR , kn_int n);

	virtual void OnTaskbarDClick(kn_int , kn_int , KMessageMouse*);
	virtual void OnTaskbarRightClick(kn_int , kn_int , KMessageMouse*);
	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 
	virtual void OnTimelineChange(kn_bool b);
	virtual void OnMoveSilder(kn_int i_value);
	virtual void OnMoveSilderWholeProject();

	virtual void OnPrjDClick(kn_int , kn_int , KMessageMouse*);

	void onKeyUp(KMessageKey*);
	void onAddTask(KView_PTR);
	void onAddEvent(KView_PTR);
	void onEventPrev(KView_PTR);
	void onEventForward(KView_PTR);

	void OnTaskBtnDrag(KMessageDrag* mag);
	void OnTaskBtnDragUp(KMessageDrag* mag);
	void setTaskDragDirection(TASK_DRAG_DIR eDrag_dir);
	void setViewPositionY(KView_PTR pView, kn_int iPositionY);
	void updateTaskBarAndDataList();
	void aniMoveAllTask();
	void updateTaskBtnList(CTaskView_PTR task_view, kn_int drag_id, kn_int drag_y_pos);
	void aniMoveTaskBtn(CTaskView_PTR task_view);
	void changeTastBtnList(kn_int old_id, kn_int new_id);

	void onDelTask(CTaskView_PTR pMsg);
	void addTask(CNProjectTask*, int task_x);
	void addEvent(CNProjectMileStone* event);
	void init(CNProjectData* p);
	void initAnimation();
	void moveTaskY(int offset, bool b_check_ani); //是否检查动画，动画驱动方式不检查，事件驱动时检查
	void exitView();
	CAnimationThread m_ani_thread;
	void delMileStone(KView_PTR p);
	virtual void viewSizeChange();
	void showWholeProject();
	//获得任务区第一个任务相对坐标
	kn_int getTaskY();
	void setTaskY(kn_int y);
	//滚动到指定任务
	void moveToTask(int index);
	//设置新添加Task的信息
	void setNewTaskInfo(CNProjectTask* pTask);

	//子任务显示
	void showActionEdit(CTaskBarView_PTR );
	bool hideActionEdit();

	//时间轴缩放动画
	void timelineScaleTo(int v, int pos);
	//更新底层处理不了的控件
	void updateModifyData();

	//调整任务显示大小
	void setMode(bool);
	void OnRefershSilder();
	void onFoldClick(KView_PTR);
	void onUnfoldClick(KView_PTR);
	void Release();

	virtual void OnGesture(KGesture* pGesture);
protected:
	CTimeBarView_PTR m_time_bar;
	KStaticView_PTR m_left_view;
	KStaticView_PTR m_action_bk;
	list<CTaskBarView_PTR> m_task_bar;
	list<CTaskView_PTR> m_task_btn;
	list<CEventView_PTR> m_lst_event;
	list<CActionBarView_PTR> m_action_bar;
	CNProjectTask_LST *m_lst_task;
	NNormalSilderView_PTR m_silder_bar;
	int m_task_height;
	int m_action_h;
	int m_event_height;
	int m_task_width;
	int m_task_y;
	kn_bool m_mode;
	CNProjectData* m_prj_data;

	KImgButtonView_PTR m_fold_btn;
	KImgButtonView_PTR m_unfold_btn;
//	KViewGroup* m_timebar_group;

	TASK_DRAG_DIR m_drag_dir;
	kn_int m_nDragViewIndex;
	kn_int m_nDragChangeIndex;
	kn_int m_nWheelOffset;
};

typedef boost::shared_ptr<CTimeBarPage> CTimeBarPage_PTR;
#endif
