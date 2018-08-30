//  File:        NProjectView.h
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
#ifndef _NPROJECTINFOVIEW_H_
#define _NPROJECTINFOVIEW_H_

#include "KViewGroup.h"
#include "KButtonView.h"
#include "KProcessView.h"
#include "KTextView.h"
#include "KProcessView.h"
#include "KEditView.h"
#include "../file/NProjectData.h"

#define PARA_ID_PROCESS PropertyPARA_ID_USER+1

class NProjectView : public KViewGroup
{
public:
	NProjectView(void);
	~NProjectView(void);
	//初始化控件
	void init( CNProjectData* p );
	void refreshProjectData();

	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 

	////用于通用型动画参数，不新增属性类实现动画参数时使用
	virtual kn_bool isChange(kn_int para_id, kn_double v);
	virtual void doChange(kn_int para_id, kn_double v);
	virtual kn_double  getParaValue(kn_int para_id);

	void processAni();
	void mouseIn();
	void mouseOut();
	CNProjectData* getProject();
protected:
	//进度条
	KProcessCirleView_PTR m_p_progress;
	//项目数据
	CNProjectData* m_proj_data;

	//title
	KTextDrawable_PTR m_proj_title;

	KTextDrawable_PTR m_undo_txt;
	KTextDrawable_PTR m_doing_txt;
	KTextDrawable_PTR m_done_txt;

	KProcessView_PTR m_undo_process;
	KProcessView_PTR m_doing_process;
	KProcessView_PTR m_done_process;

	//lead
	KStaticView_PTR  m_lead_view;



	//member
	KEditView_PTR  m_member_view;

	KStaticView_PTR m_frame_light;

	CAnimationThread m_ani_thread;
	CAnimationThread m_ani_process_thread;
};

typedef boost::shared_ptr<NProjectView> NProjectView_PTR;
#endif _NPROJECTINFOVIEW_H_

