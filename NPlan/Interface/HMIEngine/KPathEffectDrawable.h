//  File:        KPathEffectDrawable.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/26
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_PATHEFFECT_DRAWABLE_H
#define K_PATHEFFECT_DRAWABLE_H


#include "KShapeDrawable.h"


class KPathEffectDrawable : public KShapeDrawable
{
public :
	KPathEffectDrawable();
	virtual ~KPathEffectDrawable();
	
};
typedef boost::shared_ptr<KPathEffectDrawable> KPathEffectDrawable_PTR;


#endif
