//  File:        NProjectPage.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/2/25
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NTimeBarPage.h"
#include "boost/lexical_cast.hpp"
#include "NProjectPage.h"
#include "KSurfaceManager.h"
#include "KBitmapTextDrawable.h"
#include "KScreen.h"
#include "NPlanAppView.h"
#include "NProjectResourceView.h"
#include "NMessageBoxView.h"
#include "NPromptView.h"

#include "NMenu.h"
#include "../manager/KConfigManager.h"
#ifdef WIN32
#include "commdlg.h"
//句柄
extern HWND g_hWnd; 
#endif 

CProjectPage::CProjectPage(void)
{
	m_e_viewtype = KVIEW_MOVEABLE;
	m_active_page = 0;
}

void CProjectPage::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_up.connect(this, &CProjectPage::onKeyUp);
}


CProjectPage::~CProjectPage()
{
	m_ani_thread.Stop();

}


void CProjectPage::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{

}


void CProjectPage::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void CProjectPage::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}


void CProjectPage::init(CNProjectData* p)
{
	m_prj_data = *p;
	m_p_prj_data = p;

	m_active_page = TIMEBAR_PAGE;
	m_time_bar = CTimeBarPage_PTR(new CTimeBarPage);
	m_time_bar->Create(0,0 , m_rect.width(), m_rect.height() );
	m_time_bar->init(&m_prj_data);

	AddView( m_time_bar );

	m_time_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_current_page = m_time_bar;

	m_story_board =CStoryBoardPage_PTR(new CStoryBoardPage);
	m_story_board->Create(0, 0, m_rect.width(), m_rect.height());
	m_story_board->SleepPage();
	AddView(m_story_board);		
	m_story_board->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);

	m_story_man_board =CStoryBoardManPage_PTR(new CStoryBoardManPage);
	m_story_man_board->Create(0, 0, m_rect.width(), m_rect.height());
	m_story_man_board->SleepPage();
	AddView(m_story_man_board);	
	m_story_man_board->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);


	m_calendar_page =CCalendarPage_PTR(new CCalendarPage);
	m_calendar_page->Create(0, 0, m_rect.width(), m_rect.height());
	m_calendar_page->SleepPage();
	AddView(m_calendar_page);	
	m_calendar_page->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);


	m_prj_edit_page =CProjectEditPage_PTR(new CProjectEditPage);
	m_prj_edit_page->Create(0, 0, m_rect.width(), m_rect.height());
	m_prj_edit_page->SleepPage();
	AddView(m_prj_edit_page);	
	m_prj_edit_page->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);


	//////////////项目进度
	m_p_progress =KProcessView_PTR(new KProcessView);
	int process_w = 120;
	m_p_progress->Create(8, 70, process_w, 15);
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/process_alpha_bk.9.png")), TRUE) );
//	pDrawable->SizeToImage();
	pDrawable->setRect(0,0, process_w, 11 );
	m_p_progress->setBKDrawable(pDrawable);

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/process_alpha_a.9.png")), TRUE) );
//	pDrawable->SizeToImage();
	pDrawable->setRect(0,0, process_w, 11 );
	m_p_progress->setIconDrawable(pDrawable);

	m_p_progress->enableProcessClip(FALSE);
	m_p_progress->init( PROCESS_Horizontal );
	m_p_progress->SetTextBound( RERect::MakeXYWH(124, 0, 70, 15));
	m_p_progress->SetTextColor(RE_ColorWHITE);
	m_p_progress->SetFontSize(12);
	m_p_progress->showText(TRUE);

	//图片文字
	//KBitmapTextDrawable_PTR p_bmp_txt = KBitmapTextDrawable_PTR(new KBitmapTextDrawable(_T(""), RE_ColorWHITE, 20, REPaint::kLeft_Align));
	//p_bmp_txt->LoadProfile(_T("./resource/process_number_big.ini"));
	//m_p_progress->setTextDrawable(p_bmp_txt);
	//m_p_progress->init(PROCESS_Vertical);

	AddView(m_p_progress);
	m_p_progress->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	///// 项目名称
	REColor clr = SkColorSetARGBInline(255,255,255,255);
	m_proj_title = KEditView_PTR(new KEditView);  //editview可以换行
	m_proj_title->Create(5, 40, 300, 30);
	m_proj_title->Init();
