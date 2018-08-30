//  File:        PathEffect.h
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
#ifndef PathEffect_DEFINED
#define PathEffect_DEFINED

#include "SkPathEffect.h"
#include "effects/Sk1DPathEffect.h"
#include "effects/Sk2DPathEffect.h"
#include "effects/SkCornerPathEffect.h"
#include "effects/SkDashPathEffect.h"
#include "effects/SkDiscretePathEffect.h"

typedef SkPathEffect REPathEffect;
typedef Sk1DPathEffect RE1DPathEffect;
typedef SkPath1DPathEffect REPath1DPathEffect;
typedef Sk2DPathEffect RE2DPathEffect;
typedef SkPath2DPathEffect REPath2DPathEffect;
typedef SkCornerPathEffect RECornerPathEffect;
typedef SkDashPathEffect REDashPathEffect;
typedef SkDiscretePathEffect REDiscretePathEffect;
typedef SkSumPathEffect RESumPathEffect;
typedef SkComposePathEffect REComposePathEffect;

#endif
