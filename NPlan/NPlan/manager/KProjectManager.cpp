//  File:        KProjectManager.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/10/30
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "KProjectManager.h"
#include "NewRenderingEngine.h"
#include "KConfigManager.h"

///////////////////////////////////////

KProjectManager* g_proj_m = NULL;
KProjectManager* getProjectManager()
{
	if (g_proj_m == NULL)
	{
		g_proj_m = new KProjectManager();
		g_proj_m->init();
	}
	return g_proj_m;
}

void ReleaseProjectManager()
{
	SAFE_DELETE(g_proj_m);
}

KProjectManager::KProjectManager()
{
}


KProjectManager::~KProjectManager()
{
	ProjectDataVec::iterator it;
	for ( it = m_lst_proj.begin(); it != m_lst_proj.end(); it++ )
	{
		 delete *it;
	}

	m_lst_proj.clear();
}

CNProjectData* KProjectManager::GetProjectByIndex(int index)
{
	if (index < m_lst_proj.size())
	{
		return m_lst_proj[index];
	}
	return NULL;
}

int KProjectManager::getSize()
{
	return m_lst_proj.size();
}

CNProjectResourcePool* KProjectManager::getGlobalResource()
{
	return &m_global_resource;
}

void KProjectManager::init()
{
	kn_string str = GetCurrentPath();
	str += _T("config/global.xml");

	LOGI ("KProjectManager::init 111");
	m_global_resource.loadXMLFile( str);
	
   kn_string dir_name = getProjectPath();
   kn_string szFind = getProjectPath() + _T("*.nprj");  
 
	LOGI ("KProjectManager::init 222");
   vector<kn_string> vStrFileNameArray;
   GetFilesArrayOfDirectory(dir_name.c_str(), _T("*.nprj"), vStrFileNameArray);
	LOGI ("KProjectManager::init 333");
   for(size_t i = 0; i < vStrFileNameArray.size(); i++)
   {
	   CNProjectData* p = new CNProjectData();
	   kn_string str = dir_name + vStrFileNameArray[i] ;

	   if(p->loadPrjFile(str) == NO_ERROR )
	   {
		   m_lst_proj.push_back(p);
	   }
	   else
	   {
		   delete p;
	   }
   }

   openHistoryPrj();
	LOGI ("KProjectManager::init 444");
//#ifdef WIN32
//  
//    WIN32_FIND_DATA FindFileData;  
//  
//  
//    HANDLE hFind=::FindFirstFile(szFind.c_str(),&FindFileData);  
//    if(INVALID_HANDLE_VALUE == hFind)    return;  
//    while(TRUE)  
//    {  
//        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
//        {  //不遍历子目录
//
//            //if(FindFileData.cFileName[0]!='.')  
//            //{  
//            //    strcpy(szFile,lpPath);  
//            //    strcat(szFile,"//");  
//            //    strcat(szFile,FindFileData.cFileName);  
//            //    find(szFile);  
//            //}  
//        }  
//        else  
//        {        
//               //cout<<FindFileData.cFileName<<endl;  
//			CNProjectData* p = new CNProjectData();
//			kn_string str = dir_name + FindFileData.cFileName ;
//			p->loadPrjFile(str);
//			m_lst_proj.push_back(p);
//        }  
//        if(!FindNextFile(hFind,&FindFileData))  
//            break;  
//    }  
//    FindClose(hFind);   
////	openHistoryPrj();
//
//
//#endif
}
void KProjectManager::openHistoryPrj()
{
	vector<kn_string> openHistory;
	getConfigManager()->GetVectorConfig(_T("historys"),openHistory);
	if (!openHistory.empty())
	{
		vector<kn_string> ::iterator iter = openHistory.begin();
		for (;iter != openHistory.end(); iter++)//判断历史是否已存在，存在退出保存。
		{
			CNProjectData* p = new CNProjectData();
			if(p->loadPrjFile(*iter) == NO_ERROR )
			{
				m_lst_proj.push_back(p);
			}
			else
			{
				delete p;
			}
		}

	}
}
CNProjectData* KProjectManager::newProject()
{
	CNProjectData* p = new CNProjectData();
	m_lst_proj.push_back(p);
	return p;
}

bool KProjectManager::removeProject(CNProjectData* p)
{
	if (p==NULL)
	{
		return false;
	}

	for (ProjectDataVec::iterator iter = m_lst_proj.begin();iter!=m_lst_proj.end();iter++)
	{
		if ((*iter)->getName()==p->getName())
		{
			m_lst_proj.erase(iter);
			return true;
		}
	}

	return false;
}

CNProjectData* KProjectManager::openProject(kn_string  filename,OUT int& result)
{
	CNProjectData* p = new CNProjectData();
	result = p->loadPrjFile(filename );
	if (result == NO_ERROR)//打开成功才添加
	{
		m_lst_proj.push_back(p);
	}
	return p;
}

kn_string KProjectManager::getProjectPath()
{
	kn_string strPath = GetCurrentPath();
	strPath += _T("project/");

	return strPath;

}
