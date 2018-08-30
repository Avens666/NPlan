//  File:        NProjectResourceView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/28
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NProjectResourceView.h"
#include "../manager/KStringManager.h"
#include "../manager/KProjectManager.h"

extern	int g_iScreenWidth; // 屏幕宽度
extern	int g_iScreenHeight;  // 屏幕高度

NProjectResourceView::NProjectResourceView(CNProjectData* pProjectData)
{
	m_down_x=m_down_y=0;
	m_p_originalProjData = pProjectData;
	
	if (pProjectData->getTaskBoardItems().size()==0)
	{

			CNProjectTaskBoardItem board1;
			board1.setName(_T("Todo"));
			board1.setId(0);
			pProjectData->m_taskboard.addBoardItem( board1 );
			CNProjectTaskBoardItem board2;
			board2.setName(_T("In Process"));
			board2.setId(1);
			pProjectData->m_taskboard.addBoardItem( board2 );
			CNProjectTaskBoardItem board3;
			board3.setName(_T("Verify"));
			board3.setId(2);
			pProjectData->m_taskboard.addBoardItem( board3 );
			CNProjectTaskBoardItem board4;
			board4.setName(_T("Done"));
			board4.setId(3);
			pProjectData->m_taskboard.addBoardItem( board4 );

			//CNProjectTask task;
			//
			//task.setStartTime( _T("not a valid time "));
			//task.setEndTime( _T( "not a valid time "));
			//m_p_originalProjData->addTask( task);
	}
	
	m_projectData= *pProjectData;
	m_globalResource=*(getProjectManager()->getGlobalResource());

	m_down_title = false;
}

NProjectResourceView::~NProjectResourceView( void )
{

}

