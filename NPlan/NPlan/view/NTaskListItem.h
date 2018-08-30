//  File:        NTaskListItem.h
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
#ifndef _NTaskListItem_H_
#define _NTaskListItem_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KSurfaceManager.h"
#include "NGridView.h"
#include "NSilderCursorView.h"
#include "NSlideView.h"
#include "KEditView.h"
#include "KScreen.h"
#include "K9PatchImageDrawable.h"
class NTaskListItem : public KViewGroup
{
public:
	NTaskListItem(KDrawable_PTR p);
	~NTaskListItem(void);
	void initCtrl();
	void setValue(kn_string str_name,kn_string str_people,kn_string str_time,kn_string str_state);
	void onMoveDirect(kn_int x, kn_int y, KMessageMouse* pMsg);
	void setTaskCardID(kn_int i);
	void btnDel(KView_PTR p);
	kn_int getTaskCardID();
	kn_string getTaskCardName();
	vector<kn_string> getMember();
	void  setMember(const kn_string& str);
	vector<kn_int> GetTaskTime();
	sigslot::signal1<kn_int> m_sign_del_card;
	void updateDrawable(KDrawable_PTR p);
	void showActive(kn_bool b);
	void onDragDirect( KMessageDrag* msg );
	void showDelete(kn_bool b);
private:
	KEditView_PTR		m_edit_name;
	KEditView_PTR		m_edit_mem;
	KEditView_PTR		m_edit_time;
	KEditView_PTR		m_edit_state;
	KImgButtonView_PTR	m_btn_delete;
	kn_int				m_i_card_id;
	K9PatchImageDrawable_PTR	m_bg_drawable;
};

typedef boost::shared_ptr<NTaskListItem> NTaskListItem_PTR;

#endif

