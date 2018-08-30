//  File:        NStaffView.h
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
#ifndef _NStaffView_H_
#define _NStaffView_H_


#include "KViewGroup.h"
#include "KButtonView.h"
#include "KTextView.h"
#include "KSurfaceManager.h"
#include "../file/NProjectDataStruct.h"
#include "../file/NProjectResource.h"
#include "../manager/KStringManager.h"
#include "KScreen.h"
#include "../NPlan.h"

typedef struct NMemberMsg
{
	kn_string	str_name;	//名字

	kn_int		id;		//ID

	res_type	tp;		//类型

	kn_bool		b_update;	//是否可以修改.
	/*有属性以后再加*/

	NMemberMsg(CNProjectResource& p)
	{
		str_name	=  p.getName();
		id			=  p.getId();
		tp			=  p.getType();
		b_update	=  true;
	}
	NMemberMsg()
	{
		str_name	=  _T("");
		id			=  -1;
		tp			=  res_unknown;
		b_update	=  true;
	}
	NMemberMsg(kn_string s,kn_int i,res_type t,kn_bool b = true)
	{
		str_name	= s;
		id			= i;
		tp			= t;
		b_update	= b;
	}
	bool operator == (NMemberMsg msg) const
	{
		return (str_name == msg.str_name/* && id == msg.id && tp == msg.tp*/);
	}

	bool operator != (NMemberMsg msg) const
	{
		return (str_name != msg.str_name || id != msg.id || tp != msg.tp);
	}

}NMemberMsg;

enum STAFF_NAME_CLIP	//名字超出长度的处理方式
{
	NAME_NOMAL,	//不做处理
	NAME_CLIP,	//截取
	NAME_OMIT,	//用'...'省略
};


class NStaffView : public KTextView
{
public:
	//构造函数,传名字和性别创建
	NStaffView();
	NStaffView(const NMemberMsg& msg);
	NStaffView( const kn_string& strName,res_type type ,kn_int id ,kn_bool b = true);
	~NStaffView(void);

	//设置头像图片
	void SetHeadImg(const kn_string& strPath,bool b = true);
	void SetHeadImg( KDrawable_PTR p ,bool b = true);
	//设置名称
	void SetName( const kn_string& strName ,kn_int i_font_w = 80,STAFF_NAME_CLIP  type = NAME_OMIT);
	//设置性别
	void SetResourceType(res_type type,bool b = true);

	//设置新ID
	void SetID(int id);

	kn_int GetId();

	void GetName(kn_string& str);
	kn_string GetName();
	res_type GetResourceType();

	void setLeader(kn_bool b);

	kn_bool isLeader();

	virtual void OnDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void OnSelected();
	void UnSelected();
	void setSelectedStatus(kn_bool b);
	NMemberMsg&	getMsg();
	void OnInit(const kn_string& strName,res_type type ,kn_int id,kn_bool b);
	void setCanDBclick(kn_bool b);
	void setCanLeader(kn_bool b);
	void setCanUpdate(kn_bool b);
	bool getCanUpdate();
	void setIcon(KImageDrawable_PTR drawable,kn_int x = 0, int y = 0,kn_bool b_show = false);
	void setStringOmit(kn_string& strText, REPaint& paint, int iBoundWidth);
protected:
	NMemberMsg	m_mem_msg;

	kn_bool		m_b_is_leader;	//是不是领导


	kn_bool		m_b_can_dbclick;//是否可以双击变亮

	kn_bool		m_b_set_leader;	//双击是否设成领袖

	kn_bool		m_b_drag_move;	//拖拽时,控件是否移动

	kn_int		m_i_down_x,m_i_down_y;

};

typedef boost::shared_ptr<NStaffView> NStaffView_PTR;
typedef vector<NStaffView_PTR>  VECSTAFF;
typedef vector<NStaffView_PTR>::iterator  VECSTAFFITR;



#endif