void NProjectResourceView::initCtrl()
{
	kn_int i_btn_w = 100;
	kn_int i_btn_h = 40;
	int iSlideWidth = 15;
	int iGridRow = 3;
	int i_grid_col = 6;
	int i_width = 700;		//窗口高度
	int i_height = 480;		//窗口宽度
	int i_title_height = 38;	//标题栏高度
	int i_text_width = 65;	//文本宽度;
	int i_text_x = 20;		//文本框left坐标
	int i_edit_height = 32;	//编辑框高度;
	int i_name_edit_width = 598; //项目名称编辑框宽度
	int i_pm_edit_width = 500; //项目经理编辑框宽度
	int i_start_edit_width = 300; //起始时间编辑框宽度
	int i_end_edit_width = 300; //结束时间编辑框宽度
	int i_pool_height = 300; //资源池查看框的高度
	int i_view_y = 0;
	//初始化背景
	K9PatchImageDrawable_PTR p_tsk_bg  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png")),true));
	m_group_project = KViewGroup_PTR(new KViewGroup);
	m_group_project->Create(RERect::MakeXYWH((g_iScreenWidth-i_width)/2,(g_iScreenHeight-i_height)/2,i_width,i_height));
	m_group_project->addDrawable(p_tsk_bg);
	AddView(m_group_project);

	//初始化标题
	m_text_title = KTextView_PTR(new KTextView);
	m_text_title->SetText(getStringManager()->GetStringByID(_T("project_info")));
    m_text_title->setTextAlign(REPaint::kLeft_Align);
	m_text_title->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE,RE_ColorWHITE);
	m_text_title->SetFontSize(18);
	m_text_title->Create(20,4,i_width - 46,i_title_height);
	m_group_project->AddView(m_text_title);
	m_text_title->enableMessage(false);
	i_view_y = 4 + i_title_height + 15;

	//project name and edit
	m_text_name =  KTextView_PTR(new KTextView);
	m_text_name->SetText(getStringManager()->GetStringByID(_T("name")));
    m_text_name->setTextAlign(REPaint::kLeft_Align);
	m_text_name->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_text_name->SetFontSize(14);
	m_text_name->Create(i_text_x,i_view_y,i_text_width,i_edit_height);
	m_text_name->enableMessage(false);
	m_group_project->AddView(m_text_name);
	m_edit_name = KEditView_PTR( new KEditView);
	K9PatchImageDrawable_PTR p_name  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
	p_name->SetRect(RERect::MakeWH(i_name_edit_width,i_edit_height));
	K9PatchImageDrawable_PTR p_name_a  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text_a.9.png")),true));
	p_name_a->SetRect(RERect::MakeWH(i_name_edit_width,i_edit_height));
	m_edit_name->Create(m_text_name->GetRect().right(),i_view_y,i_name_edit_width,i_edit_height);
	m_edit_name->Init(5,5,RE_ColorBLACK);
	m_edit_name->SetTextColor(0xff444444,0xff444444,0xff444444,0xff444444);;
	m_edit_name->setBKDrawable(p_name);
	m_edit_name->setFocusDrawable(p_name_a);
	m_edit_name->SetCrossIndex(0);
	m_edit_name->setTextOff(2,2);
	m_edit_name->setCrossOff(4);
	m_edit_name->SetText(m_projectData.getName());
	m_group_project->AddView(m_edit_name);
	i_view_y += i_edit_height+15;
	//start and end time
	if (!m_projectData.getProBeginTime().is_not_a_date_time())
	{
		m_text_time_start = KTextView_PTR( new KTextView);
		m_text_time_start->SetText(getStringManager()->GetStringByID(_T("task_resource_time_start"))+ _T("  ") + to_iso_extended_wstring(m_projectData.getProBeginTime().date()) );
		m_text_time_start->setTextAlign(REPaint::kLeft_Align);
		m_text_time_start->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
		m_text_time_start->SetFontSize(14);
		m_text_time_start->Create(i_text_x,i_view_y,(i_text_width+i_start_edit_width)/2,i_edit_height);
		m_text_time_start->enableMessage(false);
		m_group_project->AddView(m_text_time_start);

		m_text_time_end = KTextView_PTR(new KTextView);
		m_text_time_end->SetText(getStringManager()->GetStringByID(_T("task_resource_time_end")) + _T("  ") + to_iso_extended_wstring(m_projectData.getProEndTime().date()) );
		m_text_time_end->setTextAlign(REPaint::kLeft_Align);
		m_text_time_end->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
		m_text_time_end->SetFontSize(14);
		m_text_time_end->Create(m_text_time_start->GetRect().right(), i_view_y,(i_text_width+i_end_edit_width)/2,i_edit_height);
		m_text_time_end->enableMessage(false);
		m_group_project->AddView(m_text_time_end);

		i_view_y += i_edit_height+15;
	}

	//project manager name and edit
	m_text_pm =KTextView_PTR( new KTextView);
	m_text_pm->SetText(getStringManager()->GetStringByID(_T("task_owner")));
    m_text_pm->setTextAlign(REPaint::kLeft_Align);
	m_text_pm->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_text_pm->SetFontSize(14);
	m_text_pm->Create(i_text_x, i_view_y,i_text_width,i_edit_height);
	m_text_pm->enableMessage(false);
	m_group_project->AddView(m_text_pm);
	m_edit_pm = KEditView_PTR(new KEditView);
	K9PatchImageDrawable_PTR p_manager_name  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
	K9PatchImageDrawable_PTR p_manager_name_a  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text_a.9.png")),true));
	p_manager_name->SetRect(RERect::MakeWH((i_pm_edit_width - i_text_width)/2,i_edit_height));
	p_manager_name_a->SetRect(RERect::MakeWH((i_pm_edit_width - i_text_width)/2,i_edit_height));
	m_edit_pm->Create(m_text_pm->GetRect().right(), i_view_y,(i_pm_edit_width - i_text_width)/2,i_edit_height);
	m_edit_pm->Init(5,5,RE_ColorBLACK);
	m_edit_pm->SetTextColor(0xff444444,0xff444444,0xff444444,0xff444444);;
	m_edit_pm->setBKDrawable(p_manager_name);
	m_edit_pm->setFocusDrawable(p_manager_name_a);
	m_edit_pm->SetCrossIndex(0);
	m_edit_pm->setTextOff(2,2);
	m_edit_pm->setCrossOff(4);
	m_edit_pm->SetText(m_projectData.getLeader());
	m_edit_pm->setReadonly(true);
	i_view_y += i_edit_height+15;


	p_pmedit_active = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/member_bg_a.9.png")),true));
	p_pmedit_active->SetRect(RERect::MakeXYWH(0,0,m_edit_pm->GetRect().width(),m_edit_pm->GetRect().height()));
	p_pmedit_active->SetShow(FALSE);
	m_edit_pm->addDrawable(p_pmedit_active);
	m_group_project->AddView(m_edit_pm);

	//resource management
	//resource text
	m_text_resource = KTextView_PTR( new KTextView);
	m_text_resource->SetText(getStringManager()->GetStringByID(_T("project_res")));
    m_text_resource->setTextAlign(REPaint::kLeft_Align);
	m_text_resource->SetTextColor(0xff666666,0xff666666,0xff666666,0xff666666);
	m_text_resource->SetFontSize(14);
	m_text_resource->Create(i_text_x, i_view_y,i_text_width*4,i_edit_height);
	m_text_resource->enableMessage(false);
	m_group_project->AddView(m_text_resource);

	i_view_y += i_edit_height+2;

	//add people button
	KImgButtonView_PTR p_btn_add;
	m_group_project->createImgViewHelper(&p_btn_add,
		_T("./resource/btn_add_n.png")
		, _T("./resource/btn_add_n.png"),
		_T("./resource/btn_add_a.png"),
		m_edit_name->GetRect().right()-30, m_text_resource->GetRect().top() + 5);
	p_btn_add->m_clicked_signal.connect(this,&NProjectResourceView::onBtnAdd);

	//project resource pool 

	/** 添加一个用于滑动的人员*/
	m_p_drag_view = NDragStaffView_PTR( new NDragStaffView(_T(""),(res_type)0,0) );
	m_p_drag_view->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
	m_p_drag_view->setOpacity(0X88);
	m_p_drag_view->SetShow(FALSE);
	AddView(m_p_drag_view);
	m_grou_resource = KViewGroup_PTR(new KViewGroup);
	K9PatchImageDrawable_PTR draw_resource  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_people_bg.9.png")),true));
	draw_resource->SetRect(RERect::MakeWH(i_width - 46,i_pool_height));
	m_grou_resource->Create(23, i_view_y,i_width - 46,i_pool_height);
	m_grou_resource->addDrawable(draw_resource);
	m_group_project->AddView(m_grou_resource);
	RERect rect;
	rect.setXYWH(10,2,draw_resource->GetRect().width() - 20,draw_resource->GetRect().height() - 4);
	//gridview
	m_view_satff_grid = NGridView_PTR( new NGridView((rect.width()-20) / i_grid_col,rect.height() / iGridRow,rect) );
	m_view_satff_grid->Create(rect);
	m_view_satff_grid->m_sign_GridItemDClicked.connect(this,&NProjectResourceView::onResourceDClick);
	m_grou_resource->AddView(m_view_satff_grid);

	//init project resource pool
	int count = m_projectData.getResourcePool().getCount();
	int leaderID = m_projectData.getResourceIdByName(m_projectData.getLeader().c_str());
	for (int i = 0 ; i < count; i++)
	{
		CNProjectResource * tempRes = m_projectData.getResourcePool().getResByIndex(i);
		
		NStaffView_PTR pView = NStaffView_PTR( new NStaffView( tempRes->getName() ,tempRes->getType(),tempRes->getId()) );
		pView->SetTextColor(0xff444444,0xff444444,0xff444444,0xff444444);

		pView->m_sign_down.connect(this,&NProjectResourceView::onDownMem);
		pView->setLeader(leaderID == tempRes->getId());
		m_view_satff_grid->AddView(pView);
	}

	
	//初始化游标
	KSlideView_PTR p_slide = KSlideView_PTR(new KSlideView() );
	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_bk.9.png")),true));
	p_slide->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_cursor.9.png")),true));

	p_slide->setIconDrawable(normal);
	p_slide->Create( RERect::MakeXYWH( m_view_satff_grid->GetRect().width()-8,10,18,m_view_satff_grid->GetRect().height()  - 20));
	p_slide->init(PROCESS_Vertical);	
	p_slide->showBK(TRUE);
	m_view_satff_grid->BindSlide(p_slide);
	
	m_grou_resource->AddView(p_slide);


	//取消
	KImgButtonView_PTR p_btn_ok;
	m_group_project->createImgView9PatchHelper(&p_btn_ok,
		_T("./resource/btn_ok_n.9.png"),
		_T("./resource/btn_ok_a.9.png"),
		_T("./resource/btn_ok_n.9.png"),
		i_width - 23 - i_btn_w, m_grou_resource->GetRect().bottom()+13, i_btn_w, i_btn_h);
	p_btn_ok->SetText(getStringManager()->GetStringByID(_T("ok")));
	p_btn_ok->SetFontSize(14);
	p_btn_ok->setStateChangeEnable(true);
	p_btn_ok->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	p_btn_ok->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    p_btn_ok->setTextAlign(REPaint::kCenter_Align);
	p_btn_ok->m_clicked_signal.connect(this,&NProjectResourceView::onBtnOk);

	//ȷ��
	KImgButtonView_PTR p_btn_cancel;
	m_group_project->createImgView9PatchHelper(&p_btn_cancel,
		_T("./resource/btn_cancel_n.9.png"),
		_T("./resource/btn_cancel_a.9.png"),
		_T("./resource/btn_cancel_n.9.png"),
		p_btn_ok->GetRect().left()- 15  - i_btn_w, m_grou_resource->GetRect().bottom()+13, i_btn_w, i_btn_h);
	p_btn_cancel->SetText(getStringManager()->GetStringByID(_T("cancel")));
	p_btn_cancel->SetFontSize(14);
	p_btn_cancel->setStateChangeEnable(true);
	p_btn_cancel->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	p_btn_cancel->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    p_btn_cancel->setTextAlign(REPaint::kCenter_Align);
	p_btn_cancel->m_clicked_signal.connect(this,&NProjectResourceView::onBtnCancle);

	p_tsk_bg->SetRect(RERect::MakeWH(i_width,p_btn_ok->GetRect().bottom() + 30));
	m_group_project->changeViewLayerTop(m_grou_resource);

	m_group_project->m_sign_down.connect(this,&NProjectResourceView::onDown);
	m_group_project->m_sign_move.connect(this,&NProjectResourceView::onMove);
	m_group_project->m_sign_up.connect(this,&NProjectResourceView::onUp);


	//�˳�

	m_group_project->createImgViewHelper(&m_p_btn_exit,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		i_width - 45,3);

	m_p_btn_exit->m_clicked_signal.connect(this,&NProjectResourceView::onBtnCancle);
}


