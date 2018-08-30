//  File:        NSwicthImagePage.h
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
#include "KViewGroup.h"
#include "KCheckboxView.h"
#include "KButtonView.h"
class NSwicthImagePage :
	public KViewGroup
{
public:
	NSwicthImagePage(void);
	virtual ~NSwicthImagePage(void);
	virtual void shared_ptr_inited(); 
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void onKeyUp(KMessageKey *kMsg);
	void Init(kn_string imagePathKeyValue, kn_int imageNum);

	// 上一个画面
	void onBtnLastImage(KView_PTR);
	// 下一个画面
	void onBtnNextImage(KView_PTR);
	// 到达最后一页
	virtual void arriveEndView();
	// 到达最后一页的前一页
	virtual void arriveLastEndView();

protected:
	CAnimationThread m_ani_thread;
	vector<KStaticView_PTR> m_images; //欢迎画面
	KRadioboxGroup_PTR m_radio_group;
	vector<KRadioboxView_PTR> m_radio_button;
	KImgButtonView_PTR m_last_page_btn;
	KImgButtonView_PTR m_next_page_btn;
	kn_int m_index_page;
};
typedef boost::shared_ptr<NSwicthImagePage> NSwicthImagePage_PTR;

////////////////////////////NWelcomeImagePage///////////////////////////////////////
class NWelcomeImagePage : public NSwicthImagePage
{
public:
	NWelcomeImagePage();
	virtual ~NWelcomeImagePage();
	void Init(kn_string imagePathKeyValue, kn_int imageNum);
	void arriveEndView();
	void arriveLastEndView();
	void OnEnterMainViewClick(KView_PTR);

protected:
	KImgButtonView_PTR m_enter_main_view_btn;
};

typedef boost::shared_ptr<NWelcomeImagePage> NWelcomeImagePage_PTR;



