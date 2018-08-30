//  File:        NMindMapView.h
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
#ifndef _NMindMapView_H_
#define _NMindMapView_H_

#include "KViewGroup.h"
#include "../file/NProjectData.h"
#include "NStoryBoardPage.h"
#include "TaskView.h"
#include "../NPlan.h"
#include "sigslot.h"
#include "KShapeDrawable.h"
#include "KButtonView.h"
#include "AnimationThread.h"

//action子项
class NMindMapActionView: public CActionView
{
public:
	NMindMapActionView();
	virtual ~NMindMapActionView();
	virtual void shared_ptr_inited();
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void Init(CNProjectTaskCard* p, kn_uint row);
	void OnClickEvent(KView_PTR);
	void showOnFoucsDrawable();
	void onKeyUp(KMessageKey* kMsg);
	void onDeleteAction();
	void hideOnFoucsDrawable();
	void setActionBkAShow(kn_bool bShow);
	void OnMemDragUpIn(KView_PTR v);

	void OnEditAction(kn_int x, kn_int y, KMessageMouse* KMsg);
	void ShowEditActionName();
	void ShowEditActionTime();
	void OnEditActionNameOver(kn_string edit_str);
	void OnEditActionTimeOver(kn_string edit_str);

	void OnMindActionMenu(KView_PTR, kn_int);
public:
	sigslot::signal3<KView_PTR, RERect, kn_string> m_name_edit_sign;
	sigslot::signal3<KView_PTR, RERect, kn_string> m_time_edit_sign;

protected:
	KDrawable_PTR m_bk_a;
	// 记录自己原来在parent m_lst_view中的位置
	kn_int m_own_index;
	// 表示选中的drawable
	KDrawable_PTR m_onfoucs_drawable;
	kn_bool m_b_edit;
};
typedef boost::shared_ptr<NMindMapActionView> NMindMapActionView_PTR;

//action 面板
class NMindMapBoardView: public CBoardView
{
public:
	NMindMapBoardView();
	virtual ~NMindMapBoardView();
	virtual void init(CNProjectTask* p, kn_uint row);
	void OnMenDragUpIn(KMessageDrag* mag);
	void OnMenDragIn(KMessageDrag* mag);
	void OnMenDragOut(KMessageDrag* mag);
	void setShowBkA(kn_bool bShow);
	kn_bool isPointInActionView(kn_int x, kn_int y);
	void calcActionPos();
	virtual void onDragDirect(KMessageDrag* mag);
	void OnAddAction(kn_int row);
	void OnMouseDown(kn_int x, kn_int y);
	NMindMapActionView_PTR getEditActionView();

	// 编辑
	void OnEditActionName(KView_PTR v, RERect rect, kn_string str);
	void OnEditActionTime(KView_PTR v, RERect rect, kn_string str);

protected:
	KEditView_PTR m_name_edit;
	KEditView_PTR m_time_edit;
	// 标记当前正在编辑的action view
	NMindMapActionView_PTR m_edit_action_view;
};
typedef boost::shared_ptr<NMindMapBoardView> NMindMapBoardView_PTR;

////基类
class NMindMapItemBase: public KViewGroup
{
public:
	typedef boost::shared_ptr<NMindMapItemBase> NMindMapItemBase_PTR;
	NMindMapItemBase();
	virtual void shared_ptr_inited();

	virtual ~NMindMapItemBase();

	virtual void calcPos(); //计算子项的坐标
	virtual int getItemHeight();  //返回当前项及子项的整体高度
	void addItem(NMindMapItemBase_PTR);
	void onClickEvent(KView_PTR v);
	virtual void showOnFoucsDrawable();
	virtual void hideOnFoucsDrawable();
	virtual void setBkAShow(kn_bool bShow);
	virtual void OnMemDragUpIn(KView_PTR v) = 0;
	virtual void OnPointDragOut(KMessageDrag* mag) = 0;
	virtual void OnActDragUpIn(KView_PTR v) = 0;
	virtual void OnActDragIn(KView_PTR v) = 0;
	virtual kn_bool isPointInActionView(kn_int x, kn_int y) = 0;
	virtual void adjustConnectParientLinePos(const RERect& par_rect) = 0;
	virtual void OnMouseDown(kn_int x, kn_int y) = 0;
protected:
	KDrawable_PTR m_bk;
	KDrawable_PTR m_bk_a;
	KTextMultiLineDrawable_PTR	m_name_text_drawable;
	KStaticView_PTR m_leader;
	KEditView_PTR m_edit_name;
	KColorDrawable_PTR m_child_line_drawable;
	KShapeDrawable_PTR m_parient_line_drawable;
	std::list<NMindMapItemBase_PTR> m_item_lst;
	KDrawable_PTR m_onfoucs_drawable;
};
typedef boost::shared_ptr<NMindMapItemBase> NMindMapItemBase_PTR;

