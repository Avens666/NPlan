//  File:        NResourceGroupView.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/8/26
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "KViewGroup.h"
#include "NGridView.h"
#include "KSlideView.h"
#include "NDragStaffView.h"
class KResourceGroupView :
	public KViewGroup
{
public:
	KResourceGroupView();
	virtual ~KResourceGroupView(void);
	void Init(kn_int iRow, kn_int iCol, vector<NMemberMsg>& vec_mem);

	void onDownMem(kn_int x, kn_int y,KMessageMouse* msg);

	void Release();
private:
	NGridView_PTR	m_view_grid;
	NDragStaffView_PTR m_p_drag_view;
	KSlideView_PTR m_p_slide;
	vector<NMemberMsg> m_vec_mem;

};

typedef boost::shared_ptr<KResourceGroupView> KResourceGroupView_PTR;
