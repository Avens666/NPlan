//  File:        NProjectResourceView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/28
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
#include "../file/NProjectData.h"

class NProjectResourceView : public KViewGroup
{
public:
	NProjectResourceView(CNProjectData* pProjectData);
	~NProjectResourceView(void);
	void initCtrl();

private:
	/**
	* 统一设置字体的式样
	*/
	void setFontStyle(KTextDrawable_PTR& p);
	void onBtnCancle(KView_PTR p);
	void onBtnOk(KView_PTR p);
	void onBtnAdd(KView_PTR p);
	void saveProjectData();
	void UpdateResourceGrid(vector<NMemberMsg>& v );
	void TranslateMemberListToResouceList(vector<NMemberMsg>& vecMemeber,CNProjectResourcePool& originalResourcePool, CNProjectResourcePool& extraResourcePool );
	void MergeResourceByName(CNProjectResourcePool& dstPool, CNProjectResourcePool&srcPool);
	void onUp( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg );
	void onMove( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg );
	void onDown( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg );
	void onResourceDClick( KView_PTR view );
	void onDownMem(kn_int x, kn_int y,KMessageMouse* msg);
	void OnMemDragEnd(KMessageDrag* msg);
	void OnMemDrag(KMessageDrag* msg);
	void onDragDirect( KMessageDrag* msg );
private:
	//背景
	KTextView_PTR m_img_backgroup;

	KViewGroup_PTR m_group_project;
	/**
	* 标题文本
	*/
	KTextView_PTR m_text_title;
	/**
	* 名称文本	
	*/
	KTextView_PTR m_text_name;
	KEditView_PTR m_edit_name;
	
	/**
	* 负责人文本	
	*/
	KTextView_PTR m_text_pm;
	KEditView_PTR m_edit_pm;

	/**
	* 起始时间文本	
	*/
	KTextView_PTR m_text_time_start;

	/**
	* 结束时间文本	
	*/
	KTextView_PTR m_text_time_end;

	/**
	* 资源	
	*/
	KTextView_PTR m_text_resource;
	KViewGroup_PTR	m_grou_resource;
	NGridView_PTR	m_view_satff_grid;
	NSlideView_PTR m_silder;

	//是否点击标题栏
	kn_bool		m_down_title;
	// 记录点击的坐标
	kn_int		m_down_x,m_down_y;

	//project data
	CNProjectData m_projectData;  //local data
	CNProjectResourcePool m_globalResource;  //local data (proj data+global data)

	CNProjectData* m_p_originalProjData;

	NDragStaffView_PTR				m_p_drag_view; //拖动时的虚影
	K9PatchImageDrawable_PTR p_pmedit_active; //pm编辑窗高亮

	/*
	 *	exit btn
	 */

	KImgButtonView_PTR m_p_btn_exit;
};

typedef boost::shared_ptr<NProjectResourceView> NProjectResourceView_PTR;
#endif

