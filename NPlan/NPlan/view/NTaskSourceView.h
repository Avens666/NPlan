//  File:        NTaskSourceView.h
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
#ifndef _NTaskSourceView_H_
#define _NTaskSourceView_H_
#include "KButtonView.h"
#include "KSurfaceManager.h"
#include "NGridView.h"
#include "KEditView.h"
#include "KScreen.h"
#include "../file/NProjectTask.h"
#include "KListViewEx.h"
#include "NMemberSourceView.h"
#include "NSlideView.h"
#include "NDragStaffView.h"

class NTaskSourceView : public KViewGroup
{
public:
	NTaskSourceView( CNProjectTask* task );
	~NTaskSourceView(void);
	void init();
	void MoveGrid(kn_int iValue);


	/**
	* 重载消息
	*/
	virtual void onDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 

	sigslot::signal0<> m_sign_btn_ok;
	sigslot::signal1<CNProjectTask*> m_sign_btn_cancle;
	void onDelTaskCard(kn_int id);
private:
	/**
	* 统一设置字体的式样
	*/
	void setFontStyle(KTextDrawable_PTR& p);
	void onBtnCancle(KView_PTR p);
	void onBtnOk(KView_PTR p);
	void saveTaskData();
	void addMenber(KView_PTR );
	void addTaskCard(KView_PTR);
	void onEditMem(vector<NMemberMsg>& v);
	void onDownMem(kn_int x, kn_int y,KMessageMouse* msg);
	void onResourceDClick( KView_PTR view );
	void itemChange(kn_int i);
	void itemDel(vector<int>&);
private:
	//背景
	KTextView_PTR m_img_backgroup;


	CNProjectTask* m_p_task;
	KViewGroup_PTR m_group_task;
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
	* 开始时间文本	
	*/
	KTextView_PTR m_text_time_start;
	KEditView_PTR m_edit_time_start;
	/**
	* 结束时间文本	
	*/
	KTextView_PTR m_text_time_end;
	KEditView_PTR m_edit_time_end;
	/**
	* 资源	
	*/
	KTextView_PTR m_text_resource;
	KViewGroup_PTR	m_grou_resource;
	NGridView_PTR	m_view_satff_grid;
	NSlideView_PTR m_silder;

	KListViewEx_PTR m_list_task;

	//是否点击标题栏
	kn_bool		m_down_title;
	//记录点击的坐标
	kn_int		m_down_x,m_down_y;

	//添加按钮
	KImgButtonView_PTR			m_btn_add_mem;
	KImgButtonView_PTR			m_btn_add_card;
	NMemberSourceView_PTR		m_p_mem;
	VECSTAFF		m_vec_mem;

	vector<NMemberMsg>		m_vec_task_mem;

	NDragStaffView_PTR				m_p_drag_view;
	kn_int m_i_index ;	//�б�۽���
};

typedef boost::shared_ptr<NTaskSourceView> NTaskSourceView_PTR;

#endif

