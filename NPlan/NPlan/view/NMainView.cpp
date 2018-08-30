//  File:        NMainView.cpp
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
#include "NMainView.h"
#include "NProjectView.h"
#include "NProjectPage.h"
#include "../manager/KProjectManager.h"
#include "../manager/KConfigManager.h"
#include "KScreen.h"
#include "KSlideView.h"
#include "NPlanAppView.h"
#include "NPublicSourceView.h"
#ifdef WIN32
#include "commdlg.h"
#endif
#include "NProjectResourceView.h"
#include "NSettingView.h"
#include "NAboutView.h"
#include "NMenu.h"
#ifdef WIN32
//句柄
extern HWND g_hWnd; 
#endif 

//宏
#define  ICON_POS_X			(30)				//icon的X坐标
#define  ICON_POS_Y			(15)				//icon的Y坐标
#define  BOTTOMBAR_POS_X	(0)					//底部菜单栏的X坐标
#define  BOTTOMBAR_POS_Y	(587)				//底部菜单栏的Y坐标
#define  ICON_INTERVAL		(6)				//文字和Icon间的距离


NMainView::NMainView(void)
{
#ifdef WIN32
	m_bMoveWindow = false;
#endif
	m_proj_view_w = 294;
	m_proj_view_h = 416;

}


NMainView::~NMainView(void)
{

}

void NMainView::addProject(CNProjectData* p)
{
	NProjectView_PTR prov = NProjectView_PTR(new NProjectView);
	prov->Create(50,150, m_proj_view_w, m_proj_view_h);
	prov->init( p );
	prov->m_sign_dclick.connect(this, &NMainView::OnDClickProjectView);
	prov->m_sign_rup.connect(this, &NMainView::OnRClickProjectView);
	m_view_grid->AddView(prov);
}

//初始化控件
void NMainView::init()
{
	//大背景(1024*700)
//	createTextViewHelper(&m_img_backgroup,_T("./resource/backgroup.png"), 0, 0);

	int head_h = 60;

	//KImageDrawable_PTR bk_drawable = KImageDrawable_PTR(new KImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/bk1.png")),true) );
	//bk_drawable->SetFillType(KImageDrawable::FILL_TILE);
	//bk_drawable->setAutoSize(TRUE);
	//bk_drawable->setRect(0,0,m_rect.width(), m_rect.height());
	//bk_drawable->setTileModeXY(SkShader::TileMode::kMirror_TileMode, SkShader::TileMode::kMirror_TileMode);
	//bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	//addDrawable(bk_drawable);

	KColorDrawable_PTR bk_drawable = KColorDrawable_PTR(new KColorDrawable(ARGB(255, 25, 36, 25)) );
	bk_drawable->setRect(0,head_h, m_rect.width(), m_rect.height() - head_h);
	bk_drawable->setAutoSize(TRUE);

//	bk_drawable->setRect(0,0,m_rect.width(), m_rect.height());
 //   bk_drawable->setTileModeXY(SkShader::kMirror_TileMode, SkShader::kMirror_TileMode);
	bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	addDrawable(bk_drawable);

	KColorDrawable_PTR h_bk_drawable = KColorDrawable_PTR(new KColorDrawable(ARGB(255, 83, 155, 72)) );
	h_bk_drawable->setRect(0,0, m_rect.width(), head_h);
	h_bk_drawable->setAutoSize(TRUE);
	h_bk_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	addDrawable(h_bk_drawable);

	RERect rect;
	rect.setXYWH(0,head_h,  m_rect.width() - 24,  m_rect.height() - 150);	
	m_view_grid =NGridView_PTR(new NGridView(300,480,rect) );
	m_view_grid->setPreHeight(30);
	m_view_grid->Create(rect);
//	m_view_grid->SetClip(RERect::MakeXYWH(10,75, 1000, 525 ) );
	AddView(m_view_grid);

	

	for (int i = 0; i < getProjectManager()->getSize(); i ++ )
	{
		addProject(getProjectManager()->GetProjectByIndex(i) );
	}


	//初始化游标
	KSlideView_PTR p_slide = KSlideView_PTR(new KSlideView);
	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/staff_silder_bk.9.png")),true));
	p_slide->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/staff_silder_cursor.9.png")),true));

	p_slide->setIconDrawable(normal);
	p_slide->Create( RERect::MakeXYWH( m_rect.width() - 24, 75, 18, m_rect.height() - 200 ));

	p_slide->init(PROCESS_Vertical);
	m_view_grid->BindSlide(p_slide);
	AddView(p_slide);

	//底部控件
	m_view_bottom = NBottomView_PTR(new NBottomView);
	m_view_bottom->Create(0, m_rect.height() - 113, m_rect.width(), 113);
	m_view_bottom->init();
	AddView(m_view_bottom);

	//标题栏(1024*100)
//	createTextViewHelper(&m_img_title_bar,_T("./resource/title_bar.png"), 0, 0);
	//Icon
	createTextViewHelper(&m_img_icon,_T("./resource/title_icon.png"),ICON_POS_X,ICON_POS_Y);

	kn_string path;
	switch (getConfigManager()->getLanguage())
	{
	case LAN_CN:
		path = _T("./resource/title_text.png");
		break;
	case LAN_JP:
		path = _T("./resource/title_text_jp.png");
		break;
	default:
		path = _T("./resource/title_text_en.png");
		break;
	}
	//标题文字.
	createTextViewHelper(&m_img_text,path, 0, 0);

	//设置文字的坐标
	RERect rectIcon = m_img_icon->GetRect();
	RERect rectText = m_img_text->GetRect();
    RERect rectImg = RERect::MakeXYWH(rectIcon.right() + ICON_INTERVAL,rectIcon.bottom() - rectText.height() ,rectText.width(), rectText.height());
    m_img_text->Create(rectImg);

	m_img_icon->enableMessage(false);
	m_img_text->enableMessage(false);

	m_view_grid->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	p_slide->SetViewChangeType(KVIEW_LRMethod_Parent_Right, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
	m_view_bottom->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Bottom, KVIEW_BTMethod_Parent_Bottom);

//	m_img_title_bar->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);

	m_img_icon->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);
	m_img_text->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Left, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Top);


}