//	m_proj_title->setMultiLine(true);
	m_proj_title->showText(TRUE);
	m_proj_title->SetTextColor(clr);
	m_proj_title->SetFontSize(20);
	m_proj_title->setReadonly(true);
	m_proj_title->showBK(FALSE);
	AddView(m_proj_title);
	m_proj_title->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	////////  按钮
	//// home
	KImgButtonView_PTR pView ;
	createImgViewHelper(&pView, _T("./resource/return_n.png"), _T("./resource/return_a.png"),_T("./resource/return_f.png"), m_rect.width() - 120, 15);
	pView->m_clicked_signal.connect(this, &CProjectPage::onHomeClick );
	pView->setTip(getStringManager()->GetStringByID(_T("back_prj")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//// save
	createImgViewHelper(&pView, _T("./resource/save_n.png"), _T("./resource/save_a.png"),_T("./resource/save_f.png"), 12, 15);
	pView->m_clicked_signal.connect(this, &CProjectPage::onSaveClick );
	pView->setTip(getStringManager()->GetStringByID(_T("save_prj")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//// saveas
	createImgViewHelper(&pView, _T("./resource/saveas_n.png"), _T("./resource/saveas_a.png"),_T("./resource/saveas_f.png"), 44, 15);
	pView->m_clicked_signal.connect(this, &CProjectPage::onSaveAsClick );
	pView->setTip(getStringManager()->GetStringByID(_T("saveas_prj")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	//// setting
	createImgViewHelper(&pView, _T("./resource/prj_setting_n.png"), _T("./resource/prj_setting_a.png"),_T("./resource/prj_setting_f.png"), 76, 15);
	pView->m_clicked_signal.connect(this, &CProjectPage::onSettingClick );
	pView->setTip(getStringManager()->GetStringByID(_T("setting_prj")),NO_TIMER,4000);
	pView->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_bottom_view =NBottomMenuView_PTR(new NBottomMenuView);
	m_bottom_view->Create( (m_rect.width() - 420)/2, m_rect.height()- 100, 420, 100);
	m_bottom_view->init();
	AddView(m_bottom_view);
	m_bottom_view->SetViewChangeType(KVIEW_LRMethod_Parent_LRMiddle, KVIEW_LRMethod_Parent_LRMiddle, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);
	//容器层，不阻塞消息
	m_bottom_view->SetShieldMsg(FALSE);
	refreshProjectData();
	m_prj_data.setModify(FALSE);
}

void CProjectPage::onHomeClick(KView_PTR p)
{
	if (!confirmModifyView())
	{
		return;
	}
	exitView();
	p->hideTip();
}

void CProjectPage::onSaveClick(KView_PTR)
{
	if (NO_ERROR != m_prj_data.savePrjFile())
	{
		MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("save_fail")).c_str(),
			getStringManager()->GetStringByID(_T("warning")).c_str(),MB_OK);
	}

	*m_p_prj_data = m_prj_data;
	m_prj_data.setModify(FALSE);
}

void CProjectPage::onSaveAsClick(KView_PTR p)
{
	CMenuSaveas_PTR menu = CMenuSaveas_PTR(new CMenuSaveas);
	menu->Create(50, 40, 480, 125);
	//注意传入点击的taskview
	menu->init(p );
	menu->m_sign_menu_view_click.connect(this, &CProjectPage::onSaveAs);
	showMenu(menu, 50, 40);
	InvalidateView();	
}

void CProjectPage::onSaveAs(KView_PTR v, kn_int n)
{//使用消息传递而不是直接调用saveas,是因为save会弹出文件选择模式对话框,导致收不到up消息,菜单不能隐藏
	releaseFocusView();
	KMessage* msg = new KMessage;
	msg->m_lParam = n;
	GetScreen()->sendNUIMessage(MSG_SAVE_AS, msg);
}

kn_bool CProjectPage::saveAsDetail(kn_string config_item, kn_string png_name, kn_string ext_name, kn_string & filepath)
{
	if (getConfigManager()->getIntConfig(config_item) == 0)
	{
		return true;
	}
	
	NPromptView_PTR prompt_view = NPromptView_PTR(new NPromptView());
	prompt_view->Create((m_rect.width() -500)/2, (m_rect.height() - 340)/2 + 50, 500, 340);
	AddView(prompt_view);

	prompt_view->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, 
		KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	prompt_view->Init(png_name);

	int r = prompt_view->doModal();
	if (prompt_view->IsDisplayPrompt())
	{
		getConfigManager()->setConfig(config_item, 1);
	}else
	{
		getConfigManager()->setConfig(config_item, 0);
	}

	if (r == KN_REUSLT_OK)
	{
		AddViewToDel(prompt_view);
		filepath = m_prj_data.getName() +  ext_name;
	}
	else
	{
		AddViewToDel(prompt_view);
		return false;	// cancel to save file
	}

	return true;
}

void CProjectPage::saveAs( kn_int n)
{
	kn_string filepath;
	
	switch(n)
	{
	case 0: // nprj
		filepath = m_prj_data.getName() +  _T(".nprj");
		break;
	case 1: // excel xls
		if (!saveAsDetail(_T("xls_save_as_prompt"), _T("xls_save_prompt.jpg"), _T(".xls"), filepath))
		{
			return;
		}
		break;
	case 2: // excel xml
		if (!saveAsDetail(_T("xls_save_as_prompt"), _T("xls_save_prompt.jpg"), _T(".xml"), filepath))
		{
			return;
		}
		break;
	case 3: // project
		if (!saveAsDetail(_T("xml_save_as_prompt"), _T("xml_save_prompt.jpg"), _T(".xml"), filepath))
		{
			return;
		}
		break;
	default:
		break;
	}

	bool b_save= FALSE;

#ifdef WIN32
	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	// lStructSize  
	// 指定这个结构的大小，以字节为单位。  
	// Windows 95/98和Windows NT 4.0：特意为Windows 95/98或Windows NT 4.0，及带有WINVER和_WIN32_WINNT >= 0x0500编译时，  
	//  为这个成员使用OPENFILENAME_SIZE_VERSION_400。  
	// Windows 2000及更高版本：这个参数使用sizeof (OPENFILENAME) 。  
	ofn.hwndOwner = GetForegroundWindow();// 打开OR保存文件对话框的父窗口  
	ofn.lpstrFilter = TEXT("All\0*.*\0NPlan Project\0*.nprj\0Xml\0*.xml\0Xls\0*.xls\0");//xls 需要确认本地安装了excel否则解析不成功
	//过滤器 如果为 NULL 不使用过滤器  
	//具体用法看上面  注意 /0  
	lstrcpy(szPathName, filepath.c_str());  
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区  
	ofn.lpstrTitle = TEXT("Save Project File");//选择文件对话框标题  
	TCHAR szCurDir[MAX_PATH];
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	ofn.lpstrInitialDir=szCurDir;//设置对话框显示的初始目录  
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn)) //调用对话框打开文件
	{
		b_save = TRUE;
		filepath = szPathName;
	}
#endif

	if (b_save)
	{
		kn_string str_template = getConfigManager()->getAppPath()+_T("template/nplan_template_cn.xls");
		switch (getConfigManager()->getLanguage())
		{
		case LAN_JP:
			str_template = getConfigManager()->getAppPath()+_T("template/nplan_template_jp.xls");
			break;
		case LAN_EN:
			str_template = getConfigManager()->getAppPath()+_T("template/nplan_template_en.xls");
			break;
		}

		int ret = NO_ERROR;
		switch(n)
		{
		case 0: // nprj
			ret =m_prj_data.saveAsPrjFile(filepath, XML_PRJ_FILE);
				break;
		case 1: // excel
			ret =m_prj_data.saveAsPrjFile(filepath, XLS_PRJ_FILE, (kn_char*)str_template.c_str() );
			break;
		case 2: // excel
			ret =m_prj_data.saveAsPrjFile(filepath, XLS_XML_FILE);
			break;
		case 3: // project
			ret =m_prj_data.saveAsPrjFile(filepath, MPP_XML_FILE);
			break;
		}

		if (ret != NO_ERROR)
		{
			MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("export_fail")).c_str(),
				getStringManager()->GetStringByID(_T("warning")).c_str(),MB_OK);
		}
		else
		{
			MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("export_ok")).c_str(),
				getStringManager()->GetStringByID(_T("warning")).c_str(),MB_OK);
		}
	}

	//GetScreen()->sendSystemMessage(WM_LBUTTONUP, 0, 0 );
}

