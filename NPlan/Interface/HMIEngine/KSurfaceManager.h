//  File:        KSurfaceManager.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/11/30
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#ifndef K_SURFACE_MANAGER_H
#define K_SURFACE_MANAGER_H

#include "boost/unordered_map.hpp"
#include "NewRenderingEngine.h"
#define  mapSurfaceMap boost::unordered_map<kn_string, IRESurface*>
using namespace skia_surface;
class KSurfaceManager
{
private:

	mapSurfaceMap m_SurfaceManagerMap;
public:
	KSurfaceManager();
	~KSurfaceManager();
	IRESurface* GetSurface(kn_string surfacepath);
};
typedef mapSurfaceMap::iterator MAPSurfaceIter;

KSurfaceManager* getSurfaceManager();
void ReleaseSurfaceManager();
#endif
