//  File:        NGlobleResourcePool.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2015/10/29
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
#include "NGlobleResourcePool.h"
#include <sstream>

CNGlobleResourcePool::CNGlobleResourcePool( void )
{
}

CNGlobleResourcePool::~CNGlobleResourcePool( void )
{
}

int CNGlobleResourcePool::loadXMLFile( kn_string &filename )
{
    string file = ns2s( filename );
    TiXmlDocument *readdoc = new TiXmlDocument( file.c_str());
    if ( !readdoc->LoadFile())
    {
        delete readdoc;
        return ERROR_FILE_INVALID;
    }

    TiXmlElement *pRootElement = readdoc->RootElement();
    TiXmlElement* pResPooElement = pRootElement->FirstChildElement( "ResourcePool" );
    if ( pResPooElement )
    {
        //解析ResourceGroup标签
        TiXmlElement* pResourceGroupElement = pResPooElement->FirstChildElement( "ResourceGroup" );
        if ( pResourceGroupElement )
        {
            TiXmlElement* pResGroupElement = pResourceGroupElement->FirstChildElement();  //resgroup标签
            for ( pResGroupElement; pResGroupElement != NULL; pResGroupElement = pResGroupElement->NextSiblingElement())
            {
                CNProjectResGroup resgrp;
                resgrp.setName( GetAttributeString( pResGroupElement, "name" ).c_str());
                resgrp.setId( GetAttributeInt( pResGroupElement, "id" ));
                addResGroup( resgrp );
            }
        }

        //解析Resources标签
        TiXmlElement* pRessElement = pResPooElement->FirstChildElement( "Resources" );
        if ( pRessElement )
        {
            TiXmlElement* pResElement = pRessElement->FirstChildElement();  //resgroup标签
            for ( pResElement; pResElement != NULL; pResElement = pResElement->NextSiblingElement())
            {
                CNProjectResource res;
                res.setName( GetAttributeString( pResElement, "name" ).c_str()); //name
                res.setId( GetAttributeInt( pResElement, "id" ));                //id
                res.setType( GetAttributeInt( pResElement, "type" ));            //type
                vector<int> vArrt = GetAttribute2IntVector( pResElement, "group" );
                res.setGroups(vArrt);
                addResource( res );
            }
        }
    }
    else
    {
        delete readdoc;
        return ERROR_FILE_INVALID;
    }

    delete readdoc;
    return NO_ERROR;
}

int CNGlobleResourcePool::saveXMLFile( kn_string &filename )
{
    if ( filename.length() == 0 )
    {
        return ERROR_INVALID_PARAMETER;
    }

    string file = ns2s( filename );

    std::replace(filename.begin(), filename.end(), _T('\\'), _T('/'));

    size_t stIndex = filename.rfind( _T( '/' ));
    if ( stIndex != std::string::npos )
    {
        kn_string pathname = filename.substr( 0, stIndex +1 );
        if ( !IsDirectoryExist( pathname.c_str()))
        {
            return ERROR_PATH_NOT_FOUND;
        }
    }
    else
    {
        return ERROR_PATH_NOT_FOUND;
    }

    TiXmlDocument doc;

    TiXmlDeclaration * decl     = new TiXmlDeclaration( "1.0", "UTF-8", "" );
    TiXmlElement*m_pRootElement = new TiXmlElement( "NProject" );

    TiXmlElement *respoolElement  = new TiXmlElement ( "ResourcePool" );
    TiXmlElement *resgroupElement = new TiXmlElement ( "ResourceGroup" );
    vector <CNProjectResGroup>::iterator iter;
    for ( iter = m_resourcegroups.begin(); iter != m_resourcegroups.end(); ++iter )
    {
        TiXmlElement *resgrpElement = new TiXmlElement ( "ResGroup" );
        SetAttributeStr( resgrpElement, "name", iter->getName());
        resgrpElement->SetAttribute( "id", iter->getId());
        resgroupElement->LinkEndChild( resgrpElement );
    }

    TiXmlElement *resElement = new TiXmlElement ( "Resources" );
    vector <CNProjectResource>::iterator ite;
    for ( ite = m_resources.begin(); ite != m_resources.end(); ++ite )
    {
        TiXmlElement *reElement = new TiXmlElement ( "Resource" );
        SetAttributeStr( reElement, "name", ite->getName());
        reElement->SetAttribute( "id", ite->getId());
        reElement->SetAttribute( "type", ite->getType());
        vector <int> grps = ite->getGroups();

        reElement->SetAttribute( "group", IntVector2str( grps ).c_str());
        resElement->LinkEndChild( reElement );
    }

    respoolElement->LinkEndChild( resgroupElement );
    respoolElement->LinkEndChild( resElement );
    m_pRootElement->LinkEndChild( respoolElement );

    doc.LinkEndChild( decl );

    doc.LinkEndChild( m_pRootElement );

    if( !doc.SaveFile( file.c_str()) )
    {
        return ERROR_CREATE_FAILED;
    }

    return NO_ERROR;
}

