//  File:        NProjectView.cpp
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
#include "NProjectView.h"
#include "K9PatchImageDrawable.h"
#include "KBitmapTextDrawable.h"
#include "KSurfaceManager.h"
#include "KFontManager.h"
NProjectView::NProjectView(void)
{

}

NProjectView::~NProjectView(void)
{

}

//初始化控件
void NProjectView::init(CNProjectData* p )
{
	m_proj_data = p;
	//bk
	K9PatchImageDrawable_PTR pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/project_view_bk.9.png")), TRUE ) );
	pDrawable->setRect(0,0, m_rect.width(), m_rect.height() );
	addDrawable(pDrawable);

	//head
	kn_string color_no = boost::lexical_cast<kn_string>( (m_proj_data->getColor() )%3 +1);
	kn_string icon_file =  _T("./resource/project_view_head"); 
	icon_file += color_no;
	icon_file += _T(".9.png");

	IRESurface* sur = getSurfaceManager()->GetSurface( icon_file );
	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable(sur, TRUE) );
	//动态获取主题色
	REColor prj_color = ARGB(255, 247, 146, 39);
	if (sur)
	{
		prj_color = sur->getColor( sur->GetWidth()/2, sur->GetHeight()/2);
	}
	pDrawable->setRect(4,5, m_rect.width()-10, 40 );
	addDrawable(pDrawable);



	//project name
	//m_proj_title = KTextDrawable_PTR(new KTextDrawable(_T(""), RE_ColorWHITE, 18, REPaint::Align::kCenter_Align));
	//m_proj_title->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	//m_proj_title->setRect(5,8, m_rect.width()-10, 40 );
	//addDrawable( m_proj_title );	
    createTextDrawableHelper(&m_proj_title, _T(""), 18, 5, 8, m_rect.width()-10, 40, RE_ColorWHITE, REPaint::kCenter_Align );


	//////////////进度条
	//m_p_progress = KProcessView_PTR(new KProcessView);
	//m_p_progress->Create(20, 56, 45, 260);
	//pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/process_bk.9.png")), TRUE) );
	//pDrawable->setRect(0,0, 44, 260 );
	//m_p_progress->setBKDrawable(pDrawable);
	////前景色
	// icon_file =  _T("./resource/process_"); 
	//icon_file += boost::lexical_cast<kn_string>( (m_proj_data->getColor() )%6 +1);
	//icon_file += _T(".9.png");

	//pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(icon_file), TRUE) );
	//pDrawable->setRect(1,1, 42, 258 );
	//m_p_progress->setIconDrawable(pDrawable);
	////图片文字
	//KBitmapTextDrawable_PTR p_bmp_txt = KBitmapTextDrawable_PTR(new KBitmapTextDrawable(_T(""), RE_ColorBLACK, 16, REPaint::kLeft_Align));
	//p_bmp_txt->LoadProfile(_T("./resource/process_number.ini"));
	//m_p_progress->setTextDrawable(p_bmp_txt);
	//m_p_progress->init(PROCESS_Vertical);

	m_p_progress = KProcessCirleView_PTR(new KProcessCirleView);
	int prc_width = 120;
	m_p_progress->Create( (m_rect.width()-prc_width)/2, 60, prc_width, prc_width);
	m_p_progress->init( prj_color );
	AddView(m_p_progress);



	//////////leader
	//静态文字
	KTextDrawable_PTR txt_drawable;
	kn_string str = _T("PM: ");
	createTextDrawableHelper(&txt_drawable, str, 18, 30, 203, 60, 20 );


	// PM名 
	createTextHelper(&m_lead_view, str, 18, 80, 203, 150, 20);
	m_lead_view->SetTextColor( RE_ColorBLACK );

	//line
	K9PatchImageDrawable_PTR img_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/pri_line.9.png")), TRUE) );
	img_drawable->setRect(29, 227, 240, img_drawable->GetRect().height());
	//	p_img_drawable->SetFillType( p_img_drawable->FillType::FILL_TILE);
	addDrawable(img_drawable);

	//
	KTextDrawable_PTR info_txt;

	///// info multi line txt

	REColor info_clr = ARGB(255,150,150,150);
	// 项目子精度静态信息
	createTextDrawableHelper(&info_txt, _T("Undo:"),  14, 27, 243, 50, 20, info_clr );
	createTextDrawableHelper(&info_txt, _T("Doing:"), 14, 27, 266, 50, 20, info_clr );
	createTextDrawableHelper(&info_txt, _T("Done:"),  14, 27, 289, 50, 20, info_clr );

	//3个进度条
	prc_width = 158;
	m_undo_process = KProcessView_PTR(new KProcessView());
	m_undo_process->Create( 80, 247, prc_width, 9);
	m_doing_process = KProcessView_PTR(new KProcessView());
	m_doing_process->Create( 80, 270, prc_width, 9);
	m_done_process = KProcessView_PTR(new KProcessView());
	m_done_process->Create( 80, 293, prc_width, 9);

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/prj_info_bar_bk.9.png")), TRUE) );
	pDrawable->setRect(0,0, prc_width, 9 );
	m_undo_process->setBKDrawable(pDrawable);

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/prj_info_bar_bk.9.png")), TRUE) );
	pDrawable->setRect(0,0, prc_width, 9 );
	m_doing_process->setBKDrawable(pDrawable);

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/prj_info_bar_bk.9.png")), TRUE) );
	pDrawable->setRect(0,0, prc_width, 9 );
	m_done_process->setBKDrawable(pDrawable);

	//前景色
	icon_file =  _T("./resource/prj_info_bar_"); 
	icon_file += color_no;
	icon_file += _T(".9.png");

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(icon_file), TRUE) );
	pDrawable->setRect(0,0, 10, 9 );
	m_undo_process->setIconDrawable(pDrawable);

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(icon_file), TRUE) );
	pDrawable->setRect(0,0, 10, 9 );
	m_doing_process->setIconDrawable(pDrawable);

	pDrawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(icon_file), TRUE) );
	pDrawable->setRect(0,0, 10, 9 );
	m_done_process->setIconDrawable(pDrawable);

	m_undo_process->enableProcessClip(FALSE);
	m_undo_process->init( PROCESS_Horizontal );
	AddView(m_undo_process);
	m_doing_process->enableProcessClip(FALSE);
	m_doing_process->init( PROCESS_Horizontal );
	AddView(m_doing_process);
	m_done_process->enableProcessClip(FALSE);
	m_done_process->init( PROCESS_Horizontal );
	AddView(m_done_process);
	///////////////////////////////

	/////////三个文本信息
	info_clr = ARGB(255,110,110,110);
    createTextDrawableHelper(&m_undo_txt,  _T(""),	14, 220,  243, 40, 20, info_clr, REPaint::kRight_Align );
    createTextDrawableHelper(&m_doing_txt, _T(""),	14, 220,  266, 40, 20, info_clr, REPaint::kRight_Align );
    createTextDrawableHelper(&m_done_txt,  _T(""),	14, 220,  289, 40, 20, info_clr, REPaint::kRight_Align );

	//line
	img_drawable = K9PatchImageDrawable_PTR(new K9PatchImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/pri_line.9.png")), TRUE) );
	img_drawable->setRect(29, 315, 240, img_drawable->GetRect().height());
	//	p_img_drawable->SetFillType( p_img_drawable->FillType::FILL_TILE);
	addDrawable(img_drawable);

	///// member
	//p_img_drawable = KImageDrawable_PTR(new KImageDrawable( getSurfaceManager()->GetSurface(_T("./resource/group_icon.png")), TRUE) );
	//p_img_drawable->SizeToImage();
	//p_img_drawable->setRect(80, 205, p_img_drawable->GetRect().width(), p_img_drawable->GetRect().height() );
	//addDrawable(p_img_drawable);


	m_member_view = KEditView_PTR(new KEditView);  //editview可以换行
	m_member_view->Create(27, 324, 238, 75);
	m_member_view->Init();
	m_member_view->setMultiLine(true);
	m_member_view->showText(TRUE);
	m_member_view->showBK(FALSE);
	m_member_view->setReadonly(true);
	m_member_view->SetFontSize(13);
	m_member_view->setRowInterval(3);
	m_member_view->SetTextColor(prj_color);
	AddView(m_member_view);

	icon_file =  _T("./resource/project_view_frame");
	icon_file += color_no;
	icon_file += _T(".9.png"); 
	createImageHelper(&m_frame_light, icon_file, -20, -20);
	m_frame_light->setRect(4, 44, m_rect.width()-10, m_rect.height() - 47);
	m_frame_light->SetShow(FALSE);

	//刷新数据参数
	refreshProjectData();

	SetScalePoint(m_rect.width()/2, m_rect.height()/2 );
}


