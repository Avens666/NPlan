//  File:        NPromptView.h
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
#ifndef N_PROMPT_VIEW_H
#define N_PROMPT_VIEW_H

#include "KViewGroup.h"
#include "KCheckboxView.h"
#include "KButtonView.h"
#include "KImageDrawable.h"

class NPromptView :
	public KViewGroup
{
public:
	NPromptView(void);
	virtual ~NPromptView(void);
	void Init(kn_string image_name);
	void OnOkBtnClick(KView_PTR);
	void OnOkCancelClick(KView_PTR);
	void onPromptCheckBoxClick(KView_PTR);
	kn_bool IsDisplayPrompt();

protected:
	KCheckboxView_PTR m_check_box_view;
	KImageDrawable_PTR m_image_view;
	kn_bool m_display_prompt;
};
typedef boost::shared_ptr<NPromptView> NPromptView_PTR;
#endif //N_PROMPT_VIEW_H
