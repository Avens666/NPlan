//  File:        NprojectResourcePool.cpp
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
#include "NProjectResourcePool.h"
#include <algorithm>
#include <functional>

CNProjectResourcePool::CNProjectResourcePool( void )
{
}

CNProjectResourcePool::~CNProjectResourcePool( void )
{
}

//通过资源组ID获取相关联的名称
kn_string CNProjectResourcePool::getResourceGroupName( int groupId )
{
    kn_string strGroupName;

    vector <CNProjectResGroup>::iterator finder;
    for ( finder = m_resourcegroups.begin(); finder != m_resourcegroups.end(); ++finder )
    {
        if ( finder->getId() == groupId )
        {
            strGroupName = finder->getName();
        }
    }

    return strGroupName;
}

//通过资源ID获取相关联的名称
kn_string CNProjectResourcePool::getResourceName( int resId )
{
    kn_string resource_name;

	CNProjectResource* p = getResourceByID(resId);
	if (p)
	{
		resource_name = p->getName();
	}

    return resource_name;
}

CNProjectResource* CNProjectResourcePool::getResourceByID( int resId )
{
	if (resId > 0)
	{
		CNProjectResource dummy;
		dummy.setId(resId);

		std::vector<CNProjectResource>::iterator finder;
		finder = lower_bound(m_resources.begin(), m_resources.end(), dummy) ;
		if ( (finder != m_resources.end()) && ( !(dummy < *finder ) ) )
		{
			return  &(*finder);
		}
	}
	return NULL;
}

res_type CNProjectResourcePool::getResourceType( int resId )
{
	res_type type = res_man;

	CNProjectResource* p = getResourceByID(resId);
	if (p)
	{
		type = p->getType();
	}

	return type;
}


//增加一个资源组(返回新增资源组的ID)
int CNProjectResourcePool::addResGroup( kn_char* name, int groupId /* = 0*/ )
{
    if( 0 == groupId )
    {
        groupId = getUnusedResourceGroupID();
    }

    CNProjectResGroup resgroup( name, groupId );
    return addResGroup(resgroup);
}

//增加一个资源组
int CNProjectResourcePool::addResGroup( CNProjectResGroup & resGroup )
{
    if ( 0 == resGroup.getId())
    {
        resGroup.setId( getUnusedResourceGroupID());
    }

    m_resourcegroups.push_back( resGroup );
    sort( m_resourcegroups.begin(), m_resourcegroups.end() );
    return resGroup.getId();
};

//增加一个资源项
int CNProjectResourcePool::addResource( CNProjectResource & res )
{
    if ( 0 == res.getId())
    {
        res.setId( getUnusedResourceID());
    }

    m_resources.push_back( res );

    sort( m_resources.begin(), m_resources.end() );
    return res.getId();
};

//增加一个资源项,
CNProjectResource & CNProjectResourcePool::addResource( kn_string& name, int type, int id )
{
    CNProjectResource res ( name.c_str(), id, type );

    if ( 0 == id )
    {
        res.setId( getUnusedResourceID());
    }

    m_resources.push_back( res );

    sort( m_resources.begin(), m_resources.end() );

    vector <CNProjectResource>::iterator it;
    for ( it = m_resources.begin(); it != m_resources.end(); ++it )
    {
        if ( it->getId() == res.getId())
        {
            break;
        }
    }

   return *it;
}
//通过ID删除一个资源项
bool CNProjectResourcePool::delResourceByID(int id)
{
	vector <CNProjectResource>::iterator it;
	for ( it = m_resources.begin(); it != m_resources.end(); ++it )
	{
		if ( it->getId() == id)
		{
			m_resources.erase(it);
			return true;
		}
	}
	return false;
}
//通过id查找资源是否存在（id唯一）
bool CNProjectResourcePool::checkResourceByID(int id)
{
	vector <CNProjectResource>::iterator it;
	for ( it = m_resources.begin(); it != m_resources.end(); ++it )
	{
		if ( it->getId() == id)
		{
			return true;
		}
	}
	return false;
}
//修改一个资源项
bool CNProjectResourcePool::modifyResource(kn_string& name,int type, int id, int newId)
{
	vector <CNProjectResource>::iterator it;
	for ( it = m_resources.begin(); it != m_resources.end(); ++it )
	{
		if ( it->getId() == id)
		{
			it->setName(name.c_str());
			it->setId(newId);
			it->setType(type);
			return true;
		}
	}
	return false;
}
//通过资源组名称获取相资源组ID
int CNProjectResourcePool::findResourceGroupID( const kn_char* name )
{
    int index( 0 );

    vector <CNProjectResGroup>::iterator it = m_resourcegroups.begin();
    for(; it != m_resourcegroups.end(); ++it )
    {
        if ( 0 == _tcscmp( name, (*it).getName().c_str()))
        {
            index = std::distance( m_resourcegroups.begin(), it )+1;
            break;
        }
    }

    return index;
}