/**
* 确定
*/
void NProjectResourceView::onBtnOk(KView_PTR p)
{

	if (m_edit_name->getText().size()==0)
	{
		MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("prj_name_error")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), MB_OK);
		return;
	}
	m_projectData.setName(m_edit_name->getText().c_str());
	m_projectData.setLeader(m_edit_pm->getText());

	m_p_originalProjData->setResourcePool(m_projectData.getResourcePool());
	m_p_originalProjData->setName(m_edit_name->getText().c_str());
	m_p_originalProjData->setLeader(m_edit_pm->getText());

	endModal(KN_REUSLT_OK,TRUE);

	if (m_p_originalProjData->getFileName().empty())
	{
		kn_string filePath = _T("project/");
		filePath+=m_edit_name->getText();
		filePath+=_T(".nprj");
		m_p_originalProjData->setFileName(filePath);
		if (NO_ERROR!=m_p_originalProjData->savePrjFile())
		{
			MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("create_file_fail")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), MB_OK);
		}
		
	}
} 

/**
* 取消
*/
void NProjectResourceView::onBtnCancle(KView_PTR p)
{
	endModal(KN_REUSLT_CANCLE,TRUE);
} 


void NProjectResourceView::onBtnAdd( KView_PTR p )
{
	vector<CNProjectResource> vec_project_member = m_projectData.getResourcePool().getResources();

	vector<NMemberMsg>	vec_project_mem;

	CNProjectTask_LST* taskList=m_projectData.getpManTasks();
	
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

	vector<CNProjectResource>	vec_global_member =m_globalResource.getResources();

	vector<NMemberMsg>	vec_global_mem;
	for (vector<CNProjectResource>::iterator  itr = vec_global_member.begin(); 
		itr != vec_global_member.end()  ; ++itr)
	{
		vec_global_mem.push_back(NMemberMsg(*itr));
	}

	NMemberSourceView_PTR m_p_mem = NMemberSourceView_PTR( new NMemberSourceView(vec_global_mem, vec_project_mem) );
	m_p_mem->Create(0,0,g_iScreenWidth,g_iScreenHeight);
	m_p_mem->initCtrl();
	m_p_mem->m_sign_btn_ok.connect(this,&NProjectResourceView::UpdateResourceGrid);
	AddView(m_p_mem);

	m_p_mem->doModal();
}