void CProjectPage::onSettingClick(KView_PTR)
{
	NProjectResourceView_PTR pProjectRscView = NProjectResourceView_PTR(new NProjectResourceView(&m_prj_data) );
	pProjectRscView->initCtrl();
	AddView(pProjectRscView);
	pProjectRscView->setViewFocus();
	InvalidateView();
	pProjectRscView->doModal();

	m_proj_title->SetText(m_prj_data.getName() );
	if (m_prj_edit_page)
	{
		m_prj_edit_page->updateProjectResourceView();
	}
}

void  CProjectPage::refreshProjectData()
{
	{
		m_p_progress->setValue(m_prj_data.getDonePercentage());
		m_proj_title->SetText(m_prj_data.getName() );
	}
}

void CProjectPage::initAnimation()
{
//构造动画
	m_time_bar->initAnimation();
	m_bottom_view->initAnimation();
}

void CProjectPage::onKeyUp(KMessageKey* k)
{
	if ( k->m_key_id == VK_ESCAPE)
	{
		if (!confirmModifyView())
		{
			return;
		}
		exitView();
	}

	if(k->m_key_flag == KMSG_KEYFLAG_CONTROL)
	{
		if( k->m_key_id == 'S' ||  k->m_key_id == 's')
		{//ctrl+T 直接加task
			onSaveClick(k->m_p_view );
		}

		if ( k->m_key_id == '1')
		{
			switchPage(PROJECT_EDIT_PAGE);
		}
		else if( k->m_key_id == '2')
		{
			switchPage(TIMEBAR_PAGE);
		}
		else if( k->m_key_id == '3')
		{
			switchPage(STORYBOARD_PAGE);
		}
		else if( k->m_key_id == '4')
		{
			switchPage(STORYBOARD_MAN_PAGE);
		}
		else if( k->m_key_id == '5')
		{
			switchPage(CALENDAR_PAGE);
		}
	}

}