//初始化控件
void NMainView::OnBtnTitleDown()
{
#ifdef WIN32
	tagRECT rect;
	POINT point;
	GetWindowRect(g_hWnd,&rect);
	GetCursorPos(&point);
	m_iMoveWidth = point.x - rect.left;
	m_iMoveHeight = point.y - rect.top;
	m_bMoveWindow = TRUE;
#endif
}
void NMainView::OnBtnTitleUp()
{
#ifdef WIN32
	m_bMoveWindow = FALSE;
#endif
}
void NMainView::OnBtnTitleMove( kn_int x,kn_int y , KMessageMouse* msg )
{
#ifdef WIN32
	if (m_bMoveWindow)
	{
		POINT point;
		GetCursorPos(&point);
		::SetWindowPos(
			g_hWnd,
			HWND_TOP,
			point.x - m_iMoveWidth, 
			point.y - m_iMoveHeight, 
			0,
			0,
			SWP_NOZORDER|SWP_NOSIZE);
	}

#endif
}

void NMainView::OnDClickProjectView( kn_int x,kn_int y , KMessageMouse* msg )
{
	if (msg->m_p_view)
	{
		m_active_prj_view = VIEW_PTR(NProjectView)(msg->m_p_view);
		 
		(VIEW_PTR(NPlanAppView)(m_p_parent.lock()) )->enterProjectView( m_active_prj_view );

	}
}

void NMainView::OnRClickProjectView(kn_int x,kn_int y , KMessageMouse* msg)
{
	CMenuProject_PTR menu = CMenuProject_PTR(new CMenuProject );

	menu->Create(0,0, 150, 75);
	//注意传入点击的taskview
	menu->SetScreen(GetScreen());
	menu->init(msg->m_p_view, 1 );
	menu->m_sign_menu_view_click.connect(this, &NMainView::OnProjectMenu);
	//showmenu 传入view自身的相对坐标，注意projectview点击，传入的是projectview相对于m_view_grid的坐标，隔了一级，需要矫正，否则不是对其鼠标位置
	showMenu(menu,  x - m_rect.x() + m_view_grid->GetRect().x(), y - m_rect.y()+ m_view_grid->GetRect().y());
	InvalidateView();
}

