//  File:        KStaticTextView.h
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
#ifndef _K_STATIC_TEXT_VIEW_H_
#define _K_STATIC_TEXT_VIEW_H_

#include <boost/shared_ptr.hpp>
#include "KTextView.h"
class KStaticTextView : public KStaticView
{
public:
	KStaticTextView(void);
	virtual ~KStaticTextView(void);
public:
	SkTypeface* GetFont();
	void SetFont(SkTypeface* pTypeFace);
};

typedef boost::shared_ptr<KStaticTextView> KStaticTextView_PTR;
#endif // _K_STATIC_TEXT_VIEW_H_