//任务节点
class NMindMapItemTask: public NMindMapItemBase
{
public:
	NMindMapItemTask();
	virtual ~NMindMapItemTask();
	virtual void shared_ptr_inited();
	void init(CNProjectTask* p);
	void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	//返回当前项及子项的整体高度
	virtual int getItemHeight();
	void setLeft(BOOL);
	void refreshTaskInfo();
	//计算子项的坐标
	void calcPos();

	// 拖拽
	void OnPointDragOut(KMessageDrag* mag);
	void OnMemDragUpIn(KView_PTR v);
	void OnActDragUpIn(KView_PTR v);
	void OnActDragIn(KView_PTR v);
	kn_bool isPointInActionView(kn_int x, kn_int y);
	kn_bool isPointInBoardView(kn_int x, kn_int y);
	void adjustConnectParientLinePos(const RERect& par_rect);
	// 编辑
	void OnEditTaskName(kn_int, kn_int, KMessageMouse*);
	void ShowEditTaskName();
	void OnEditTaskNameOver(kn_string edit_str);

	// 鼠标down更新按钮foucs状态
	void OnMouseDown(kn_int x, kn_int y);
	void onKeyUp(KMessageKey* kMsg);
	void onAddAction();
	void onDelTask();
	// 菜单响应函数
	void OnMindTaskMenu(KView_PTR, kn_int);

public:
	sigslot::signal2<CNProjectTask*, KView_PTR> m_del_task;
	sigslot::signal0<> m_update_pos_sign;
	sigslot::signal1<RERect> m_rect_adjust_pos_sign;
protected:
	CNProjectTask* m_p_task;
	CTaskView_PTR m_task_view;
	NMindMapBoardView_PTR m_action_board;
	BOOL m_b_left; //标识节点在导图中布局是左还是右
};
typedef boost::shared_ptr<NMindMapItemTask> NMindMapItemTask_PTR;

//项目节点
class NMindMapItemPrj: public NMindMapItemBase
{
public:
	NMindMapItemPrj();
	virtual ~NMindMapItemPrj();
	virtual void shared_ptr_inited();
	void init(CNProjectData* p);
	kn_bool OnUserMsg(KMessage* pMsg);
	void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	// 创建TaskView
	NMindMapItemTask_PTR CreatTaskView(kn_int iPos, CNProjectTask* p);
	// 计算子项的坐标
	virtual void calcPos();
	// 调整位置
	void OnUpdatePos();
	// 根据view的调整位置
	void adjustPosByViewRect(RERect rect_by_area);

	void OnMemDragUpIn(KView_PTR v);
	void OnPointDragOut(KMessageDrag* mag);
	void OnActDragUpIn(KView_PTR v);
	void OnActDragIn(KView_PTR v);

	kn_bool isPointInActionView(kn_int x, kn_int y);
	kn_bool isPointInBlank(kn_int x, kn_int y);
	void adjustConnectParientLinePos(const RERect& par_rect);
	void OnEditPrjName(kn_int, kn_int, KMessageMouse*);
	void ShowEditPrjName();
	void OnEditPrjNameOver(kn_string edit_str);
	void OnMouseDown(kn_int x, kn_int y);
	void onKeyUp(KMessageKey* KMsg);
	void OnAddTask();
	void OnDelTask(CNProjectTask* prj_data, KView_PTR v);

	void OnMindPrjMenu(KView_PTR , kn_int);

protected:
	CNProjectData* m_p_prj;
	KViewGroup_PTR m_prj_group;
	CAnimationThread m_ani_thread;
};
typedef boost::shared_ptr<NMindMapItemPrj> NMindMapItemPrj_PTR;

//项目导图编辑区
class NMindMapArea : public KViewGroup
{
public:
	NMindMapArea();
	virtual ~NMindMapArea();
	void init(CNProjectData* p);

	void OnMove(kn_int x, kn_int y, KMessageMouse* pMsg);
	void OnDown(kn_int x, kn_int y, KMessageMouse* pMsg);
	void OnRDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void onWheelDirect(KMessageMouseWheel* pMsg);
	void onDragDirect(KMessageDrag* mag);

	// 是否应该移动
	kn_bool isCanMove(kn_int x, kn_int y);
	// 调整位置
	void updatePos();
	// 菜单点击响应函数
	void OnMindAreaMenu(KView_PTR , kn_int);
protected:
	NMindMapItemPrj_PTR m_mindmap_item_prj;
	REPoint m_pos_bak;
	REPoint m_mouse_pos_bak;
	kn_bool m_b_down;
};
typedef boost::shared_ptr<NMindMapArea> NMindMapArea_PTR;

//导图控件
class NMindMapView : public KViewGroup
{
public:
	NMindMapView();
	virtual ~NMindMapView();
	void init(CNProjectData* p);
	// 窗口大小调整时会调用
	void viewSizeChange();
protected:
	NMindMapArea_PTR m_mindmap;
};
typedef boost::shared_ptr<NMindMapView> NMindMapView_PTR;
#endif //_NMindMapView_H_
