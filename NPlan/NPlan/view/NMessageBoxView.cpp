//  File:        NMessageBoxView.cpp
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
#include "NMessageBoxView.h"
#include "../manager/KStringManager.h"

#define MSG_FONT_CLR		0xffffffff
#define MSG_FONT_CLR_A		0xfffff000
#define  MSG_BUTTON_W   75
#define  MSG_BUTTON_H	28
NMessageBoxView::NMessageBoxView( const kn_string& s,KMB_TYPE t)
{
	m_str_text = s;
	m_type = t;
	m_i_index = 0;
}


NMessageBoxView::~NMessageBoxView(void)
{
}

//��ʼ���ؼ�
void NMessageBoxView::Init(KDrawable_PTR p_backgroup)
{
	KDrawable_PTR p_drawable (new KColorDrawable(0x70000000));
	p_drawable->setRect(0,0,m_rect.width(),m_rect.height());
	m_lst_drawable.insert(m_lst_drawable.begin(),p_drawable);

	if (p_backgroup)
	{	
		KViewGroup_PTR p = KViewGroup_PTR(new KViewGroup());
		RERect r = p_backgroup->GetRect();
		p->Create(RERect::MakeXYWH(m_rect.left() + (m_rect.width() - r.width())/2,m_rect.top() + (m_rect.height() - r.height())/2 , r.width(), r.height()));
		p->addDrawable(p_backgroup);


		KTextDrawable_PTR text_drawable = KTextDrawable_PTR (new KTextDrawable(getStringManager()->GetStringByID(_T("warning")).c_str(), 0XFFFFFFFF, 14, REPaint::kLeft_Align) );
		text_drawable->setRect(20,0, r.width() - 40,40);
		p->addDrawable(text_drawable);

		m_text_drawable = KTextMultiLineDrawable_PTR (new KTextMultiLineDrawable( 0xff000000, 15, REPaint::kLeft_Align) );
		m_text_drawable->SetRect(RERect::MakeXYWH(0,0,r.width() - 10, r.height() - MSG_BUTTON_H - 10));
		m_text_drawable->SetText(m_str_text);
		m_text_drawable->setTextAlign(REPaint::kCenter_Align);
		m_text_drawable->SetViewChangeType(KVIEW_LRMethod_Parent_Left, KVIEW_LRMethod_Parent_Right, KVIEW_BTMethod_Parent_BTMiddle, KVIEW_BTMethod_Parent_BTMiddle);
		m_text_drawable->setAlignType(ALIGN_CENTER);

		m_txt_view = KTextView_PTR(new KTextView);
		m_txt_view->setTextDrawable(m_text_drawable);
		m_txt_view->Create(5,10,m_text_drawable->GetRect().width(),m_text_drawable->GetRect().height());
		p->AddView(m_txt_view);

		switch (m_type)
		{
		case KMB_OK:
			p->createImgView9PatchHelper(&m_btn_ok,
				_T("./resource/btn_ok_n.9.png")
				,_T("./resource/btn_ok_a.9.png"),
				_T("./resource/btn_ok_n.9.png"),
				r.left() + (r.width() - MSG_BUTTON_W)/2, r.bottom() - MSG_BUTTON_H - 12, MSG_BUTTON_W, MSG_BUTTON_H);
			m_btn_ok->SetText( getStringManager()->GetStringByID(_T("ok")) );
			m_btn_ok->SetFontSize(12);
			m_btn_ok->setStateChangeEnable(true);
			m_btn_ok->SetTextColor(MSG_FONT_CLR,MSG_FONT_CLR,MSG_FONT_CLR_A,MSG_FONT_CLR);
			m_btn_ok->setTextAlign(REPaint::kCenter_Align);
			m_btn_ok->m_clicked_signal.connect(this,&NMessageBoxView::onBtnOk);
			break;

		case KMB_QUESTION:
			p->createImgView9PatchHelper(&m_btn_yes, 
				_T("./resource/btn_ok_n.9.png")
				,_T("./resource/btn_ok_a.9.png"),
				_T("./resource/btn_ok_n.9.png"),
				r.left() + (r.width()/2 - MSG_BUTTON_W)/2, r.bottom() - MSG_BUTTON_H - 12, MSG_BUTTON_W, MSG_BUTTON_H);
			m_btn_yes->SetText( getStringManager()->GetStringByID(_T("yes")) );
			m_btn_yes->SetFontSize(12);
			m_btn_yes->setStateChangeEnable(true);
			m_btn_yes->setTextAlign(REPaint::kCenter_Align);
			m_btn_yes->SetTextColor(MSG_FONT_CLR,MSG_FONT_CLR,MSG_FONT_CLR_A,MSG_FONT_CLR);
			m_btn_yes->m_clicked_signal.connect(this,&NMessageBoxView::onBtnYes);

			p->createImgView9PatchHelper(&m_btn_no, 
				_T("./resource/btn_ok_n.9.png")
				,_T("./resource/btn_ok_a.9.png"),
				_T("./resource/btn_ok_n.9.png"),
				r.left() + (r.width()/2 - MSG_BUTTON_W)/2 + r.width()/2 , r.bottom() - MSG_BUTTON_H - 12, MSG_BUTTON_W, MSG_BUTTON_H);
			m_btn_no->SetText( getStringManager()->GetStringByID(_T("no")) );
			m_btn_no->SetFontSize(12);
			m_btn_no->setStateChangeEnable(true);
			m_btn_no->setTextAlign(REPaint::kCenter_Align);
			m_btn_no->m_clicked_signal.connect(this,&NMessageBoxView::onBtnNo);
			m_btn_no->SetTextColor(MSG_FONT_CLR,MSG_FONT_CLR,MSG_FONT_CLR_A,MSG_FONT_CLR);
			break;
		case KMB_SELECT:

			p->createImgView9PatchHelper(&m_btn_yes,
				_T("./resource/btn_ok_n.9.png")
				,_T("./resource/btn_ok_a.9.png"),
				_T("./resource/btn_ok_n.9.png"),
				r.left() + (r.width()/3 - MSG_BUTTON_W)/3, r.bottom() - MSG_BUTTON_H - 12, MSG_BUTTON_W, MSG_BUTTON_H);
			m_btn_yes->SetText( getStringManager()->GetStringByID(_T("ok")) );
			m_btn_yes->SetFontSize(12);
			m_btn_yes->setStateChangeEnable(true);
			m_btn_yes->setTextAlign(REPaint::kCenter_Align);
			m_btn_yes->SetTextColor(MSG_FONT_CLR,MSG_FONT_CLR,MSG_FONT_CLR_A,MSG_FONT_CLR);
			m_btn_yes->m_clicked_signal.connect(this,&NMessageBoxView::onBtnYes);


			p->createImgView9PatchHelper(&m_btn_no,
				_T("./resource/btn_ok_n.9.png")
				,_T("./resource/btn_ok_a.9.png"),
				_T("./resource/btn_ok_n.9.png"),
				r.left() + (r.width()/3 - MSG_BUTTON_W)/3 + r.width()/3 , r.bottom() - MSG_BUTTON_H - 12, MSG_BUTTON_W, MSG_BUTTON_H);
			m_btn_no->SetText( getStringManager()->GetStringByID(_T("no")) );
			m_btn_no->SetFontSize(12);
			m_btn_no->setStateChangeEnable(true);
			m_btn_no->setTextAlign(REPaint::kCenter_Align);
			m_btn_no->m_clicked_signal.connect(this,&NMessageBoxView::onBtnNo);
			m_btn_no->SetTextColor(MSG_FONT_CLR,MSG_FONT_CLR,MSG_FONT_CLR_A,MSG_FONT_CLR);

			p->createImgView9PatchHelper(&m_btn_no,
				_T("./resource/btn_ok_n.9.png")
				,_T("./resource/btn_ok_a.9.png"),
				_T("./resource/btn_ok_n.9.png"),
				r.left() + (r.width()/3 - MSG_BUTTON_W)/3 + r.width()/3*2 , r.bottom() - MSG_BUTTON_H - 12, MSG_BUTTON_W, MSG_BUTTON_H);
			m_btn_no->SetText( getStringManager()->GetStringByID(_T("cancel")) );
			m_btn_no->SetFontSize(12);
			m_btn_no->setStateChangeEnable(true);
			m_btn_no->setTextAlign(REPaint::kCenter_Align);
			m_btn_no->m_clicked_signal.connect(this,&NMessageBoxView::onBtnCancal);
			m_btn_no->SetTextColor(MSG_FONT_CLR,MSG_FONT_CLR,MSG_FONT_CLR_A,MSG_FONT_CLR);
			break;
		case KMB_WAIT:
			for (int i = 0; i < 4 ; i++)
			{
				KImgButtonView_PTR btn_n;
				p->createImgViewHelper(&btn_n, _T("./resource/loading_n.png"), _T("./resource/loading_n.png"), _T("./resource/loading_a.png"),
					r.left() + r.width()/4 + i * r.width()/6 ,r.bottom() - 40);
				m_vec_loading.push_back(btn_n);
			}

			m_timer = KTimer_PTR(new KTimer(200, -1));
			m_timer->m_timeout_signal.connect(this, &NMessageBoxView::timerProc);
			m_timer->setView(shared_from_this());
			m_timer->start();
			break;
		}
		AddView(p);
	}

}

