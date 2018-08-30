//  File:        KLayer.h
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
#ifndef K_LAYER_H
#define K_LAYER_H


#include "KViewGroup.h"

class KLayer : public KObject
{
private:
	RERect m_rect;

public:
	KLayer();
	~KLayer();
	void Release();
	kn_bool Create(int iWidth, int iHeight, REBitmap::Config cf);
	void Draw(IRESurface*  pDstSurface);
	void Draw(IRESurface* pDstSurface, RERect& rect);
	kn_bool DoMessage(KMessage* pMsg);
	
};
#endif
