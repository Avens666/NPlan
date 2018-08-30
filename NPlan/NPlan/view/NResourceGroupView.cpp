//  File:        NResourceGroupView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/26
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NResourceGroupView.h"

const static kn_int i_grid_off = 2;			
const static  kn_int i_grid_off_x = 5;			
const static kn_int i_silder_off = 18;	
KResourceGroupView::KResourceGroupView()
{
	SetShieldMsg(true);
}


KResourceGroupView::~KResourceGroupView(void)
{
}

void KResourceGroupView::Init(kn_int iRow, kn_int iCol, vector<NMemberMsg>& vec_mem)
{
	std::copy(vec_mem.begin(),vec_mem.end(),std::back_inserter( m_vec_mem));
	// ����
	KColorDrawable_PTR draw_resource  = KColorDrawable_PTR(new KColorDrawable(ARGB(255, 234, 234, 234)));
	draw_resource->SetRect(RERect::MakeWH(m_rect.width(), m_rect.height()));
	draw_resource->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	addDrawable(draw_resource);

	// ��קview
	m_p_drag_view = NDragStaffView_PTR(new NDragStaffView(_T(""),(res_type)0,0) );
	m_p_drag_view->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
	m_p_drag_view->SetFontSize(16);
	AddView(m_p_drag_view);
	// ��͸��
	m_p_drag_view->setOpacity(0x88);
	// Ĭ������
	m_p_drag_view->SetShow(false);
	// ���ûص���Ϣ
	m_p_drag_view->setDragMsg(TASK_MEMBER_CONST_DRAG,TASK_MEMBER_CONST_DRAG_UP);

	// ����view
	RERect grid_rect = RERect::MakeXYWH(i_grid_off_x, i_grid_off, m_rect.width() - i_silder_off - 2 * i_grid_off_x,m_rect.height() - 2 * i_grid_off);
	m_view_grid = NGridView_PTR(new NGridView(grid_rect.width() / iCol, grid_rect.height() / iRow, grid_rect) );
	m_view_grid->Create(grid_rect);
	AddView(m_view_grid);

	// ��ʼ���α�
	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_bk.9.png")),true));
	m_p_slide =KSlideView_PTR(new KSlideView);
	m_p_slide->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_cursor.9.png")),true));
	m_p_slide->setIconDrawable(normal);
	m_p_slide->Create( RERect::MakeXYWH(grid_rect.right() + 1,grid_rect.top() +5,i_silder_off,grid_rect.height()  - 10));
	m_p_slide->init(PROCESS_Vertical);
	m_p_slide->showBK(TRUE);
	m_view_grid->BindSlide(m_p_slide);
	AddView(m_p_slide);

	// ��ʼ��Ԫ��
	if (m_view_grid)
	{	
		for (vector<NMemberMsg>::iterator itr = m_vec_mem.begin();
			itr != m_vec_mem.end();++itr)
		{
			NStaffView_PTR pView = NStaffView_PTR(new NStaffView((*itr)) );
			pView->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
			pView->setCanDBclick(false);
			pView->m_sign_down.connect(this, &KResourceGroupView::onDownMem);
			m_view_grid->AddView(pView);
		}
	}
}

void KResourceGroupView::onDownMem( kn_int x, kn_int y,KMessageMouse* msg )
{
	// ����ǰdown��ȥ��view���Ը���drag_view
	NStaffView_PTR pView = VIEW_PTR(NStaffView)(msg->m_p_view);
	kn_string str ;
	pView->GetName(str);
	m_p_drag_view->SetID(pView->GetId());
	m_p_drag_view->SetResourceType(pView->GetResourceType());
	m_p_drag_view->SetName(str);
	m_p_drag_view->OnDown(x,y,msg);
}

void KResourceGroupView::Release()
{
	KViewGroup::Release();
	m_vec_mem.clear();
}
