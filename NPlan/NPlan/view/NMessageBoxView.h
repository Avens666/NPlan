//  File:        NMessageBoxView.h
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
#ifndef _NMessageBoxView_H_
#define _NMessageBoxView_H_
#include "KViewGroup.h"
#include "KButtonView.h"
#include "KTextMultiLineDrawable.h"

enum KMB_TYPE {KMB_OK,KMB_QUESTION,KMB_WAIT,KMB_SELECT};
class NMessageBoxView : public KViewGroup
{
public:
	NMessageBoxView(const kn_string& s,KMB_TYPE t = KMB_OK );
	virtual ~NMessageBoxView(void);
	void Init(KDrawable_PTR p_backgroup);

	//�������ʱ��������Ϣ
	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 

	void setBKColor(REColor clr);
	KTextMultiLineDrawable_PTR getTextDrawable();
	virtual void onBtnOk(KView_PTR p);
	void closeMsg();
	virtual void onBtnYes(KView_PTR p);
	virtual void onBtnNo(KView_PTR p);
	void initOk();
	void initQuestion();
	void initWait();
	void setText(const kn_string& s);
	KTextMultiLineDrawable_PTR getMultiDrawable();
	virtual void onBtnCancal(KView_PTR p);
	void timerProc(int pram);
	//�ص��ź�
	sigslot::signal0<> m_sign_msg_yes;
	sigslot::signal0<> m_sign_msg_no;
	sigslot::signal0<> m_sign_msg_ok;
protected:
	KTextView_PTR			m_p_backgroup;
	REColor				m_clr_bk;
	kn_string			m_str_text;
	KTextMultiLineDrawable_PTR	m_text_drawable;
	KTextView_PTR			m_txt_view;
	KImgButtonView_PTR		m_btn_ok;
	KImgButtonView_PTR		m_btn_yes;
	KImgButtonView_PTR		m_btn_no;
	vector<KImgButtonView_PTR>	m_vec_loading;
	KMB_TYPE			m_type;
	KTimer_PTR m_timer;
	kn_int m_i_index;

	boost::shared_mutex m_draw_mult_mutex;

};

class NMessageModalView : public NMessageBoxView
{

public:
	NMessageModalView(const kn_string& s,KMB_TYPE t = KMB_OK );
	virtual ~NMessageModalView(void);
	void onBtnYes(KView_PTR p);
	void onBtnNo(KView_PTR p);
	void onBtnCancal(KView_PTR p);
};

typedef boost::shared_ptr<NMessageBoxView> NMessageBoxView_PTR;
typedef boost::shared_ptr<NMessageModalView> NMessageModalView_PTR;
#endif _NMAINVIEW_H_