void CProjectPage::exitView()
{
	// 释放聚焦控件
	releaseFocusView();
	(VIEW_PTR(NPlanAppView)(m_p_parent.lock()) )->enterMainView( );

}

void CProjectPage::switchPage(kn_int id)
{
	if (id == m_active_page)
	{
		return;
	}
	if (m_ani_thread.isPlaying())
	{
		return;
	}

	KViewGroup_PTR new_page;
	switch (id)
	{
	case STORYBOARD_MAN_PAGE:

		if (m_current_page != m_story_man_board)
		{
			//数据可能更新，每次显示都重新加载
			m_story_man_board->init(&m_prj_data);
			m_story_man_board->Wakeup();
			new_page = m_story_man_board;
		}

		break;
	case STORYBOARD_PAGE:
		if (m_current_page != m_story_board)
		{
			//数据可能更新，每次显示都重新加载
			m_story_board->init(&m_prj_data);
			m_story_board->Wakeup();
			new_page = m_story_board;
		}
		
		break;
	case TIMEBAR_PAGE:

		if (m_current_page != m_time_bar)
		{
			m_time_bar->init(&m_prj_data);
			m_time_bar->Wakeup();
			new_page = m_time_bar;
		}
		break;
	case CALENDAR_PAGE:
		if (m_current_page != m_calendar_page)
		{
			m_calendar_page->init(&m_prj_data);
			m_calendar_page->Wakeup();
			new_page = m_calendar_page;
		}
		break;
		
	case PROJECT_EDIT_PAGE:
		if (m_current_page != m_prj_edit_page)
		{
			m_prj_edit_page->init(&m_prj_data);
			m_prj_edit_page->Wakeup();
			new_page = m_prj_edit_page;
		}
		break;
	}

	if (new_page)
	{
		int mov_width = -m_rect.width();
		if (id < m_active_page)
		{//向右滑
			mov_width = m_rect.width();
		}

		m_active_page = id;

		RERect r1 = m_current_page->GetRect();

		//先设置一下，否则可能会闪一下， 动画启动前被刷新一次
		new_page->SetPosition( r1.left()-mov_width , 0);

		m_ani_thread.addAnimationHelper(m_current_page, PropertyPARA_ID_POS_X, r1.left()+mov_width, 600, 0,0  );
		m_ani_thread.addAnimationHelper(new_page, PropertyPARA_ID_POS_X, r1.left()-mov_width, r1.left(), 600, 0,0  );
		m_ani_thread.setStopMsg(SWITCH_PAGE_ANI_STOP);

		m_old_page = m_current_page;
		m_current_page = new_page;
		m_ani_thread.Start();
	}
}

