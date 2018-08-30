//  File:        NSettingView.h
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
#ifndef _NSettingView_H_
#define _NSettingView_H_
#include "KEditView.h"
#include "KSurfaceManager.h"
#include "NGridView.h"
#include "KCheckboxView.h"
#include "../file/NProjectResourcePool.h"
struct KRdoMsg
{
	kn_string strClickResPath; 
	kn_string strUnclickResPath;
	kn_string strMessage;
};
class NSettingView : public KViewGroup
{
public:
	NSettingView(RERect rect);
	~NSettingView(void);

	void init(RERect rect);
	void showAni(RERect rect);
	void setRdoResourse( const vector<KRdoMsg>& vec );
	void onBtnCancle(KView_PTR p);
	void onClickRdo( kn_int i );
	KRadioboxGroup_PTR	m_p_rdo_set;
protected:

	CAnimationThread m_ani_thread; 
	KImgButtonView_PTR m_p_btn_cancel;
};

typedef boost::shared_ptr<NSettingView> NSettingView_PTR;
#endif