void NProjectResourceView::UpdateResourceGrid(vector<NMemberMsg>& v )
{
	m_view_satff_grid->resetCursorPos();
	for (int i = m_view_satff_grid->getViewCount() - 1; i >= 0  ; i--)
	{
		m_view_satff_grid->AddViewToDel(m_view_satff_grid->getViewByIndex(i));
		m_view_satff_grid->eraseView(m_view_satff_grid->getViewByIndex(i));
	}
	
	std::vector <CNProjectResource >  NewResources;
	int offset =0;
	bool leaderStillExist=false;
	//OutputDebugString(_T("UpdateResourceGrid			: "));
	for (vector<NMemberMsg>::iterator it  = v.begin();
		it != v.end()  ; it++)
	{
		NStaffView_PTR pView = NStaffView_PTR( new NStaffView((*it)) );
		pView->SetTextColor(0xFF555555,0xFF555555,0xFF555555,0xFF555555);
		m_view_satff_grid->AddView(pView);
		pView->setCanLeader(true);
		pView->m_sign_down.connect(this,&NProjectResourceView::onDownMem);
		if (m_edit_pm->getText()==pView->GetName())
		{
			pView->setLeader(true);
			leaderStillExist=true;
		}
		//OutputDebugString(it->str_name.c_str());
	}
	if (leaderStillExist==false)
	{
		m_edit_pm->SetText(_T(""));
	}
	
	//OutputDebugString(_T("\n"));
	TranslateMemberListToResouceList(v,m_projectData.getResourcePool(),m_globalResource);
	
}

