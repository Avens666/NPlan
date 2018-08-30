//  File:        NPublicSourceView.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/10/30
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NPublicSourceView.h"
#include "../manager/KStringManager.h"
#include "../manager/KProjectManager.h"

#ifdef WIN32
#include "commdlg.h"
#endif

#include "KScreen.h"
#define BUFF_SIZE 64
#define ID_MAX_LEN 8 //id最大长度

NPublicSourceView::NPublicSourceView(void)
{

	m_res_pool = NULL;
	iGridRow = 0;
	iGridCol = 0;
}


NPublicSourceView::~NPublicSourceView(void)
{
}

void NPublicSourceView::init(CNProjectResourcePool* pool)
{
	iGridRow = 5;
	iGridCol = 8;
	m_res_pool = pool;
	int i_btn_w = 100;
	int i_btn_h = 40;
	//初始化背景
	K9PatchImageDrawable_PTR drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/resource_bg.9.png") ),true) );
	drawable->setRect( 0,0 , m_rect.width(),  m_rect.height() );
	addDrawable(drawable);

	KStaticView_PTR head;
	createTextHelper( &head, getStringManager()->GetStringByID(_T("resource_edit")), 16, 20, 10, 200,  20 );
    head->setTextAlign(REPaint::kLeft_Align);

	//name
	KStaticView_PTR static_name_view;
	createTextHelper( &static_name_view, getStringManager()->GetStringByID(_T("name")), 16, 36, 70, 100,  20 );
	static_name_view->SetTextColor(0xff999999);

	createEditViewHelper(&m_edit_name, 16, 95, 58, 120, 40);
	K9PatchImageDrawable_PTR p_name  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
	p_name->SetRect(RERect::MakeWH(m_edit_name->GetRect().width(), m_edit_name->GetRect().height() ));
	K9PatchImageDrawable_PTR p_name_a  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text_a.9.png")),true));
	p_name_a->SetRect(RERect::MakeWH(m_edit_name->GetRect().width(), m_edit_name->GetRect().height() ));
	m_edit_name->Init(5,5,RE_ColorBLACK);
	m_edit_name->setBKDrawable(p_name);
	m_edit_name->setFocusDrawable(p_name_a);
	m_edit_name->SetCrossIndex(0);
	m_edit_name->setTextOff(2,2);
	m_edit_name->setCrossOff(4);
	m_edit_name->SetTextColor(0xff444444,0xff444444,0xff444444,0xff444444);
	//id
	KStaticView_PTR static_id_view;
	createTextHelper( &static_id_view, getStringManager()->GetStringByID(_T("id")), 16, 230, 70, 100,  20 );
	static_id_view->SetTextColor(0xff999999);
	createEditViewHelper(&m_edit_id, 16, 289, 58, 110, 40);
	K9PatchImageDrawable_PTR p_id  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
	K9PatchImageDrawable_PTR p_id_a  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(
		getSurfaceManager()->GetSurface(_T("./resource/public_resource_text_a.9.png")),true));
	p_id_a->SetRect(RERect::MakeWH(m_edit_id->GetRect().width(), m_edit_id->GetRect().height() ));
	p_id->SetRect(RERect::MakeWH(m_edit_id->GetRect().width(), m_edit_id->GetRect().height() ));
	m_edit_id->Init(5,5,RE_ColorBLACK);
	m_edit_id->setBKDrawable(p_id);
	m_edit_id->setFocusDrawable(p_id_a);
	m_edit_id->SetCrossIndex(0);
	m_edit_id->setTextOff(2,2);
	m_edit_id->setCrossOff(4);
	m_edit_id->SetTextColor(0xff444444,0xff444444,0xff444444,0xff444444);

	createImgViewHelper(&import_btn,
		_T("./resource/public_load_n.png"),
		_T("./resource/public_load_n.png"),
		_T("./resource/public_load_a.png"),
		585, 124);
	import_btn->setTip(getStringManager()->GetStringByID(_T("import_res")),NO_TIMER,4000);
	createImgViewHelper(&add_btn, 
		_T("./resource/public_add_n.png"),
		_T("./resource/public_add_n.png"),
		_T("./resource/public_add_a.png"),
		630, 124);
	add_btn->setTip(getStringManager()->GetStringByID(_T("add_res")),NO_TIMER,4000);
	createImgViewHelper(&del_btn,
		_T("./resource/public_delete_n.png"),
		_T("./resource/public_delete_n.png"),
		_T("./resource/public_delete_a.png"),
		675, 124);
	del_btn->setTip(getStringManager()->GetStringByID(_T("del_res")),NO_TIMER,4000);
	createImgViewHelper(&mod_btn,
		_T("./resource/public_edit_n.png"),
		_T("./resource/public_edit_n.png"),
		_T("./resource/public_edit_a.png"),
		720, 124);
	mod_btn->setTip(getStringManager()->GetStringByID(_T("mod_res")),NO_TIMER,4000);


	KStaticView_PTR res_view;
	createTextHelper( &res_view, getStringManager()->GetStringByID(_T("task_resource")), 16, 36, 130, 100,  20 );
	res_view->SetTextColor(0xff999999);

	//初始化人员列表
	m_grou_resource = KViewGroup_PTR(new KViewGroup);
	K9PatchImageDrawable_PTR p_groupBg  = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/public_resource_text.9.png")),true));
	p_groupBg->SetRect(RERect::MakeWH(m_rect.width() - 60, m_rect.height() - 230));
	m_grou_resource->Create(30, 155, m_rect.width() - 60, m_rect.height() - 230);
	m_grou_resource->addDrawable(p_groupBg );
	AddView(m_grou_resource);

	RERect rect;
	rect.setXYWH(10,5,m_grou_resource->GetBoundRect().width() - 40,m_grou_resource->GetBoundRect().height() - 10);
 	
	m_view_satffGrid = NGridView_PTR( new NGridView(rect.width() / iGridCol,rect.height() / iGridRow,rect) );
	m_view_satffGrid->Create(rect);
	m_grou_resource->AddView(m_view_satffGrid);

	UpdateStaffGridView();

	KImgButtonView_PTR ok_btn;
	KImgButtonView_PTR cacle_btn;
	createImgView9PatchHelper(&ok_btn,
		_T("./resource/btn_ok_n.9.png"),
		_T("./resource/btn_ok_a.9.png"),
		_T("./resource/btn_ok_n.9.png"),
		m_grou_resource->GetRect().right() - i_btn_w, m_grou_resource->GetRect().bottom() + 10, i_btn_w, i_btn_h);
	ok_btn->SetText( getStringManager()->GetStringByID(_T("ok")) );
	ok_btn->SetFontSize(14);
	ok_btn->SetTextBound(RERect::MakeXYWH(8,10,80,24));

	ok_btn->setStateChangeEnable(true);
	ok_btn->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
    ok_btn->setTextAlign(REPaint::kCenter_Align);

	createImgView9PatchHelper(&cacle_btn,
		_T("./resource/btn_cancel_n.9.png"),
		_T("./resource/btn_cancel_a.9.png"),
		_T("./resource/btn_cancel_n.9.png"),
		ok_btn->GetRect().left() - i_btn_w - 15, m_grou_resource->GetRect().bottom() + 10, i_btn_w, i_btn_h);
	cacle_btn->SetText( getStringManager()->GetStringByID(_T("cancel")) );
	cacle_btn->SetFontSize(14);
	cacle_btn->setStateChangeEnable(true);
	cacle_btn->SetTextColor(RE_ColorWHITE,RE_ColorWHITE,0xfffff000,RE_ColorWHITE);
	cacle_btn->SetTextBound(RERect::MakeXYWH(8,10,80,24));
    cacle_btn->setTextAlign(REPaint::kCenter_Align);

	KStaticView_PTR static_type_view;
	createTextHelper( &static_type_view, getStringManager()->GetStringByID(_T("res_type")), 16, 435, 70, 200,  20 );
	static_type_view->SetTextColor(0xff999999);
	//资源类型选择
	m_type =KRadioboxGroup_PTR( new KRadioboxGroup);
	m_type->Create(535, 52, 180, 50);
	KRadioboxView_PTR  radio;
	KImageDrawable_PTR da_boy  = KImageDrawable_PTR(new KImageDrawable(	getSurfaceManager()->GetSurface(_T("./resource/img_boy.png")),true));
	KImageDrawable_PTR da_girl  = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/img_girl.png")),true));
	KImageDrawable_PTR da_device = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/img_device.png")),true));
	kn_string str;

	radio =KRadioboxView_PTR( new KRadioboxView);
	radio->Create(0, 0, 60, 50);
	radio->setIconDrawable( da_boy );
	radio->setResourse( getSurfaceManager()->GetSurface(_T("./resource/check_ico.png")), getSurfaceManager()->GetSurface(_T("./resource/img_uncheck.png")),  str);
	m_type->AddView(radio);

	radio =KRadioboxView_PTR( new KRadioboxView);
	radio->Create(70, 0, 60, 50);
	radio->setIconDrawable( da_girl );
	radio->setResourse( getSurfaceManager()->GetSurface(_T("./resource/check_ico.png")), getSurfaceManager()->GetSurface(_T("./resource/img_uncheck.png")),  str);
	m_type->AddView(radio);

	radio =KRadioboxView_PTR( new KRadioboxView);
	radio->Create(140, 0, 60, 50);
	radio->setIconDrawable( da_device );
	radio->setResourse( getSurfaceManager()->GetSurface(_T("./resource/check_ico.png")), getSurfaceManager()->GetSurface(_T("./resource/img_uncheck.png")),  str);
	m_type->AddView(radio);
	m_type->SetSelectedIndex(0);
	AddView(m_type);

	//初始化游标
	KSlideView_PTR p_slide = KSlideView_PTR(new KSlideView );
	K9PatchImageDrawable_PTR normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_bk.9.png")),true));
	p_slide->setBKDrawable(normal);
	normal  = K9PatchImageDrawable_PTR(
		new K9PatchImageDrawable(getSurfaceManager()->GetSurface(_T("./resource/list_silder_cursor.9.png")),true));

	p_slide->setIconDrawable(normal);
	p_slide->Create( RERect::MakeXYWH(m_grou_resource->GetRect().width()- 20, 5,
		18, m_grou_resource->GetRect().height() - 10) );
	p_slide->init(PROCESS_Vertical);
	p_slide->showBK(TRUE);
	m_view_satffGrid->BindSlide(p_slide);
	m_grou_resource->AddView(p_slide);
	
	ok_btn->m_clicked_signal.connect(this,&NPublicSourceView::onBtnOk);
	cacle_btn->m_clicked_signal.connect(this,&NPublicSourceView::onBtnCancle);
	import_btn->m_clicked_signal.connect(this,&NPublicSourceView::onBtnImport);
	add_btn->m_clicked_signal.connect(this,&NPublicSourceView::onBtnAdd);
	del_btn->m_clicked_signal.connect(this,&NPublicSourceView::onBtnDel);
	mod_btn->m_clicked_signal.connect(this,&NPublicSourceView::onBtnModify);


	//�˳�
	KImgButtonView_PTR btn_exit;
	createImgViewHelper(&btn_exit,
		_T("./resource/btn_delete_n.png")
		,_T("./resource/btn_delete_a.png"),
		_T("./resource/btn_delete_f.png"),
		m_rect.width() - 45,3);
	btn_exit->m_clicked_signal.connect(this,&NPublicSourceView::onBtnCancle);

}
void NPublicSourceView::MoveGrid(kn_int iValue)
{
	float f = (float) iValue/(float)m_pSilderView->GetLimit();
	m_view_satffGrid->MoveTo(f);
}