void  NProjectView::refreshProjectData()
{
	if (m_proj_data)
	{
		m_p_progress->setValue(m_proj_data->getDonePercentage());
		m_lead_view->SetText( m_proj_data->getLeader() );

		int i_undo  = m_proj_data->getTaskCardCount(0);
		int i_doing = m_proj_data->getTaskCardCount(1) + m_proj_data->getTaskCardCount(2);
		int i_done  = m_proj_data->getTaskCardCount(3);
		int i_total = i_undo + i_doing + i_done ;
		if (i_total == 0)
		{
			m_undo_process->setValue(0);
			m_doing_process->setValue(0);
			m_done_process->setValue(0);
		}
		else
		{
			m_undo_process->setValue(i_undo*100/i_total);
			m_doing_process->setValue(i_doing*100/i_total);
			m_done_process->setValue(i_done*100/i_total);
		}

		m_undo_txt->SetText( boost::lexical_cast<kn_string>(i_undo) );
		m_doing_txt->SetText( boost::lexical_cast<kn_string>(i_doing) );
		m_done_txt->SetText( boost::lexical_cast<kn_string>(i_done) );

		m_proj_title->SetText(m_proj_data->getName() );
		m_member_view->SetText(m_proj_data->getResMembersNameStr(TRUE) );
	}

}