void CNGlobleResourcePool::addResource( CNProjectResource & res )
{
    if ( res.getId() == 0 )
    {
        res.setId( getLargestResId());
    }

    m_resources.push_back( res );

    // 排序
    std::sort( m_resources.begin(), m_resources.end(), less_nprjobject<CNProjectResource>() );
}

void CNGlobleResourcePool::addResGroup( CNProjectResGroup & resgp )
{
    if ( resgp.getId() == 0 )
    {
        resgp.setId( getLargestResGroupId());
    }

    m_resourcegroups.push_back( resgp );

    // 排序
    sort( m_resourcegroups.begin(), m_resourcegroups.end());
}

int CNGlobleResourcePool::getLargestResId()
{
    if ( m_resources.empty())
    {
        return 1;
    }
    else
    {
        CNProjectResource& res = m_resources.back();
        return (res.getId() + 1);
    }
}

int CNGlobleResourcePool::getLargestResGroupId()
{
    if ( m_resourcegroups.empty())
    {
        return 1;
    }
    else
    {
        CNProjectResGroup& res = m_resourcegroups.back();
        return (res.getId() + 1);
    }
}

//获得资源数量
int  CNGlobleResourcePool::getCount()
{
	return m_resources.size();
}

CNProjectResource* CNGlobleResourcePool::getResByIndex(kn_uint index)
{
	if (index < m_resources.size() )
	{
		return &m_resources[index];
	}
	return NULL;
}

CNProjectResource* CNGlobleResourcePool::getResByID(int id)
{
	return NULL;
}

int CNGlobleResourcePool::loadCSVFile( kn_string &filename )
{
	// 文件名非空
	if ( filename.length() == 0 )
	{
		return ERROR_INVALID_PARAMETER;
	}

	wifstream InputStream;
	kn_int line = 0;
	kn_int event_num = 1;

	// 更改locale规则以支持中文
	InputStream.imbue(locale("chs"));

#ifdef WIN32
	// 打开CSV文件  
	InputStream.open(filename);
#else

    char szFile[MAX_PATH] = {0};
    wcstombs(szFile, filename.c_str(), filename.length());

    InputStream.open(szFile);
#endif

	// 打开失败返回
	if(!InputStream.is_open())
	{
		return ERROR_INVALID_PARAMETER;
	}

	while(InputStream.good())
	{
		kn_string tmpStr;
		getline(InputStream, tmpStr);

		//第一行属性说明不读取
		if( 0 == line++ )
		{
			continue;
		}

		if(tmpStr.empty())
		{
			continue;
		}
		vector <kn_string> vecStr = splitString2Vect(tmpStr.c_str());
		if (vecStr.size() <2)
		{//zhic  safety
			continue;
		}

		if (vecStr[0].empty() || vecStr[1].empty())
		{
			continue;
		}

		//如果改ID存在，不保存
		if (isExitResByID(kn_stringTokn_int(vecStr[1])))
		{
			continue;
		}
		CNProjectResource res;
		res.setName(vecStr[0].c_str()); //name
		res.setId(kn_stringTokn_int(vecStr[1]));                //id
		if (vecStr.size() >=3)
		{
			res.setType(kn_stringTokn_int(vecStr[2]));            //type
		}
		if (vecStr.size() >=4)
		{
			res.setGroup(kn_stringTokn_int(vecStr[3])); // group id
		}
		addResource( res );
	}
	InputStream.close();
	return NO_ERROR;

}

kn_bool CNGlobleResourcePool::isExitResByID( kn_int id )
{
	kn_bool bFind = false;
	int res_count = getCount();
	for (int index = 0 ; index < res_count; index++)
	{
		if (getResByIndex(index)->getId() == id)
		{
			bFind = true;
			return bFind;
		}
	}
	return bFind;

}

void  CNGlobleResourcePool::clear()
{
	m_resourcegroups.clear();
	m_resources.clear();
}