kn_bool  CProjectPage::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == SWITCH_PAGE_ANI_STOP)
	{
		if (m_old_page)
		{	
			m_old_page->SleepPage();
			if (m_old_page == m_story_man_board|| m_old_page == m_story_board || m_old_page == m_prj_edit_page || m_old_page == m_time_bar)
			{//story_board要释放数据 因为内部控件绑定的数据指针在编辑室时会被覆盖
				m_old_page->Release();
			}
		}
		return TRUE;
	}

	if (pMsg->m_msg_type == STORYPAGE_DATA_UPDATE)
	{//故事版数据改变，刷新项目进度数据
		refreshProjectData();
		InvalidateView();
		return TRUE;
	}
	else if (pMsg->m_msg_type == TIMEPAGE_TASK_EDIT)
	{//模式对话框弹出，隐藏底部条
		m_bottom_view->setSwitchButton(TRUE);
		return TRUE;
	}
	else if (pMsg->m_msg_type == MSG_SAVE_AS)
	{//模式对话框弹出，隐藏底部条
		saveAs(pMsg->m_lParam);
		return TRUE;
	}
	
	return FALSE;
}

void CProjectPage::releaseFocusView()
{
	KScreen* p =  GetScreen(); 
	KView_PTR v_null;
	if (p)
	{
		p->setFocusView(v_null, FALSE);
	}
}

kn_bool CProjectPage::confirmModifyView()
{
#ifndef ANDROID_NDK
	if (m_prj_data.isModify())
	{
		NMessageModalView_PTR view_msg = NMessageModalView_PTR(new NMessageModalView(getStringManager()->GetStringByID(_T("save_warning")),KMB_SELECT ));
		view_msg->Create(0,0, m_rect.width(), m_rect.height());
		K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png") ), TRUE ) );
		pDrawable->setRect(0,0,350, 150);
		view_msg->Init(pDrawable);
		AddView(view_msg);

		switch (view_msg->doModal())
		{
		case 2:
			return false;
			break;
		case 1:
			break;
		case 0:
			onSaveClick(view_msg);
			break;
		}
		return true;
	}
#endif
	return true;
}
