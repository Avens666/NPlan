//  File:        XExcel.h
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
/**
 * Copyright (C) 2003-2013，光庭信息有限公司(http://www.kotei-info.com)
 * All rights reserved.
 *
 * 文件名称：XExcel.h
 * 摘    要：Ｅxcel报表终极帮助类
 *
 * 版    本：1.0
 * 创建日期： 2008?
 * 作    者：jiaqim
 *
 * 修改历史：
 * [修改序列][修改日期][修改者][修改内容]
 **/
#ifdef WIN32      //EXCEL导入导出仅在PC版提供

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "mso9.tlh"
#include "VBE6EXT.tlh"
#include "excel9.tlh"
#include "StdString.h"
#include "NE_type.h"
#ifdef _UNICODE
typedef CStdStringW  CString;
#else
typedef CStdStringA  CString;  
#endif // _UNICODE

//////////////////////////////////////////////////////////////////////////
class CXExcel
{
public:
    CXExcel( bool auto_init_excel = true );

    virtual ~CXExcel( void );

public:
    Excel::_ApplicationPtr m_spXLApp;

    Excel::_WorkbookPtr m_spbk;

    Excel::_WorksheetPtr m_spSheet;

#define   DEFAULT_VAL -1

public:
    BOOL InitExcel( bool disable_warn = true );

    bool open( kn_char *file );

    //读取单元格内容
    void getData( const kn_char* pos, CString &strValue );

    //读取时间类型单元格的内容
    void getTimeData( const kn_char* pos, CString &strValue ); 

protected:
    void SetRangeRowHeight( LPCTSTR lpszRange, const double Height );        //设置行高[可以为行,也可以为若干范围的行高]
    void SetRangeColumnWidth( LPCTSTR lpszRange, const double Width );       //设置列宽[可以为列,也可以为若干范围的列宽]

    void SetRowHeight( const long nRowIndexFromOne, const double height );   //SetRangeRowHeight 调用包装,for invoke happy
    void SetColumnWidth( const long nColIndexFromZero, const double width ); //SetRangeColumnWidth 调用包装,for invoke happy

    void SetSheetRowHeight( const double Height );                           //对当前页所有行有效
    void SetSheetColumnWidth( const double Width );                          //对当前页所有列有效

    void SetFont( LPCTSTR lpszRange, long nftsize = DEFAULT_VAL, BOOL BoldFont = DEFAULT_VAL );

    void SetFontSize( Excel::RangePtr &pRange, long size )
    {
        _ASSERT( pRange != 0 );
        pRange->Font->PutSize( size );
    }

    void SetBoldFont( Excel::RangePtr &pRange, VARIANT_BOOL bold = VARIANT_TRUE )
    {
        _ASSERT( pRange != 0 );
        pRange->Font->PutBold( bold );
    }

    void DisableNoSaveWarning( bool warn = false )
    {
        if( m_spXLApp ) {m_spXLApp->put_DisplayAlerts( 0L, warn ? VARIANT_TRUE : VARIANT_FALSE ); }
    }

    void ShowExcel( VARIANT_BOOL show = VARIANT_TRUE )
    {
        if ( m_spXLApp ) {m_spXLApp->PutVisible( 0, show ); }
    }

    void QuitExcel()
    {
        if ( m_spSheet )
        {
            m_spSheet.Release();
            m_spSheet = NULL;
        }

        if ( m_spbk )
        {
            m_spbk->Close(VARIANT_FALSE);
            m_spbk.Release();
            m_spbk = NULL;
        }

        if ( m_spXLApp )
        {
            m_spXLApp->Quit();
            m_spXLApp.Release();
            m_spXLApp = NULL;
        }
    }

    void Zoom( unsigned int precent )
    {
        _ASSERT( m_spXLApp != 0 );
        m_spXLApp->ActiveWindow->PutZoom( precent );
    }

    void AutoFitColumns( LPCTSTR szGPS );

    bool AutofitCol( LPCTSTR szColName );

    void AutoFitColumns( CString &sGPS )
    {
        AutoFitColumns((LPCTSTR)sGPS );
    }

    bool CopySheetFromTemplate( const kn_char * sz_path,
                                long begrow, long begcol, long endrow, long endcol, long nSheetIndex = 1L );

    bool FreezeWindows( const CString &sGps ); //ALL FreezeWindow can call this

    bool FreezeRow( const int nRow );          //just for invoke freezeWindow	easy as row

protected:
    void SetPaperSize( Excel::XlPaperSize papersize = Excel::xlPaperA3 )
    {
        if( NULL == m_spSheet ) {return; }

        Excel::PageSetupPtr page = m_spSheet->GetPageSetup();
        if ( NULL != page ) {page->PaperSize = papersize; }
    }

    void SetPrintOrientation( Excel::XlPageOrientation orien = Excel::xlLandscape )
    {
        if( NULL == m_spSheet ) {return; }

        Excel::PageSetupPtr page = m_spSheet->GetPageSetup();
        if ( NULL != page ) {page->Orientation = orien; }
    };