void NProjectResourceView::TranslateMemberListToResouceList(vector<NMemberMsg>& vecMemeber,CNProjectResourcePool& originalResourcePool,  CNProjectResourcePool& extraResourcePool )
{
	std::vector <CNProjectResource >  NewResources;
	int offset =0;
	//OutputDebugString(_T("TranslateMemberListToResouceList	: "));
	for (vector<NMemberMsg>::iterator it  = vecMemeber.begin();
		it != vecMemeber.end()  ; it++)
	{
		//OutputDebugString(it->str_name.c_str());
		
// 		CNProjectResource* pFoundResource = originalResourcePool.getResourceByID(it->id);
// 		if (pFoundResource!=NULL && pFoundResource->getName()==it->str_name)
// 		{
// 			CNProjectResource tempResource(*pFoundResource);
// 			NewResources.push_back(tempResource);
// 		}
// 		else
// 		{
// 			pFoundResource= extraResourcePool.getResourceByID(it->id);
// 			if (pFoundResource!=NULL)
// 			{
// 				CNProjectResource tempResource(*pFoundResource);
// 				tempResource.setId(offset + m_projectData.getResourcePool().getLargestResId());
// 				offset++;
// 				NewResources.push_back(tempResource);
// 			}
// 		}
		CNProjectResource* pFoundResource=NULL;
		int resourceID=originalResourcePool.findResourceID(it->str_name.c_str());
		//not in original pool
		if (resourceID == -1)
		{
			resourceID = originalResourcePool.getLargestResId();
			pFoundResource = extraResourcePool.getResourceByID(it->id);
			if (pFoundResource!=NULL)
			{
				CNProjectResource tempResource(*pFoundResource);
				tempResource.setId(offset + m_projectData.getResourcePool().getLargestResId());
				offset++;
				NewResources.push_back(tempResource);
			}
		}
		//in original pool
		else
		{
			pFoundResource = originalResourcePool.getResourceByID(resourceID);
			if (pFoundResource!=NULL)
			{
				CNProjectResource tempResource(*pFoundResource);
				NewResources.push_back(tempResource);
			}

		}
	}
	//OutputDebugString(_T("\n"));
	originalResourcePool.setResources(NewResources);
}