void NMainView::OnProjectMenu(KView_PTR p , int id)
{
	if (id == 1)
	{//open
		(VIEW_PTR(NPlanAppView)(m_p_parent.lock()) )->enterProjectView( VIEW_PTR(NProjectView)(p) );
	}

	if (id == 2)
	{//del

		switch ( MessageBox(GetScreen()->getWnd(),getStringManager()->GetStringByID(_T("delete_warning")).c_str(),getStringManager()->GetStringByID(_T("warning")).c_str(),MB_YESNO) )
		{
		case IDNO:
			break;
		case IDYES:
			m_view_grid->AddViewToDel(p);
			knDeleteFile( (kn_char*)( VIEW_PTR(NProjectView)(p)->getProject()->getPrjFilePath().c_str() ) );
			break;
		}

	}
}

kn_bool  NMainView::OnUserMsg(KMessage* pMsg)
{
	if (pMsg->m_msg_type == PROJECTPAGE_DATA_UPDATE)
	{
		if (m_active_prj_view)
		{
			m_active_prj_view->refreshProjectData();
			m_active_prj_view->processAni();
			m_active_prj_view.reset();
		}
		
		return TRUE;
	}

	return FALSE;
}

void NMainView::newProject()
{
#ifdef WIN32
	tagRECT rect;
	GetWindowRect(g_hWnd,&rect);
#endif
	kn_int i_wight = 400;
	kn_int i_height = 200;
	CNProjectData* p = getProjectManager()->newProject();
	NProjectResourceView_PTR newProjResourceView = NProjectResourceView_PTR(new NProjectResourceView(p) );
	newProjResourceView->initCtrl();
	AddView(newProjResourceView);
	InvalidateView();
	newProjResourceView->setViewFocus();
	newProjResourceView->doModal();
	if (p->getName().size()==0)
	{
		getProjectManager()->removeProject(p);
	}
	else
	{
		addProject(p);
		
	}
	
	
}

void NMainView::openProject()
{
#ifdef WIN32
	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };//or  {sizeof (OPENFILENAME)}  
	// lStructSize  
	// 指定这个结构的大小，以字节为单位。  
	// Windows 95/98和Windows NT 4.0：特意为Windows 95/98或Windows NT 4.0，及带有WINVER和_WIN32_WINNT >= 0x0500编译时，  
	//  为这个成员使用OPENFILENAME_SIZE_VERSION_400。  
	// Windows 2000及更高版本：这个参数使用sizeof (OPENFILENAME) 。  
	ofn.hwndOwner =GetForegroundWindow();// 打开OR保存文件对话框的父窗口  
	ofn.lpstrFilter = TEXT("All\0*.*\0NPlan Project\0*.nprj\0Xml\0*.xml\0Xls\0*.xls\0");//xls 需要确认本地安装了excel否则解析不成功
		//过滤器 如果为 NULL 不使用过滤器  
		//具体用法看上面  注意 /0  
		lstrcpy(szPathName, TEXT(""));  
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区  
	ofn.lpstrTitle = TEXT("Open Project File");//选择文件对话框标题  
	TCHAR szCurDir[MAX_PATH];  
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	ofn.lpstrInitialDir=szCurDir;//设置对话框显示的初始目录  
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST |OFN_NOCHANGEDIR;
	_tprintf(TEXT("select file/n"));  

	if(GetOpenFileName(&ofn))//调用对话框打开文件
	{
		kn_string strPath = szPathName;
		if (checkHistory(strPath))
		{
			int res = NO_ERROR;
			CNProjectData* p = getProjectManager()->openProject(szPathName, res);
			if (res == NO_ERROR)//打开成功
			{
				getConfigManager()->addHistoryConfig(_T("historys"),strPath);
				addProject(p);
			}
			else if (res == ERROR_FILE_INVALID)//格式错误
			{
				MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("open_file_invalid")).c_str(),getStringManager()->GetStringByID(_T("open_fail")).c_str(), 0);
			}
			else//其他打开错误
			{
				MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("open_file_fail")).c_str(),getStringManager()->GetStringByID(_T("open_fail")).c_str(), 0);
			}
		}
	}
	InvalidateView();