    void SetPageBreakLocation( int nLocation, int nColIndexFromZero )
    {
        if ((nColIndexFromZero < 1) || (NULL == m_spSheet)) { _ASSERT( 0 ); return;    }

        CString strTmp = _T( "" );
        strTmp.Format( _T( "%s1" ), GetColName( nColIndexFromZero ));

        Excel::VPageBreakPtr pbrk = m_spSheet->VPageBreaks->GetItem( nLocation );

        if ( NULL != pbrk )
        {
            try
            {
                pbrk->PutRefLocation( GetRange( strTmp ));
            } catch ( _com_error &e ) {
                //TRACE(_T("%s"), (LPCTSTR)e.Description());
            }
        }
    }

    CString GetColName( long nColIndexFrom0 /*索引从0开始*/ ); // 由列号索引转换为列名

    //填充单元格[若须按原样写入,指定格式为字符串putStr=true]
    Excel::RangePtr WriteCell( CString &sGps, LPCTSTR Value, bool putStr = false, bool warpText = false );

    Excel::RangePtr WriteCell( CString &sGps, CString &Value, bool putStr = false, bool warpText = false );

    Excel::RangePtr WriteCell( CString &sGps, int Value, bool putStr = false, bool warpText = false );

    //为单元格设置边框[缺省设置左上右下四个边框]
    void SetRangeBorderEx( Excel::RangePtr &pRange, bool Left = true, bool Top = true, bool Right = true,
                           bool Bottom = true );

    void SetRangeBorder( Excel::RangePtr &pRange ); //多行一起设置

    void SetPageUp( bool directofVerti = true, bool centerAtHorizon = true );

    Excel::RangePtr CXExcel::GetRange( LPCTSTR lpszRange )
    {
        _ASSERT( m_spSheet != 0 );

        Excel::RangePtr pRange = 0;
        m_spSheet->get_Range( _variant_t( lpszRange ), vtMissing, &pRange );

        return pRange;
    }

    Excel::RangePtr CXExcel::GetRange( const CString &sGps )
    {
        _ASSERT( m_spSheet != 0 );

        Excel::RangePtr pRange = 0;
        m_spSheet->get_Range( _variant_t( _bstr_t( sGps )), vtMissing, &pRange );

        return pRange;
    }

    bool CXExcel::PutRangeValAsStr( Excel::RangePtr &rg, bool ValAsStr = true )
    {
        if( NULL == rg ) {return false; }

        if ( ValAsStr )
        {
            rg->NumberFormatLocal = _T( "@" );
        }
        else
        {
            rg->NumberFormatLocal = _T( "" );
        }

        return true;
    }

    bool CXExcel::SetRangeAlignMode( Excel::RangePtr &rg, long nHAlignType = DEFAULT_VAL,
                                     long nVAlignType = DEFAULT_VAL );

    bool SetRnageMultiRow( Excel::RangePtr &rg, bool autoWarp = true )
    {
        if( NULL == rg ) {return false; }

        rg->WrapText = autoWarp;

        return true;
    }

    Excel::_ChartPtr Outpukn_chart( const CString & sRangeIndex, LPCTSTR lpszTitle = _T(
                                      "饼状图" ), long Gallery = Excel::xlPie );

    void FillCell( const CString &strRangeFormat, const CString & strValue )
    {
        CString strRange = strRangeFormat;

        Excel::RangePtr pRange = GetRange( strRange );

        if ( pRange ) {pRange->Value = (LPCTSTR)strValue; }
    }

    void SetRangeAlignment( Excel::RangePtr &pRange, long nHAlign = Excel::xlCenter, long nVAlign = Excel::xlCenter )
    {
        if ( pRange )
        {
            pRange->VerticalAlignment   = nHAlign;
            pRange->HorizontalAlignment = nVAlign;
        }
    }

    CString GetRangeFormatStr( int nxsws = 1 )
    {
        CString strTmp = _T( "" );

        if ( nxsws < 1 ) { nxsws = 1; }

        strTmp.Format( _T( "0.%0*d" ), nxsws, 0 );

        return strTmp;
    }

    void SetRangeFormat( LPCTSTR lpszGps, LPCTSTR lpszFmtStr = _T( "0" ))
    {
        _ASSERT( lpszGps != NULL && *lpszGps );

        Excel::RangePtr pRange = NULL;
        m_spSheet->get_Range( _variant_t( _bstr_t( lpszGps )), vtMissing, &pRange );

        if( pRange )
        {
            pRange->NumberFormatLocal = lpszFmtStr;
            SetRangeAlignment( pRange );
        }
    }

    /**
     *	在当前行的某个方向插入新行（行号从1开始）
     */
    void InsertRow( int iRow, int count = 1, _variant_t Shift = Excel::xlDown );

    bool SaveAs( kn_char *file );

    void CopyRows( int beg_row, int end_row, int insert_row, _variant_t Shift = Excel::xlDown );
};
#endif
