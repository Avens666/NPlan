//  File:        MilestoneEditView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/2/27
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef _MILESTONEEDITVIEW_H_
#define _MILESTONEEDITVIEW_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KEditView.h"
#include "KSurfaceManager.h"
#include "../file/NProjectMileStone.h"
#define KN_REUSLT_USER_DEL			KN_REUSLT_USER + 1

class MilestoneEditView : public KViewGroup
{
public:
	MilestoneEditView(void);
	~MilestoneEditView(void);
	void init(CNProjectMileStone* data);
	void onBtnCancle(KView_PTR p);
	void onBtnOk(KView_PTR p);
	void onBtnDel(KView_PTR p);

protected:
	
	KEditView_PTR m_edit_name;
	CNProjectMileStone* m_p_data;
};

typedef boost::shared_ptr<MilestoneEditView> MilestoneEditView_PTR;
#endif //_MILESTONEEDITVIEW_H_