void NMessageBoxView::onBtnOk(KView_PTR p)
{
	m_sign_msg_ok.emit();
	closeMsg();
}

void NMessageBoxView::onBtnYes(KView_PTR p)
{
	m_sign_msg_yes.emit();
	closeMsg();
}

void NMessageBoxView::onBtnNo(KView_PTR p)
{
	m_sign_msg_no.emit();
	closeMsg();
}

void NMessageBoxView::onBtnCancal(KView_PTR p)
{
	closeMsg();
}



KTextMultiLineDrawable_PTR NMessageBoxView::getTextDrawable()
{
	return m_text_drawable;
}

void NMessageBoxView::onDownDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	for (int i = 0; i < getViewCount() ; i++)
	{
		getViewByIndex(i)->DoMessage(pMsg);
	}
	pMsg->setIdle(KMSG_RETURN_DILE);
}

void NMessageBoxView::onMoveDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	for (int i = 0; i < getViewCount() ; i++)
	{
		getViewByIndex(i)->DoMessage(pMsg);
	}
	pMsg->setIdle(KMSG_RETURN_DILE);
}

void NMessageBoxView::onUpDirect( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	for (int i = 0; i < getViewCount() ; i++)
	{
		getViewByIndex(i)->DoMessage(pMsg);
	}
	pMsg->setIdle(KMSG_RETURN_DILE);
}

