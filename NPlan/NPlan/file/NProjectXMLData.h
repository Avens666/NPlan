//  File:        NProjectXMLData.h
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
#ifndef NProjectDataXML_h__
#define NProjectDataXML_h__

#include "tinyxml.h"
#include "tinystr.h"
#include "NProjectBaseFile.h"
#include "FilePath.h"
class CNProjectXMLData : public CNProjectBaseFile
{
public:
    CNProjectXMLData( CNProjectData* pData );

    ~CNProjectXMLData( void );

    int load( kn_string &filename, nplan_fileType file_type = XML_PRJ_FILE );

    int save( kn_string &filename, kn_char* lpTemplatefile = NULL );

    bool parseBaseInfo();

	bool parseCalendars();

    bool parseResourcePool();

    bool parseTaskBoard();

    bool parseTasks();

    bool parseEvents();

    bool parseMilestones();

    bool saveBaseInfo();

	bool saveCalendars();

    bool saveResourcePool();

    bool saveTaskBoard();

    bool saveTasks( );

    bool saveMilestones();

    bool saveEvents();
private:

    TiXmlDocument *m_pXmldoc;

    TiXmlElement *m_pRootElement;
};
#endif
