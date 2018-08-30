//  File:        NMainView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/3
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef _NMAINVIEW_H_
#define _NMAINVIEW_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "NBottomView.h"
#include "NGridView.h"
#include "NProjectView.h"
#include "KSurfaceManager.h"
#include "NSwicthImagePage.h"
class NMainView : public KViewGroup
{
public:
	NMainView(void);
	~NMainView(void);

	void newProject();
	void openProject();
	void sourceManage();
	void setting();
	void about();
	void help();
	void init();

private:
	//初始化控件
		
	//标题栏按下
	void OnBtnTitleDown();
	//标题栏弹起
	void OnBtnTitleUp();
	//标题栏移动
	void OnBtnTitleMove(kn_int x,kn_int y , KMessageMouse* msg);

	void OnDClickProjectView(kn_int x,kn_int y , KMessageMouse* msg);
	void OnRClickProjectView(kn_int x,kn_int y , KMessageMouse* msg);
	void OnProjectMenu(KView_PTR, int id);
	virtual kn_bool OnUserMsg(KMessage* pMsg);


	void addProject(CNProjectData* );
	bool checkHistory(kn_string& strPath);

private:

	//大背景(1024*700)
	KTextView_PTR m_img_backgroup;

	//标题栏(1024*100)
	KTextView_PTR m_img_title_bar;

	//Icon
	KTextView_PTR m_img_icon;
	//Icon
	KTextView_PTR m_img_text;

	//底部控件
	NBottomView_PTR m_view_bottom;

	//项目列表区
	NGridView_PTR m_view_grid;
	//TODO

	int m_proj_view_w, m_proj_view_h;

	//正在编辑的project view
	NProjectView_PTR m_active_prj_view;

	NSwicthImagePage_PTR m_hellp_image_pages;

#ifdef WIN32
	kn_int m_iMoveWidth;
	kn_int m_iMoveHeight;
	kn_bool m_bMoveWindow;
#endif

};

typedef boost::shared_ptr<NMainView> NMainView_PTR;

#endif _NMAINVIEW_H_

