//  File:        NStaffView.cpp
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
#include "NStaffView.h"
#define MEN_HEAD_PATH		_T("./resource/img_boy.png")			// 默认男生头像
#define WOMEN_HEAD_PATH		_T("./resource/img_girl.png")		// 默认女生头像
#define DEVICE_HEAD_PATH		_T("./resource/img_device.png")		// 默认女生头像
#define SELECTED_ICON_PATH  _T("./resource/check_ico.png")

//构造函数,传名字和性别创建VIEW


NStaffView::NStaffView(const NMemberMsg& msg)
{
	OnInit(msg.str_name,msg.tp,msg.id,msg.b_update);
}
NStaffView::NStaffView( const kn_string& strName,res_type type ,kn_int id ,kn_bool b )
{
	OnInit(strName,type,id,b);
}

NStaffView::NStaffView()
{
	OnInit(_T(""),res_man,-1,true);
}
void NStaffView::OnInit(const kn_string& strName,res_type type ,kn_int id,kn_bool b)
{
	
	SetResourceType(type);
	m_mem_msg.id		=	id;
	m_mem_msg.b_update  =	b;
	SetName(strName);
	SetFontSize(14);
	KImageDrawable_PTR drawable = KImageDrawable_PTR(new KImageDrawable(getSurfaceManager()->GetSurface(SELECTED_ICON_PATH),true));
	setIconDrawable(drawable);
	showIcon(false);
	/**
	* 变量初始化
	*/
	m_b_is_leader	 = false;
	m_b_can_dbclick	 = true;
	m_b_set_leader	 = false;
	m_b_drag_move	 = false;
}

NStaffView::~NStaffView(void)
{

}
//设置头像图片
void NStaffView::SetHeadImg( KDrawable_PTR p ,bool b)
{
	if (p)
	{
		if(b)
		{
			SetRect(p->GetRect());
		}
		setBKDrawable(p);
	}
}
//设置头像图片
void NStaffView::SetHeadImg( const kn_string& strPath,bool b )
{
	IRESurface* img = getSurfaceManager()->GetSurface(strPath.c_str());
	KImageDrawable_PTR drawable = KImageDrawable_PTR(new KImageDrawable(img,true));
	SetHeadImg(drawable,b);
}
//设置名称

void NStaffView::SetName( const kn_string& strName ,kn_int i_font_w /*= 80*/,STAFF_NAME_CLIP type /*= NAME_OMIT*/ )
{
	m_mem_msg.str_name	=	strName;
	kn_string str = strName;
	switch (type)
	{
	case NAME_CLIP:
		m_text_drawable->SetClip(m_text_drawable->GetRect());
		break;
	case NAME_OMIT:
		setStringOmit(str,*(m_text_drawable->GetPaint()),i_font_w);
		break;
	}
	SetText(str);
	m_text_drawable->setRect((m_rect.width() - i_font_w) / 2,
		m_bk_drawable->GetRect().bottom(),i_font_w,m_text_drawable->GetFontSize() + 6);	//直接用fontsize会出现第一行像素显示不出来的情况
	//所以默认上下都加3像素
    m_text_drawable->setTextAlign(REPaint::kCenter_Align);
}	

void NStaffView::SetResourceType(res_type type, kn_bool b)
{
	m_mem_msg.tp = type;
	if (type == res_man)
	{
		SetHeadImg(MEN_HEAD_PATH,b);
	}
	else if (type == res_woman)
	{
		SetHeadImg(WOMEN_HEAD_PATH,b);
	}
	else if (type = res_device)
	{
		SetHeadImg(DEVICE_HEAD_PATH,b);
	}
}
void NStaffView::SetID(int id)
{
	m_mem_msg.id = id;
}
void NStaffView::GetName( kn_string& str )
{
	str = m_mem_msg.str_name;
}

kn_string NStaffView::GetName()
{
	return m_mem_msg.str_name;
}

res_type NStaffView::GetResourceType()
{
	return m_mem_msg.tp;
}
void NStaffView::setLeader(kn_bool b)
{
	showIcon(b);
	
	if (m_b_set_leader)
	{
		m_b_is_leader = b;
	}

}

void NStaffView::OnDClick( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{
	if (!m_p_parent.expired() && m_b_can_dbclick)
	{
		KViewGroup_PTR parent = m_p_parent.lock();
		for (int i = 0; i < parent->getViewCount() ; i++)
		{
			NStaffView_PTR  pview = VIEW_PTR(NStaffView)(parent->getViewByIndex(i) );
			pview->setLeader(pview == shared_from_this() );
		}
		parent->InvalidateView();
	}
}
void NStaffView::OnSelected()
{
	if (!m_p_parent.expired() && m_b_can_dbclick)
	{
		KViewGroup_PTR parent = m_p_parent.lock();
		for (int i = 0; i < parent->getViewCount() ; i++)
		{
			NStaffView_PTR  pview = VIEW_PTR(NStaffView)(parent->getViewByIndex(i));
			pview->setSelectedStatus(pview == shared_from_this());
		}
		parent->InvalidateView();
	}
}
void NStaffView::setSelectedStatus(kn_bool b)
{
		showIcon(b);
}
void NStaffView::UnSelected()
{
	showIcon(false);
}
kn_bool NStaffView::isLeader()
{
	return m_b_is_leader;
}

kn_int NStaffView::GetId()
{
	return m_mem_msg.id;
}

NMemberMsg& NStaffView::getMsg()
{
	return m_mem_msg;
}


void NStaffView::setCanDBclick( kn_bool b )
{
	m_b_can_dbclick = b;

}

void NStaffView::setCanLeader( kn_bool b )
{
	m_b_set_leader = b;
}

bool NStaffView::getCanUpdate()
{
	return m_mem_msg.b_update;
}

void NStaffView::setCanUpdate( kn_bool b )
{
	m_mem_msg.b_update = b;
}
void NStaffView::setIcon(KImageDrawable_PTR drawable,kn_int x, int y,kn_bool b_show)
{
	setIconDrawable(drawable);
	setIconPos(x,y);
	showIcon(b_show);
}

// 文字超长截断
void NStaffView::setStringOmit(kn_string& strText, REPaint& paint, int iBoundWidth)
{
	REScalar len = paint.measureText(strText.c_str(), strText.length() * sizeof(kn_char));
	if(len > iBoundWidth)
	{
		// 需要截断
		kn_char szDot[] = _T("...");
		kn_int LEN_MAX = strText.length() + 4;
		kn_char* pszTemp = new kn_char [LEN_MAX];
		memset(pszTemp, 0, LEN_MAX * sizeof(kn_char));

		kn_int i = 0;
		for(i = 0; i < strText.length(); i++)
		{
			pszTemp[i] = strText[i];
			pszTemp[i + 1] = szDot[0];
			pszTemp[i + 2] = szDot[1];
			pszTemp[i + 3] = szDot[2];

			REScalar lentemp = paint.measureText(pszTemp, (i+4) * sizeof(kn_char));

			if(lentemp > iBoundWidth)
			{

				break;
			}
		}

		SAFE_DELETE_GROUP(pszTemp);

		strText = strText.substr(0, i);
		strText += szDot;

	}
}