/**
* 确定
*/
void NPublicSourceView::onBtnOk(KView_PTR p)
{
	endModal(KN_REUSLT_OK, TRUE);
} 

/**
* 取消
*/
void NPublicSourceView::onBtnCancle(KView_PTR p)
{
	endModal(KN_REUSLT_CANCLE, TRUE);
} 
/**
删除
*/
void NPublicSourceView::onBtnDel(KView_PTR p)
{
	if (m_p_select != NULL)
	{
		if(!m_res_pool->delResourceByID(m_p_select->GetId()))
		{
			MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("delete_fail")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), 0);
		}
		m_view_satffGrid->AddViewToDel(m_p_select);
		m_view_satffGrid->eraseView(m_p_select);
		m_p_select.reset();
		m_view_satffGrid->updateChildPos();
		if (m_view_satffGrid->getViewCount() <= (iGridRow * iGridCol))
		{
			m_view_satffGrid->resetCursorPos();
		}
		m_edit_name->SetText(_T(""));
		m_edit_id->SetText(_T(""));
		InvalidateView();
	}
}
/**
修改
*/
void NPublicSourceView::onBtnModify(KView_PTR p)
{
	if (m_p_select != NULL)
	{
		//name modify
		kn_string strName = m_edit_name->getText();
		kn_string recordName;
		m_p_select->GetName(recordName);

		//id modify
		kn_string strID = m_edit_id->getText();
		int id = _wtoi(strID.c_str());
		int recordId = m_p_select->GetId();

		//resourcetype modify
		int index = m_type->GetSelectedIndex();
		res_type t = (res_type)index;
		res_type recordType = m_p_select->GetResourceType();
		if (strName != recordName || id != recordId || t != recordType)
		{
			if (id != recordId &&  m_res_pool->checkResourceByID(id))//ID为“0”或者已经存在(不包括当前修改的)，提示错误
			{
				MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("id_exist")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), 0);
				return;
			}
			modifyResource(strName,t, recordId, id);
		}
		//文本框清零
		m_edit_name->SetText(_T(""));
		m_edit_id->SetText(_T(""));
		m_p_select->UnSelected();
		m_p_select.reset();
	}
}
void NPublicSourceView::modifyResource(kn_string& strName, res_type _type, int id, int newId)
{
	m_res_pool->modifyResource(strName, _type, id, newId);		
	m_p_select->SetResourceType(_type,false);
	m_p_select->SetName(strName);
	if (id != newId)
	{
		m_p_select->SetID(newId);
	}
}
/**
选中
*/
void NPublicSourceView::onSelected(KView_PTR p)
{
	m_p_select.reset(); 
	NStaffView_PTR pView = VIEW_PTR(NStaffView)(p);
	kn_int id = pView->GetId();
	kn_char Buff[BUFF_SIZE];
	memset(Buff,0,BUFF_SIZE * sizeof(kn_char));
	_itow(id, Buff, 10);
	kn_string strID = Buff;
	res_type type = pView->GetResourceType();
	if (type > res_device)
	{
		type = res_device;
	}
	kn_string strName = _T("");
	pView->GetName(strName);
	m_type->SetSelectedIndex(type);
	m_edit_name->SetText(strName);
	m_edit_id->SetText(strID);
	pView->OnSelected();
	m_p_select = pView;
}

