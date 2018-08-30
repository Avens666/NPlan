//  File:        NProjectEditPage.h
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
#ifndef K_PROJECTEDITPage_VIEW_H__
#define K_PROJECTEDITPage_VIEW_H__

#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KCheckboxView.h"
#include "TimeBarView.h"
#include "TaskBarView.h"
#include "TaskView.h"
#include "NScaleSilderView.h"
#include "../file/NProjectData.h"
#include "NStoryBoardPage.h"
#include "NMindMapView.h"
#include "NResourceGroupView.h"

class CProjectEditPage: public KViewGroup
{
public:
	CProjectEditPage(void);
	virtual ~CProjectEditPage(void);
	virtual void shared_ptr_inited();

	void onKeyUp(KMessageKey*);
	void init(CNProjectData* p);
	vector<NMemberMsg> getProjectResourceData();
	void updateProjectResourceView();
	virtual kn_bool OnUserMsg(KMessage* pMsg);
	void initAnimation();
//	void moveTaskY(int offset, bool b_check_ani); //是否检查动画，动画驱动方式不检查，事件驱动时检查
	void exitView();


	virtual void viewSizeChange();
	void onModeChange(int);
	void OnDClickDayView(kn_int x,kn_int y , KMessageMouse* msg);
	void OnMonthClick(kn_int x,kn_int y , KMessageMouse* msg);


protected:
	int m_title_height;

	NMindMapView_PTR m_mindmap_view;
	KResourceGroupView_PTR m_prj_res_group;
	CNProjectData* m_prj_data;
};

typedef boost::shared_ptr<CProjectEditPage> CProjectEditPage_PTR;
#endif