#endif
}
bool NMainView::checkHistory(kn_string& strPath)
{
	vector<kn_string> openHistory;
	getConfigManager()->GetVectorConfig(_T("historys"),openHistory);
	//判断路径是否已经存在
	vector<kn_string>::iterator finditer = std::find(openHistory.begin(),openHistory.end(),boost::to_lower_copy(strPath));
	if (finditer == openHistory.end())
	{
		return true;
	}
	return false;
}
void NMainView::sourceManage()
{
#ifdef WIN32
	tagRECT rect;
	GetWindowRect(g_hWnd,&rect);
#endif
	kn_int i_wight = 800;
	kn_int i_height = 600;
	NPublicSourceView_PTR pView = NPublicSourceView_PTR(new NPublicSourceView);
    RERect rectView = RERect::MakeXYWH((m_rect.width() - i_wight)/2,(m_rect.height() - i_height)/2 , i_wight, i_height);
    pView->Create(rectView);
	CNProjectResourcePool* p = getProjectManager()->getGlobalResource();
	CNProjectResourcePool back_pool = *p;
	pView->init(&back_pool);
	AddView( pView );
	InvalidateView();
	pView->setViewFocus();
	if (pView->doModal() == KN_REUSLT_OK)
	{

		*p = back_pool;
		kn_string file = _T("./config/global.xml");

		getProjectManager()->getGlobalResource()->saveXMLFile( file );
	}
}

void NMainView::setting()
{
#ifdef WIN32
	tagRECT rect;
	GetWindowRect(g_hWnd,&rect);
#else
    tagRECT rect = {0,0,1024, 768};
#endif

	kn_int i_wight = 400;
	kn_int i_height = 200;
	RERect viewRect = RERect::MakeXYWH(((rect.right - rect.left) - i_wight)/2,
		(( rect.bottom - rect.top) - i_height)/2,i_wight,i_height);
	NSettingView_PTR pView = NSettingView_PTR(new NSettingView(viewRect) );
	pView->init(viewRect);

	AddView( pView );
	InvalidateView();
	pView->setViewFocus();
	pView->doModal();
}

void NMainView::about()
{
//	MessageBox(NULL, _T("NPlan 0.1 alpha 版本\n产品设计 开发:  陈治\n基于NUI 引擎开发\n武汉光庭信息科技有限公司 版权所有"), _T("about"), IDOK);
	NAboutView_PTR view = NAboutView_PTR (new NAboutView );

	view->Create(m_rect.width()/2 - 210, m_rect.height()/2 - 200, 480, 380);
	view->init();
	AddView(view);
	view->doModal();
}

void NMainView::help()
{
	if (m_hellp_image_pages)
	{
		AddViewToDel(m_hellp_image_pages);
		m_hellp_image_pages.reset();
	}
	else
	{
		kn_string strImageNum = getConfigManager()->getConfig(_T("help_image_mumber"));
		if (strImageNum==_T("0"))
		{
			return;
		}
		kn_int iImageNum = boost::lexical_cast<kn_int>(strImageNum);
		m_hellp_image_pages = NSwicthImagePage_PTR(new NSwicthImagePage());
		m_hellp_image_pages->Create(0, 60, m_rect.width(), m_view_bottom->GetRect().top() - 45);
		kn_string keyValue = _T("HelpImage");
		m_hellp_image_pages->Init(keyValue, iImageNum);
		AddView(m_hellp_image_pages);
		m_hellp_image_pages->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_Top, KVIEW_BTMethod_Parent_Bottom);
		InvalidateView();
	}
}

