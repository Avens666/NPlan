//  File:        NAboutView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/9/15
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef _NAboutVIEW_H_
#define _NAboutVIEW_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KTextView.h"
#include "KSurfaceManager.h"
class NAboutView : public KViewGroup
{
public:
	NAboutView(void);
	~NAboutView(void);
	void init();
private:
	//初始化控件
	//封装创建的一些方法

	//"关于"
	void OnBtnOK(KView_PTR);
	void OnBtnContact(KView_PTR);
	void OnBtnWeb(KView_PTR);

protected:
	int		  m_ver; //版本值标识
	kn_string m_title;	
	kn_string m_version;	
	kn_string m_product;	
	kn_string m_develop;
	kn_string m_engine;
	kn_string m_copyright;

	kn_string m_txt_path;
	kn_string m_contact;
};

typedef boost::shared_ptr<NAboutView> NAboutView_PTR;
#endif

