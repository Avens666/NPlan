//  File:        NPublicSourceView.h
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
#ifndef _NPublicSourceView_H_
#define _NPublicSourceView_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KEditView.h"
#include "KSurfaceManager.h"
#include "NGridView.h"
#include "NSilderCursorView.h"
#include "NSlideView.h"
#include "KCheckboxView.h"
#include "../file/NProjectResourcePool.h"

class NPublicSourceView : public KViewGroup
{
public:
	NPublicSourceView(void);
	~NPublicSourceView(void);
	void init(CNProjectResourcePool* );
	void MoveGrid(kn_int iValue);
	void UpdateStaffGridView();

	void onBtnCancle(KView_PTR p);
	void onBtnOk(KView_PTR p);
	void onBtnImport(KView_PTR p);
	void onBtnAdd(KView_PTR p);
	void onBtnDel(KView_PTR p);
	void onBtnModify(KView_PTR p);
	void onSelected(KView_PTR p);
	void modifyResource(kn_string& strName, res_type _type, int id, int newId);

protected:
	
	//人员列表
	KViewGroup_PTR	m_grou_resource;
	NGridView_PTR m_view_satffGrid;
	NSlideView_PTR m_pSilderView;
	
	KEditView_PTR m_edit_name;
	KEditView_PTR m_edit_id;
	KRadioboxGroup_PTR m_type;

	KImgButtonView_PTR import_btn;
	KImgButtonView_PTR add_btn;
	KImgButtonView_PTR del_btn;
	KImgButtonView_PTR mod_btn;

	CNProjectResourcePool* m_res_pool;


	NStaffView_PTR	m_p_select;

	int iGridRow;
	int iGridCol;
};
typedef boost::shared_ptr<NPublicSourceView> NPublicSourceView_PTR;
#endif

