//  File:        NProjectPage.h
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
#ifndef K_CProjectPage_VIEW_H__
#define K_CProjectPage_VIEW_H__

#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KCheckboxView.h"
#include "TaskView.h"
#include "NScaleSilderView.h"
#include "../file/NProjectData.h"
#include "NTimeBarPage.h"
#include "KProcessView.h"
#include "KEditView.h"
#include "NBottomMenuView.h"
#include "../NPlan.h"
#include "NCalendarPage.h"
#include "NProjectEditPage.h"

#define PROJECT_EDIT_PAGE 0
#define TIMEBAR_PAGE 1
#define STORYBOARD_PAGE 2
#define STORYBOARD_MAN_PAGE 3
#define CALENDAR_PAGE 4


class CProjectPage: public KViewGroup
{
public:
	CProjectPage(void);
	virtual ~CProjectPage(void);
	virtual void shared_ptr_inited(); 

	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual kn_bool OnUserMsg(KMessage* pMsg);
	void onKeyUp(KMessageKey*);
	void onHomeClick(KView_PTR);
	void onSaveClick(KView_PTR);
	void onSaveAsClick(KView_PTR);
	void onSettingClick(KView_PTR);

	void init(CNProjectData* p);
	void onSaveAs(KView_PTR v, kn_int n);
	void saveAs(kn_int n);
	kn_bool saveAsDetail(kn_string config_item, kn_string png_name, kn_string ext_name, kn_string & filepath);
	void initAnimation();
	void exitView();
	void switchPage(kn_int);
	CAnimationThread m_ani_thread;
	void refreshProjectData();
	void releaseFocusView();
	kn_bool confirmModifyView();
protected:
	KStaticView_PTR m_left_view;
	//进度条
	KProcessView_PTR m_p_progress;
	KEditView_PTR  m_proj_title;

	int m_task_height;
	int m_task_width;

	CNProjectData m_prj_data;
	//指向传入的project数据指针
	CNProjectData* m_p_prj_data;

	CTimeBarPage_PTR	m_time_bar;
	CStoryBoardPage_PTR m_story_board;
	CCalendarPage_PTR	m_calendar_page;
	CStoryBoardManPage_PTR m_story_man_board; //以人分组的故事版
	CProjectEditPage_PTR m_prj_edit_page;


	int m_active_page;//当前page编号

	KViewGroup_PTR m_current_page;
	KViewGroup_PTR m_old_page;

	NBottomMenuView_PTR m_bottom_view;
};

typedef boost::shared_ptr<CProjectPage> CProjectPage_PTR;
#endif