void NMessageBoxView::closeMsg()
{
	if (!m_p_parent.expired())
	{		
		if (m_timer)
		{
			m_timer->stop();
			m_timer.reset();
		}
		m_p_parent.lock()->AddViewToDel(shared_from_this());
	}
}

void NMessageBoxView::initOk()
{
	//TODO
}

void NMessageBoxView::initQuestion()
{
	//TODO
}

void NMessageBoxView::initWait()
{
	//TODO
}

void NMessageBoxView::setText( const kn_string& s )
{
	m_txt_view->SetText(s);
}

KTextMultiLineDrawable_PTR NMessageBoxView::getMultiDrawable()
{
	writeLock lock(m_draw_mult_mutex);
	return m_text_drawable;
}

NMessageModalView::NMessageModalView( const kn_string& s,KMB_TYPE t /*= KMB_OK */ ):NMessageBoxView(s,t)
{

}

void NMessageModalView::onBtnYes(KView_PTR p)
{
	endModal(KN_REUSLT_OK, TRUE);
}

void NMessageModalView::onBtnNo(KView_PTR p)
{
	endModal(KN_REUSLT_CANCLE, TRUE);
}

NMessageModalView::~NMessageModalView( void )
{

}

void NMessageModalView::onBtnCancal( KView_PTR p )
{
	//ȡ��
	endModal(2, TRUE);
}


void NMessageBoxView::timerProc(int pram)
{

	writeLock lock(m_draw_mult_mutex);
	if(m_i_index > 3 )
	{
		m_i_index = -1;
		for (int i = 0; i <  m_vec_loading.size(); ++i)
		{
			m_vec_loading[i]->setState(KTextView::BS_NORMAL);
		}
	}
	else
	{
		m_vec_loading[m_i_index]->setState(KTextView::BS_FOCUS);
	}
	m_i_index++;

}
