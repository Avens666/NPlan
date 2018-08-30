//  File:        NBottomView.h
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
#ifndef _NBOTTOMVIEW_H_
#define _NBOTTOMVIEW_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KTextView.h"
#include "KSurfaceManager.h"
class NBottomView : public KViewGroup
{
public:
	NBottomView(void);
	~NBottomView(void);
	void GetBottomRect(RERect& rect);
	void init();
private:
	//初始化控件
	//封装创建的一些方法

	//设置控件的文本,X Y等于0的话则自动放在控件中间;
	void SetViewText( KImgButtonView_PTR view,const kn_string& str, kn_int x  = 0,kn_int y = 0);

	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);

	//按钮回调方法
	//"新建项目"
	void OnBtnCreate(KView_PTR);

	//"打开项目"
	void OnBtnOpen(KView_PTR);

	//"公共资源管理"
	void OnBtnPublic(KView_PTR);

	//"设置按钮"
	void OnBtnSetting(KView_PTR);

	//"使用说明"
	void OnBtnHelp(KView_PTR);

	//"关于"
	void OnBtnAbout(KView_PTR);

private:

	//底部栏
	KTextView_PTR		m_img_bottom_bar;

	//新建项目
	KImgButtonView_PTR m_btn_create;

	//打开项目
	KImgButtonView_PTR m_btn_open;

	//公共资源管理
	KImgButtonView_PTR m_btn_public;

	//设置按钮
	KImgButtonView_PTR m_btn_setting;

	//使用说明
	KImgButtonView_PTR m_btn_help;

	//关于
	KImgButtonView_PTR m_btn_about;

	KSurfaceManager* m_surfaceManager;
};

typedef boost::shared_ptr<NBottomView> NBottomView_PTR;
#endif

