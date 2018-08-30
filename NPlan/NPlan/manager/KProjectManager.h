//  File:        KProjectManager.h
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
#ifndef K_PROJECT_MANAGER_H
#define K_PROJECT_MANAGER_H

#include <vector>
#include "../file/NProjectData.h"
#define  ProjectDataVec vector<CNProjectData*>

class KProjectManager
{
private:

	ProjectDataVec m_lst_proj;
	CNProjectResourcePool m_global_resource;
public:
	KProjectManager();
	~KProjectManager();
	int getSize();
	CNProjectData* GetProjectById(int id);
	CNProjectData* GetProjectByIndex(int index);
	void init();
	CNProjectData* newProject();
	CNProjectData* openProject(kn_string,OUT int& );
	CNProjectResourcePool* getGlobalResource();
	kn_string getProjectPath();
	bool removeProject(CNProjectData* p);
	void openHistoryPrj();
};

KProjectManager* getProjectManager();
void ReleaseProjectManager();
#endif
