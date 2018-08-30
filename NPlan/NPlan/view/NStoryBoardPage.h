//  File:        NStoryBoardPage.h
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
#ifndef K_NStoryBoardPage_H__
#define K_NStoryBoardPage_H__

#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KCheckboxView.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "TaskView.h"
#include "NScaleSilderView.h"
#include "../file/NProjectData.h"

#define NPLAN_ACTION_VIEW  1000
class CActionView : public KTextView
{
public:
	CActionView(void);
	virtual ~CActionView(void);
	virtual void init(CNProjectTaskCard* , kn_uint row);
	void OnMove(kn_int x, kn_int y, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);

	void onMenu(kn_int n);

	CNProjectTask* getTask();
	int getState();
	void setData(CNProjectTaskCard* task_card);
	CNProjectTaskCard*  getData();
	kn_uint getRow();
	void setRow(kn_uint);
	virtual void SetPosition(kn_int iX, kn_int iY);
	void updateData();
	kn_string getBgColorByID(kn_int lead_id);
	void updateLeaderInfo();
protected:
	CNProjectTaskCard* m_p_action;

	//KTextDrawable_PTR m_p_name;
	//KTextDrawable_PTR m_p_time;
	////task模式 info 为人， 人员模式 info为任务名
	//KTextDrawable_PTR m_p_info;
	KTextDrawable_PTR m_p_score;
	KTextDrawable_PTR m_p_member;
	KTextMultiLineDrawable_PTR m_p_name;
	KTextDrawable_PTR m_txt_date;


	kn_uint m_row;
};
typedef boost::shared_ptr<CActionView> CActionView_PTR;

class CBoardView : public KViewGroup
{
public:
	CBoardView(void);
	virtual ~CBoardView(void);
	virtual void init(CNProjectTask*, int state, kn_uint row, bool b_point ); //b_point 标识使用action的指针链表还是原始链表，任务分组是原始链表，人员分组是指针链表
	//添加
	virtual void AddView(KView_PTR v);
	virtual void SetRect(const RERect& rect);
	//重新计算内部actionview位置
	virtual void calcActionPos();
	virtual void onDragDirect(KMessageDrag* mag);
	kn_uint getRow();
	void setRow(kn_uint);



protected:
	CNProjectTask* m_p_task;

	//board的action  state
	int m_state;
	//board的行数
	kn_uint m_row;

	K9PatchImageDrawable_PTR m_bk;
	K9PatchImageDrawable_PTR m_bk_a;


};

typedef boost::shared_ptr<CBoardView> CBoardView_PTR;

class CStoryMsgView : public KView
{
public:
	CStoryMsgView(void){};
	virtual ~CStoryMsgView(void){};
	virtual void onDragDirect(KMessageDrag* mag);
};
typedef boost::shared_ptr<CStoryMsgView> CStoryMsgView_PTR;

class CStoryBoardPage: public KViewGroup
{
public:
	CStoryBoardPage(void);
	virtual ~CStoryBoardPage(void);
	virtual void shared_ptr_inited();

	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);

	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 
	virtual void viewSizeChange();
	void onKeyUp(KMessageKey*);
	virtual void init(CNProjectData* p);
	void initAnimation();
	void moveTaskY(int offset);
	//计算故事版布局
	virtual void calcBoardArea();
	CNProjectData* getPrjData();
	CAnimationThread m_ani_thread;
	virtual void Release();
	void refreshTitle();
protected:
	//放在底层接受消息使用
	CStoryMsgView_PTR m_msg_view;

	KStaticView_PTR m_left_view;
	list<CBoardView_PTR> m_lst_board;
	list<KStaticView_PTR> m_task_btn;
	list<KStaticView_PTR> m_board_title;
	CNProjectTask_LST *m_lst_task;
	int m_task_height;
	int m_task_width;
	//左侧任务按钮宽度
	int m_task_w;
	//标题区域高度
	int m_title_height;

	//action size
	REPoint m_action_size;
	
	CNProjectData* m_p_prj_data;

	int m_board_count;
};
typedef boost::shared_ptr<CStoryBoardPage> CStoryBoardPage_PTR;

class CStoryBoardManPage: public CStoryBoardPage
{
public:
	CStoryBoardManPage(void);
	virtual ~CStoryBoardManPage(void);
	virtual void init(CNProjectData* p);
	void calcBoardArea();
};
typedef boost::shared_ptr<CStoryBoardManPage> CStoryBoardManPage_PTR;

#endif

