//  File:        KAreaBlurDrawable.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/12/25
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_AREABLUR_DRAWABLE_H
#define K_AREABLUR_DRAWABLE_H


#include "KDrawable.h"


class KAreaBlurDrawable : public KDrawable
{
public :
	KAreaBlurDrawable();
	virtual ~KAreaBlurDrawable();
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);
	virtual void SetRect(const RERect& rect);
protected:
	IRESurface* m_blur_surface;
};
typedef boost::shared_ptr<KAreaBlurDrawable> KAreaBlurDrawable_PTR;

#endif