void NProjectResourceView::MergeResourceByName(CNProjectResourcePool& dstPool, CNProjectResourcePool&srcPool)
{
	vector<CNProjectResource> srcResourceVector = srcPool.getResources();
	for (vector<CNProjectResource>::iterator iter =srcResourceVector.begin();iter!=srcResourceVector.end();iter++)
	{
		if(-1 == dstPool.findResourceID(iter->getName().c_str()))
		{
			if (dstPool.checkResourceByID(iter->getId()))
			{
				iter->setId(dstPool.getLargestResId());
			}
			dstPool.addResource(*iter);
		}
	}
}

/**
* 标题移动相关
*/
void NProjectResourceView::onDown( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{	
	iScreenX -=	m_group_project->GetRect().left();
	iScreenY -=	m_group_project->GetRect().top();
	if (m_text_title->isPointInView(iScreenX,iScreenY))
	{
		m_down_title = true;
		m_down_x = iScreenX;
		m_down_y = iScreenY;
	}
}

/**
* 标题移动相关
*/
void NProjectResourceView::onMove( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{	
	if (m_down_title)
	{
		m_group_project->SetPosition(iScreenX - m_down_x ,iScreenY - m_down_y  );
		InvalidateView(true);
	}
}

/**
* 标题移动相关
*/
void NProjectResourceView::onUp( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	m_down_title = false;
}

void NProjectResourceView::onResourceDClick( KView_PTR view )
{
	NStaffView_PTR staff = VIEW_PTR(NStaffView) (view);
	if (staff==NULL)
	{
		return;
	}

	m_edit_pm->SetText(staff->GetName());
	
}

/**
 * 拖拽一个成员到负责人窗口
 */
void NProjectResourceView::onDownMem(kn_int x, kn_int y,KMessageMouse* msg)
{
	NStaffView_PTR pView = VIEW_PTR(NStaffView)(msg->m_p_view);
	kn_string str ;
	//������û������ ��������ק     cxc 2014-5-5
	//if (pView->getCanUpdate())
	{
		pView->GetName(str);
		m_p_drag_view->SetResourceType(pView->GetResourceType());
		m_p_drag_view->SetName(str);
		m_p_drag_view->OnDown(x,y,msg);
	}

}

void NProjectResourceView::OnMemDrag(KMessageDrag* msg)
{
	kn_int ix,iy,lx,ly;
	ix = msg->m_pos_x;
	iy = msg->m_pos_y;

	KView_PTR p = msg->m_p_drag_view;
	m_edit_pm->GetScreenXY(lx,ly);
	if (ix > lx && ix < lx + m_edit_pm->GetRect().width()&&
		iy > ly && iy < ly + m_edit_pm->GetRect().height())
	{
		ix -= lx;
		iy -= ly;

		p_pmedit_active->SetShow(true);
	}
	else
	{
		p_pmedit_active->SetShow(false);
	}
}
void NProjectResourceView::OnMemDragEnd(KMessageDrag* msg)
{
	kn_int ix,iy,lx,ly;
	ix = msg->m_pos_x;
	iy = msg->m_pos_y;
	KView_PTR p = msg->m_p_drag_view;
	m_edit_pm->GetScreenXY(lx,ly);
	if (ix > lx && ix < lx + m_edit_pm->GetRect().width()&&
		iy > ly && iy < ly + m_edit_pm->GetRect().height())
	{
		ix -= lx;
		iy -= ly;
		m_edit_pm->SetText((VIEW_PTR(NStaffView)(p))->GetName());
		p_pmedit_active->SetShow(FALSE);	

		for (int i = 0; i < m_view_satff_grid->getViewCount() ; i++)
		{
			NStaffView_PTR pview = VIEW_PTR(NStaffView)(m_view_satff_grid->getViewByIndex(i));
			pview->setLeader(pview->getMsg() ==  (VIEW_PTR(NStaffView)(p))->getMsg());
		}
		m_view_satff_grid->InvalidateView();
	}
	InvalidateView();
}

void NProjectResourceView::onDragDirect( KMessageDrag* msg )
{

	if (msg)
	{
		if (msg->m_drag_type == TASK_GROUP_DRAG_UP)
		{
			OnMemDragEnd(msg);
		}
		if (msg->m_drag_type == TASK_GROUP_DRAG)
		{
			OnMemDrag(msg);
		}

	}
}
