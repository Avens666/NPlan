//  File:        NDragStaffView.h
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
#ifndef _NDragStaffView_H_
#define _NDragStaffView_H_


#include "NStaffView.h"

class NDragStaffView : public NStaffView
{
public:

	NDragStaffView( const kn_string& strName,res_type type ,kn_int id ,kn_bool b = true);
	void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	void OnDown(kn_int x, kn_int y, KMessageMouse* pMsg);
	void OnMove(kn_int x, kn_int y, KMessageMouse* pMsg);
	void setDragMsg(kn_int drag_msg, kn_int drag_up_msg);
protected:
	kn_int m_i_drag_msg;
	kn_int m_i_drag_up_msg;
};
typedef boost::shared_ptr<NDragStaffView> NDragStaffView_PTR;
#endif