CNProjectData* NProjectView::getProject()
{
	return m_proj_data;
}

void NProjectView::onDownDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	static int w = 1;

	//RERect rect = RERect::MakeXYWH( m_rect.left()- w, m_rect.top()-w* m_rect.height()/m_rect.width(), m_rect.width()+2*w, m_rect.height()+2*w*m_rect.height()/m_rect.width() );
	//SetBoundRect(rect);

	//rect = m_lead_view->GetRect();
	//RERect rect2 = RERect::MakeXYWH( rect.left()- w, rect.top()-w* rect.height()/rect.width(), rect.width()+2*w, rect.height()+2*w*rect.height()/rect.width() );
	//m_lead_view->SetBoundRect(rect2);

	//InvalidateView();
	//if (pMsg->m_wParam & MK_CONTROL)
	//{
	//	w--;
	//}
	//else
	//{
	//	w++;
	//}
}

void NProjectView::onMoveDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	//if (getState() == BS_PRESSED)
	//{//点击按下状态，可不处理聚焦消息
	//	if (m_b_mouse_in_view && !rectBound.contains(iScreenX, iScreenY))   //鼠标离开按钮 
	//	{
	//		m_b_mouse_in_view = false;   //清除进入标志
	//	}
	//	return;
	//}

	{
		if(!m_b_mouse_in_view && isPointInView(iScreenX, iScreenY))    //鼠标进入按钮
		{
			m_b_mouse_in_view = true;    //设置进入标志
			mouseIn();

		}
		else if (m_b_mouse_in_view && !isPointInView(iScreenX, iScreenY))   //鼠标离开按钮 
		{
			m_b_mouse_in_view = false;   //清除进入标志
			mouseOut();

		}
	}

}

void NProjectView::mouseIn()
{
	//if (m_ani_thread.isPlaying())
	//{
	//	m_ani_thread.Stop();
	//}

	//m_ani_thread.clearAnimation();

	////m_ani_thread.addAnimationHelper(shared_from_this(), PropertyPARA_ID_ScaleX, 1.1, 150,0,0);
	////m_ani_thread.addAnimationHelper(shared_from_this(), PropertyPARA_ID_ScaleY, 1.1, 150,0,0);
	//m_ani_thread.addAnimationHelper(m_frame_light, PropertyPARA_ID_Opacity, 220, 400,0,0);
	//m_ani_thread.Start();

	m_frame_light->SetShow(TRUE);
	InvalidateView();
}

void NProjectView::mouseOut()
{
	m_frame_light->SetShow(FALSE);
	InvalidateView();
}

void NProjectView::processAni()
{
	kn_uint v = m_proj_data->getDonePercentage();

	if (v == m_p_progress->getValue())
	{//没改变不用动画
		return;
	}

	if (m_ani_process_thread.isPlaying())
	{
		m_ani_process_thread.waitAnimationStop();
	}
	
	m_ani_process_thread.clearAnimation();

	//CNUIAnimation* pNUIAnimation = new CNUIAnimation (900, 200, 0);
	//CPropertyPara* pPDst = new CPropertyPara(this,  PARA_ID_PROCESS, v);
	//CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPDst);
	//pNUIAnimation->addAnimationData(pAnimationData);
	//m_ani_process_thread.addAnimation(pNUIAnimation);

	m_ani_process_thread.addAnimationHelper(shared_from_this(),  PARA_ID_PROCESS, v,  900, 200, 0);

	m_ani_process_thread.setFrameTime(100);
	m_ani_process_thread.Start();
}


kn_bool  NProjectView::isChange(kn_int para_id, kn_double v)
{
	if (para_id == PARA_ID_PROCESS)
	{
		kn_int i_v = v;
		return  (m_p_progress->getValue() != i_v) ;
	}
	return KView::isChange(para_id, v);
}

void  NProjectView::doChange(kn_int para_id, kn_double v)
{
	 KView::doChange(para_id, v);
	if (para_id == PARA_ID_PROCESS)
	{
		kn_int i_v = v;
		m_p_progress->setValue(i_v);
	}
}

kn_double  NProjectView::getParaValue(kn_int para_id)
{
	if (para_id == PARA_ID_PROCESS)
	{
		return m_p_progress->getValue();
	}
	return KView::getParaValue(para_id);
}
