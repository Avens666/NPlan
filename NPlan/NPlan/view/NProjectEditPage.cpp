//  File:        NProjectEditPage.cpp
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
#include "boost/lexical_cast.hpp"
#include "NTaskSourceView.h"
#include "NMenu.h"
#include "NProjectEditPage.h"
using namespace boost;

#define  R_GRID_COL	 (2)			
#define	 R_GRID_ROW	 (6)

CProjectEditPage::CProjectEditPage(void)
{
	m_e_viewtype = KVIEW_MOVEABLE;

}

void CProjectEditPage::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &CProjectEditPage::onKeyUp);
}


CProjectEditPage::~CProjectEditPage()
{
}

void CProjectEditPage::init(CNProjectData* p)
{
	Release();
	m_prj_data = p;

	//顶部背景高度
	m_title_height = 100;
//
	KStaticView_PTR titlebk_view = KStaticView_PTR(new KStaticView);
	titlebk_view->Create(0, 0, m_rect.width(), m_title_height );
	titlebk_view->setBKGColor( ColorSetARGB(255,128,128,192) );
	AddView(titlebk_view);
	titlebk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//page 背景
	KStaticView_PTR bk_view = KStaticView_PTR(new KStaticView);
	bk_view->Create(0, m_title_height, m_rect.width(), m_rect.height() - m_title_height);
	bk_view->setBKGColor( ColorSetARGB(255, 29, 43, 62) );
	bk_view->showBK(TRUE);
	AddView(bk_view);
	bk_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	KStaticView_PTR titlebk_txt = KStaticView_PTR(new KStaticView);
	titlebk_txt->Create(m_rect.width()/2 - 100, 0, 300, 60 );
	titlebk_txt->SetText( _T("Project Edit") );
	titlebk_txt->SetFontSize(36);
	titlebk_txt->SetTextColor(  ColorSetARGB(255,245,245,245)  );
	AddView(titlebk_txt);
	titlebk_txt->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_prj_res_group = KResourceGroupView_PTR(new KResourceGroupView());
	RERect rectCR = RERect::MakeXYWH(0, m_title_height, 200, (m_rect.height() - m_title_height));
	m_prj_res_group->Create(rectCR);

    vector<NMemberMsg> vNm = getProjectResourceData();
    m_prj_res_group->Init(R_GRID_ROW, R_GRID_COL, vNm);
	
	m_prj_res_group->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_mindmap_view = NMindMapView_PTR(new NMindMapView);
	m_mindmap_view->Create(200, m_title_height, m_rect.width() - 200, m_rect.height() - m_title_height);
	m_mindmap_view->init(p);
	m_mindmap_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	AddView(m_mindmap_view);
	AddView(m_prj_res_group);
}

void CProjectEditPage::viewSizeChange()
{
	KViewGroup::viewSizeChange();

}

void CProjectEditPage::onKeyUp(KMessageKey* k)
{
	if ( k->m_key_id == 'A')
	{

	}
}

void CProjectEditPage::exitView()
{


}

kn_bool  CProjectEditPage::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == CALENDAR_DAY_CLICK)
	{
		return TRUE;
	}

	return FALSE;
}

void CProjectEditPage::updateProjectResourceView()
{
	if (m_prj_res_group)
	{
		m_prj_res_group->Release();
         vector<NMemberMsg> vNm = getProjectResourceData();
        m_prj_res_group->Init(R_GRID_ROW, R_GRID_COL,vNm);
	}
	
}

vector<NMemberMsg> CProjectEditPage::getProjectResourceData()
{
	vector<CNProjectResource> vec_project_member = m_prj_data->getResourcePool().getResources();
	vector<NMemberMsg>	vec_project_mem;
	CNProjectTask_LST* taskList=m_prj_data->getpManTasks();
	for (vector<CNProjectResource>::iterator  itr = vec_project_member.begin(); 
		itr != vec_project_member.end()  ; ++itr)
	{
		NMemberMsg member = NMemberMsg(*itr);
		for (CNProjectTask_LST::iterator taskIter=taskList->begin();taskIter!=taskList->end();taskIter++)
		{
			if (taskIter->getLeaderName()==itr->getName())
			{
				member.b_update=false;
			}
		}
		vec_project_mem.push_back(member);
	}
	return vec_project_mem;
}
