//  File:        NMemberSourceView.h
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
#ifndef _NMemberSourceView_H_
#define _NMemberSourceView_H_

#include "KSurfaceManager.h"
#include "NGridView.h"
#include "../file/NProjectTask.h"
#include "KButtonView.h"
#include "NDragStaffView.h"

class NMemberSourceView : public KViewGroup
{
public:
	NMemberSourceView( const vector<NMemberMsg>& _vec_const, vector<NMemberMsg>& _vec_child );
	~NMemberSourceView(void);
	void onDown( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg );
	void onMove( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg );
	void onUp( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg );
	void onConstDrag(KMessageDrag* msg);
	void onMemberDrag(KMessageDrag* msg);
	void swapView(NStaffView_PTR p,NGridView_PTR g);
	void onBtnOk(KView_PTR p);
	void onBtnCancle(KView_PTR p);
	void onCloseWindow();
	void dClickConst(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void dClickMem(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void UpdateSlider(NGridView_PTR g);
	void onDownConst(kn_int x, kn_int y,KMessageMouse* msg);
	void onDownMem(kn_int x, kn_int y,KMessageMouse* msg);
	void onConstDragEnd(KMessageDrag* msg);
	void onMemDragEnd(KMessageDrag* msg);
	void initCtrl();
	void onDragDirect( KMessageDrag* msg );
	/**
	* 重载消息
	*/
	sigslot::signal1<vector<NMemberMsg>&> m_sign_btn_ok;

private:
	KViewGroup_PTR	m_group_member;
	KImgButtonView_PTR		m_img_backgroup;
	// 标题文本
	KStaticView_PTR m_text_title;
	// 是否点击标题栏
	kn_bool		m_down_title;
	// 记录点击的坐标
	kn_int		m_down_x,m_down_y;
	// 添加面板
	KViewGroup_PTR	m_grou_resource;
	NGridView_PTR	m_view_mem_grid;
	NGridView_PTR	m_view_const_grid;

	KImgButtonView_PTR m_p_btn_cancel;
	KImgButtonView_PTR m_p_btn_ok;

	NDragStaffView_PTR		m_p_drag_mem_view;
	NDragStaffView_PTR		m_p_drag_const_view;

	KViewGroup_PTR	m_group_mem_resource;
	KViewGroup_PTR	m_group_const_resource;

	KSlideView_PTR m_p_slide;
	KSlideView_PTR m_p_slide2;

	K9PatchImageDrawable_PTR p_mem_bg_a;
	K9PatchImageDrawable_PTR p_cosnt_bg_a;

	vector<NMemberMsg> m_vec_const,m_vec_child;
};

typedef boost::shared_ptr<NMemberSourceView> NMemberSourceView_PTR;
#endif

