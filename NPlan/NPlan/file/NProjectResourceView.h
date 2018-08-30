//  File:        NProjectResourceView.h
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
#ifndef _NPROJECTRESOURCEVIEW_H_
#define _NPROJECTRESOURCEVIEW_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KSurfaceManager.h"
#include "NGridView.h"
#include "NSilderCursorView.h"
#include "NSlideView.h"
#include "KEditView.h"
#include "KScreen.h"
#include "KButtonView.h"
#include "NTaskSourceView.h"

class NProjectResourceView : public KViewGroup
{
public:
	NProjectResourceView(CNProjectData* pProjectData);
	~NProjectResourceView(void);
	void InitCtrl();

private:
	/**
	* 统一设置字体的式样
	*/
	void setFontStyle(KTextDrawable_PTR& p);
	void onBtnCancle(KView* p);
	void onBtnOk(KView* p);
	void onBtnAdd(KView* p);
	void onCloseWindow();
	void saveProjectData();

private:
	//背景
	KTextView* m_img_backgroup;

	KViewGroup* m_group_task;
	/**
	* 标题文本
	*/
	KTitleView* m_text_title;
	/**
	* 名称文本	
	*/
	KTextView* m_text_name;
	KEditView* m_edit_name;

	/**
	* 资源	
	*/
	KTextView* m_text_resource;
	KViewGroup*	m_grou_resource;
	NGridView*	m_view_satff_grid;
	NSlideView* m_silder;

	//是否点击标题栏
	kn_bool		m_down_title;

	//project data
	CNProjectData* m_projectData;
};
#endif