void NPublicSourceView::onBtnImport(KView_PTR p)
{
	kn_string str = _T("");

#ifdef WIN32
	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };//or  {sizeof (OPENFILENAME)}    
	ofn.hwndOwner =GetForegroundWindow();// 打开OR保存文件对话框的父窗口  
	ofn.lpstrFilter = TEXT("CSV FILE(*.csv)\0*.csv");
	//过滤器 如果为 NULL 不使用过滤器   
	lstrcpy(szPathName, TEXT(""));  
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区  
	ofn.lpstrTitle = TEXT("Select Resource File");//选择文件对话框标题  
	TCHAR szCurDir[MAX_PATH];  
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	ofn.lpstrInitialDir=szCurDir;//设置对话框显示的初始目录  
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST |OFN_NOCHANGEDIR;
	_tprintf(TEXT("select file/n")); 
	if(GetOpenFileName(&ofn))//调用对话框打开文件
	{
		str = szPathName;
	}
#endif
	
	m_res_pool->loadCSVFile(str);
	UpdateStaffGridView();
}

void NPublicSourceView::onBtnAdd(KView_PTR p)
{
	res_type t = (res_type )m_type->GetSelectedIndex();
	kn_string strName = m_edit_name->getText();
	kn_string strID = m_edit_id->getText();
	int id = _wtoi(strID.c_str());
	if (strName.size() == 0)
	{
		MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("input_name")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), 0);
		if (m_p_select != NULL)
		{
			m_p_select->UnSelected();
			m_p_select.reset();
		}
		return;
	}
	else
	{
		if (m_res_pool->checkResourceByID(id))
		{
			//提示框
			MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("id_exist")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), 0);
			return;
		}else if (id <= 0)
		{
			MessageBox(GetScreen()->getWnd(), getStringManager()->GetStringByID(_T("id_error")).c_str(),getStringManager()->GetStringByID(_T("error")).c_str(), 0);
			return;
		}
		else
		{
			m_res_pool->addResource( strName, t, id);
			NStaffView_PTR pView = NStaffView_PTR( new NStaffView( strName, t,id) );
			pView->SetTextColor(0xff555555,0xff555555,0xff555555,0xff555555);
			pView->m_clicked_signal.connect(this,&NPublicSourceView::onSelected);
			m_view_satffGrid->AddView(pView);
		}	
		//文本框清零
		m_edit_name->SetText(_T(""));
		m_edit_id->SetText(_T(""));
		if (m_p_select)
		{
			m_p_select->UnSelected();
			m_p_select.reset();
		}
	}
} 

void NPublicSourceView::UpdateStaffGridView()
{
	m_view_satffGrid->Release();
	int count = m_res_pool->getCount();
	for (int i = 0 ; i < count; i++)
	{
		NStaffView_PTR pView = NStaffView_PTR( new NStaffView( m_res_pool->getResByIndex(i)->getName() ,m_res_pool->getResByIndex(i)->getType(),m_res_pool->getResByIndex(i)->getId()) );
		pView->SetTextColor(0xff555555,0xff555555,0xff555555,0xff555555);
		pView->m_clicked_signal.connect(this,&NPublicSourceView::onSelected);
		m_view_satffGrid->AddView(pView);
	}

}