//通过资源名称获取相资源ID
int CNProjectResourcePool::findResourceID( const kn_char* name )
{
    //资源列表为空，直接返回-1
    if ( m_resources.size() == 0 )
    {
        return -1;
    }
    int index = -1;
    if( name != NULL )
    {
        kn_string tpname = name;


    vector <CNProjectResource>::iterator it;
    for ( it = m_resources.begin(); it != m_resources.end(); ++it )
    {
        if ((*it).getName() == tpname )
        {
            index = (*it).getId();
            break;
        }
    }
    }
    return index;
}

//设置资源所属组别
void CNProjectResourcePool::setResourceGroupIDs( CNProjectResource & res, const kn_char* lpGroups )
{
    vector <kn_string> vectGroups = splitString2Vect( lpGroups );
    size_t stCount = vectGroups.size();
    for ( size_t i = 0; i < stCount; i++ )
    {
        kn_string &strGroupName = vectGroups[i];
        int index = findResourceGroupID( strGroupName.c_str());
        if ( 0 == index )
        {
            index = addResGroup((kn_char*)strGroupName.c_str(), 0 );
        }

        res.setGroup( index );
    }
}

//获取一个资源组有哪些资源
std::vector <int> CNProjectResourcePool::getGroupHaveResource( int _groupid )
{
    vector <int> resultv;
    vector <CNProjectResource>::iterator iter;
    for ( iter = m_resources.begin(); iter != m_resources.end(); ++iter )
    {
        vector <int> gp = iter->getGroups();
        vector <int>::iterator itgp = find( gp.begin( ), gp.end( ), _groupid ); //查找
        if ( itgp == gp.end( ))  //没找到
        {
        }
        else //找到了，从resultv中寻找，
        {
            vector <int>::iterator itv = find( resultv.begin( ), resultv.end( ), iter->getId()); //查找
            if ( itv == resultv.end())
            {
                resultv.push_back( iter->getId());
            }
        }
    }

    return resultv;
}

std::vector <CNProjectResGroup> & CNProjectResourcePool::getResourceGroups()
{
    return m_resourcegroups;
}

std::vector <int> CNProjectResourcePool::getResourceBelongGroups( int resId )
{
    vector <int> result;
    vector <CNProjectResource>::iterator it;
    for ( it = m_resources.begin(); it != m_resources.end(); ++it )
    {
        if ( it->getId() == resId )
        {
            return it->getGroups();
        }
    }

    return result;
}

void CNProjectResourcePool::setResourceGroups( std::vector <CNProjectResGroup> & resourcegroups )
{
    m_resourcegroups.clear();
    std::copy( resourcegroups.begin(), resourcegroups.end(), std::back_inserter( m_resourcegroups ));
}

std::vector <CNProjectResource> & CNProjectResourcePool::getResources()
{
    return m_resources;
}

void CNProjectResourcePool::setResources( std::vector <CNProjectResource > & resources )
{
    m_resources.clear();
    std::copy( resources.begin(), resources.end(), std::back_inserter( m_resources ));
}

int CNProjectResourcePool::getUnusedResourceID()
{
    vector <CNProjectResource>::iterator iter;
    vector <int> vecids;
    for ( iter = m_resources.begin(); iter != m_resources.end(); ++iter )
    {
        vecids.push_back( iter->getId());
    }

    if ( vecids.empty())
    {
        return 1;
    }
    else
    {
        for ( int i = 1; i <= vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))  //没找到
            {
                return i;
            }
        }

        return (vecids.size()+1);
    }
}

int CNProjectResourcePool::getUnusedResourceGroupID()
{
    vector <CNProjectResGroup>::iterator iter;
    vector <int> vecids;
    for ( iter = m_resourcegroups.begin(); iter != m_resourcegroups.end(); ++iter )
    {
        vecids.push_back( iter->getId());
    }

    if ( vecids.empty())
    {
        return 1;
    }
    else
    {
        for ( int i = 1; i <= vecids.size(); i++ )
        {
            vector <int>::iterator result = find( vecids.begin( ), vecids.end( ), i );
            if ( result == vecids.end( ))  //没找到
            {
                return i;
            }
        }

        return (vecids.size()+1);
    }
}

kn_string CNProjectResourcePool::getResMembersNameStr( int leader_id )
{
    kn_string member_Res_namestr;

    vector <CNProjectResource>::iterator iter;
    for( iter = getResources().begin(); iter != getResources().end(); iter++ )
    {
        if ( leader_id == iter->getId()) //过滤leader
        {
            continue;
        }

        member_Res_namestr += (iter->getName()+_T( " " ));
    }

    return member_Res_namestr;
}

kn_string CNProjectResourcePool::typeInt2Str( int type )
{
    kn_string type_str;
    switch( type )
    {
    case res_woman:
        type_str = getStringManager()->GetStringByID(_T("woman"));
        break;
    case res_man:
        type_str = getStringManager()->GetStringByID(_T("man"));
        break;
    case res_device:
        type_str = getStringManager()->GetStringByID(_T("device"));
        break;
    default:
        type_str = RES_NAME_UNKNOWN;
        break;
    }

    boost::algorithm::trim( type_str );
    return type_str;
}
