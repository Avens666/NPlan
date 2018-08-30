//  File:        XExcel.cpp
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
#ifdef WIN32   //EXCEL导入导出仅在PC版提供

#include "XExcel.h"
#include <io.h>
#include <assert.h>

#define NULL_RETURN_EX( ptr, val ) if( ptr == NULL ) \
    { \
        return val; \
    }

CXExcel::CXExcel( bool auto_init_excel /*=true*/ ) : m_spXLApp( NULL ), m_spSheet( NULL ), m_spbk( NULL )
{
    if ( auto_init_excel )
    {
        InitExcel();
    }
}

CXExcel::~CXExcel( void )
{
    QuitExcel();
}

BOOL CXExcel::InitExcel( bool disable_warn /* = true*/ )
{
    if ( m_spXLApp == NULL )
    {
        m_spXLApp.CreateInstance( _T( "Excel.Application" ));
        NULL_RETURN_EX( m_spXLApp, FALSE );
    }

    m_spXLApp->PutVisible( 0, VARIANT_FALSE );

    Excel::WorkbooksPtr spBooks = m_spXLApp->Workbooks;
    NULL_RETURN_EX( spBooks, FALSE );

    Excel::_WorkbookPtr spBook = spBooks->Add( 1L );
    NULL_RETURN_EX( spBook, FALSE );

    m_spbk = spBook;

    m_spSheet = m_spXLApp->ActiveSheet;
    NULL_RETURN_EX( m_spSheet, FALSE );

    if ( disable_warn )
    {
        m_spXLApp->put_DisplayAlerts( 0L, VARIANT_FALSE );
    }

    return TRUE;
}

bool CXExcel::open( kn_char *sz_path )
{
    if ( m_spXLApp == NULL )
    {
        m_spXLApp.CreateInstance( _T( "Excel.Application" ));
        NULL_RETURN_EX( m_spXLApp, FALSE );
    }

    m_spXLApp->PutVisible( 0, VARIANT_FALSE );

    if( !sz_path || !*sz_path || (_taccess( sz_path, 0 ) == -1) || !m_spXLApp )
    {
        return false;
    }

    Excel::WorkbooksPtr pbks = m_spXLApp->Workbooks;
    if( !pbks )
    {
        return false;
    }

    m_spbk = pbks->Open( sz_path );

    m_spSheet = m_spXLApp->ActiveSheet;
    NULL_RETURN_EX( m_spSheet, FALSE );

    return true;
}

bool CXExcel::SaveAs( kn_char *file )
{
    bool _result = false;

    if ( m_spSheet != NULL )
    {
        if ( _taccess( file, 0 ) != -1 )
        {
            if ( FALSE == DeleteFile( file ))
            {
                MessageBox( NULL, _T( "文件被其他程序占用！" ), _T( "导出失败！" ), MB_ICONWARNING );
                m_spbk->Close( VARIANT_FALSE );
                return false;
            }
        }

        HRESULT hr = m_spSheet->SaveAs((_bstr_t)file );
        _result = SUCCEEDED( hr );
    }

    return _result;
}

// 由列号索引转换为列名
CString CXExcel::GetColName( long nColIndexFrom0 /*索引从0开始*/ )
{
    CString sName = _T( "" );
    char c1, c2;

    if ((nColIndexFrom0 >= 0) && (nColIndexFrom0 < 26))
    {
        c1 = 'A'+nColIndexFrom0;
        sName.Format( _T( "%c" ), c1 );
    }

    //AA,AB,AC
    if ((nColIndexFrom0 >= 26) && (nColIndexFrom0 < 26*26))
    {
        long n1, n2;
        n1 = nColIndexFrom0/26;
        n2 = nColIndexFrom0%26;

        c1 = 'A'+n1-1;
        c2 = 'A'+n2;
        sName.Format( _T( "%c%c" ), c1, c2 );
    }

    return sName;
};

void CXExcel::SetFont( LPCTSTR lpszRange, long nftsize /* = DEFAULT_VAL*/, BOOL BoldFont /* = DEFAULT_VAL*/ )
{
    if ( !lpszRange || !*lpszRange )
    {
        return;
    }

    Excel::RangePtr pRange = GetRange( lpszRange );

    FontPtr pFont = pRange->Font;
    assert( pFont != NULL );

    if( DEFAULT_VAL != nftsize )
    {
        SetFontSize( pRange, nftsize );
    }

    if( DEFAULT_VAL != BoldFont )
    {
        SetBoldFont( pRange, BoldFont );
    }
}

void CXExcel::SetSheetRowHeight( const double Height )  //对当前页所有行有效
{
    _ASSERT( m_spSheet != NULL );
    m_spSheet->Rows->PutRowHeight( Height );
}

void CXExcel::SetSheetColumnWidth( const double Width ) //对当前页所有列有效
{
    _ASSERT( m_spSheet != NULL );
    m_spSheet->Rows->PutColumnWidth( Width );
}

//读取单元格内容
void CXExcel::getData( const kn_char* pos, CString &strValue )
{
    strValue.Empty();

    _ASSERT( m_spSheet != 0 );

    if( NULL != pos )
    {
        Excel::RangePtr pRange = 0;
        m_spSheet->get_Range( _variant_t( pos ), vtMissing, &pRange );
        _variant_t val = pRange->GetValue();
        if ( val.vt != VT_SAFEARRAY )  //合并单元格，会返回数组，此时取第一个单元格即可
        {
            strValue = (LPCTSTR)(_bstr_t)val;
        }
    }
}

//读取时间类型单元格的内容
void CXExcel::getTimeData( const kn_char* pos, CString &strValue )
{
    strValue.Empty();

    _ASSERT( m_spSheet != 0 );

    if( NULL != pos )
    {
        Excel::RangePtr pRange = 0;
        m_spSheet->get_Range( _variant_t( pos ), vtMissing, &pRange );
        _variant_t val = pRange->GetValue();
        if ( val.vt == VT_R8 )
        {
            double dfVal = val.dblVal;
            if ( dfVal > 0 )
            {
                int hours = dfVal*24;
                int minutes = dfVal*24*60-hours*60;
                int seconds = dfVal*24*60*60-minutes*60-hours*60*60;
                strValue.Format( _T( "%02d:%02d:%02d" ), hours, minutes, seconds );
            }
        }
    }
}

void CXExcel::SetRangeRowHeight( LPCTSTR lpszRange, const double Height )       //设置行高[可以为行,也可以为若干范围的行高]
{
    if ( !lpszRange || !*lpszRange )
    {
        return;
    }

    Excel::RangePtr rng = GetRange( lpszRange );
    if ( rng )
    {
        rng->PutRowHeight( Height );
    }
}

void CXExcel::SetRangeColumnWidth( LPCTSTR lpszRange, const double Width )      //设置列宽[可以为列,也可以为若干范围的列宽]
{
    if ( !lpszRange || !*lpszRange )
    {
        return;
    }

    Excel::RangePtr rng = GetRange( lpszRange );
    if ( rng )
    {
        rng->PutColumnWidth( Width );
    }
}

void CXExcel::SetRowHeight( const long nRowIndexFromOne, const double height )  //SetRangeRowHeight 调用包装,for invoke happy
{
    CString sRng;

    sRng.Format( _T( "%d:%d" ), nRowIndexFromOne, nRowIndexFromOne );

    SetRangeRowHeight( sRng, height );
}

void CXExcel::SetColumnWidth( const long nCol, const double Width ) //SetRangeColumnWidth 调用包装,for invoke happy
{
    CString sRng, sCol = GetColName( nCol );

    sRng.Format( _T( "%s:%s" ), sCol.c_str(), sCol.c_str());

    SetRangeColumnWidth( sRng, Width );
}

bool CXExcel::SetRangeAlignMode( Excel::RangePtr &rg, long nHAlignType, long nVAlignType )
{
    if( NULL == rg )
    {
        return false;
    }

    switch( nVAlignType )
    {
    case DEFAULT_VAL:
        rg->VerticalAlignment = long( Excel::xlCenter );
        break;
    case 1:
        rg->VerticalAlignment = long( Excel::xlLeft );
        break;
    case 2:
        rg->VerticalAlignment = long( Excel::xlRight );
        break;
    }

    switch( nHAlignType )
    {
    case DEFAULT_VAL:
        rg->HorizontalAlignment = long( Excel::xlCenter );
        break;
    case 1L:
        rg->HorizontalAlignment = long( Excel::xlLeft );
        break;
    case 2:
        rg->HorizontalAlignment = long( Excel::xlRight );
        break;
    default:
        break;
    }

    return true;
}

bool CXExcel::AutofitCol( LPCTSTR szColName )
{
    if((NULL == m_spSheet) || !szColName || !*szColName )
    {
        _ASSERT( 0 );
        return false;
    }

    kn_char szGPS[32] = {0};
    _stprintf( szGPS, _T( "%s:%s" ), szColName, szColName );

    _variant_t varColumns = m_spSheet->Columns->Item[szGPS];
    if ((varColumns.vt != VT_EMPTY) && (varColumns.vt != VT_NULL))
    {
        Excel::RangePtr pRange = (IUnknown *)varColumns;

        if ( NULL == pRange )
        {
            return false;
        }

        pRange->EntireColumn->AutoFit();
    }

    return true;
}

void CXExcel::AutoFitColumns( LPCTSTR szGPS )
{
    if((NULL == m_spSheet) || !szGPS || !*szGPS )
    {
        _ASSERT( 0 );
        return;
    }

    _variant_t varColumns = m_spSheet->Columns->Item[szGPS];
    if ((varColumns.vt != VT_EMPTY) && (varColumns.vt != VT_NULL))
    {
        Excel::RangePtr pRange = (IUnknown *)varColumns;
        if ( pRange )
        {
            pRange->AutoFit();
        }
    }
}

//填充单元格
Excel::RangePtr CXExcel::WriteCell( CString &sGps, LPCTSTR Value, bool putStr, bool warpText )
{
    _ASSERT( m_spSheet != 0 );

    Excel::RangePtr pRange = m_spSheet->GetRange((LPCTSTR)sGps );
    if ( pRange )
    {
        if ( putStr )
        {
            pRange->NumberFormatLocal = _T( "@" );    //将输入数据作为字符串处理[避免强制转换为数字类型]
        }

        if ( sGps.FindOneOf( _T( ":" )) != -1 )
        {
            pRange->Merge();
        }

        pRange->Value = Value;

        //设置对齐方式
        pRange->VerticalAlignment = long( Excel::xlCenter );
        pRange->HorizontalAlignment = long( Excel::xlCenter );

        if ( warpText )
        {
            pRange->WrapText = true;
        }
    }

    return pRange;
}

Excel::RangePtr CXExcel::WriteCell( CString &sGps, CString &Value, bool putStr, bool warpText )
{
    return WriteCell( sGps, (LPCTSTR)Value, putStr, warpText );
}

Excel::RangePtr CXExcel::WriteCell( CString &sGps, int Value, bool putStr, bool warpText )
{
    CString str;

    str.Format( _T( "%d" ), Value );

    return WriteCell( sGps, str, putStr, warpText );
}

//为单元格设置边框
void CXExcel::SetRangeBorderEx( Excel::RangePtr &pRange, bool Left, bool Top, bool Right, bool Bottom )
{
    _ASSERT( pRange != 0 );

    if ( pRange )
    {
        Excel::BorderPtr border = NULL;

        if ( Left )
        {
            border = pRange->Borders->Item[ Excel::xlEdgeLeft  ];
            border->LineStyle = long( Excel::xlContinuous );
        }

        if ( Top )
        {
            border = pRange->Borders->Item[ Excel::xlEdgeTop ];
            border->LineStyle = long( Excel::xlContinuous );
        }

        if ( Right )
        {
            border = pRange->Borders->Item[ Excel::xlEdgeRight ];
            border->LineStyle = long( Excel::xlContinuous );
        }

        if ( Bottom )
        {
            border = pRange->Borders->Item[ Excel::xlEdgeBottom ];
            border->LineStyle = long( Excel::xlContinuous );
        }
    }
}

void CXExcel::SetRangeBorder( Excel::RangePtr &pRange )
{
    _ASSERT( pRange != 0 );

    if ( pRange )
    {
        pRange->Borders->LineStyle = Excel::xlContinuous;
    }
}

void CXExcel::SetPageUp( bool directofVerti, bool centerAtHorizon )
{
    _ASSERT( m_spSheet != 0 );

    Excel::PageSetupPtr page = m_spSheet->GetPageSetup();
    if ( page )
    {
        if ( centerAtHorizon )
        {
            page->PutCenterHorizontally( VARIANT_TRUE );
        }
        else
        {
            page->PutCenterVertically( VARIANT_TRUE );
        }

        if ( directofVerti )
        {
            page->PutOrientation( Excel::xlPortrait );
        }
        else
        {
            page->PutOrientation( Excel::xlLandscape );
        }
    }
}

Excel::_ChartPtr CXExcel::Outpukn_chart( const CString & sRangeIndex, LPCTSTR lpszTitle /*=_T("饼状图")*/, long Gallery /*=Excel::xlPie*/ )
{
    _ASSERT( m_spXLApp != NULL );
    _ASSERT( m_spSheet != NULL );

    Excel::_ChartPtr pChart = NULL;

    Excel::RangePtr pRange = NULL;
    m_spSheet->get_Range( _variant_t( _bstr_t( sRangeIndex )), vtMissing, &pRange );

    Excel::_WorkbookPtr pbk = m_spXLApp->ActiveWorkbook;

    if( pbk && pRange )
    {
        pRange->Select();

        pChart = pbk->Charts->Add();

        _variant_t vtSource((IDispatch*)pRange );

        pChart->put_Name( _bstr_t( lpszTitle ));

        pChart->ChartWizard( vtSource, Gallery, vtMissing, (long) Excel::xlRows,
                             1L /*包含标题的行或列的个数*/, vtMissing, vtMissing, lpszTitle, vtMissing, VARIANT_TRUE );

        m_spSheet->Select();
    }

    return pChart;
}

bool CXExcel::CopySheetFromTemplate( const kn_char * sz_path,
                                     long begrow, long begcol, long endrow, long endcol, long nSheetIndex )
{
    if ( m_spXLApp == NULL )
    {
        m_spXLApp.CreateInstance( _T( "Excel.Application" ));
        NULL_RETURN_EX( m_spXLApp, FALSE );
    }

    m_spXLApp->PutVisible( 0, VARIANT_FALSE );

    if( !sz_path || !*sz_path || (_taccess( sz_path, 0 ) == -1) || !m_spXLApp )
    {
        return false;
    }

    Excel::WorkbooksPtr pbks = m_spXLApp->Workbooks;
    if( !pbks )
    {
        return false;
    }

    Excel::_WorkbookPtr pbk = NULL;
    pbk = pbks->Open( sz_path );

    if( !pbk )
    {
        return false;
    }

    Excel::SheetsPtr psheets = pbk->Sheets;

    if( !psheets )
    {
        return false;
    }

    if ( nSheetIndex < 1L )
    {
        nSheetIndex = 1L;
    }

    if ( psheets->Count < nSheetIndex )
    {
        assert( 0 && L"sheet页索引不正确,该页可能不存在!" );
        return false;
    }

    Excel::_WorksheetPtr pTmpSheet = psheets->GetItem( nSheetIndex );

    if( !pTmpSheet )
    {
        return false;
    }

    Excel::RangePtr pRange = pTmpSheet->Cells->GetItem( begrow, begcol );
    if( !pRange )
    {
        return false;
    }

    pRange = pRange->GetResize( endrow, endcol );
    if( !pRange )
    {
        return false;
    }

    Excel::_WorkbookPtr spBook = m_spbk;

    if ( spBook == NULL )
    {
        spBook = pbks->Add( 1L );
        m_spbk = spBook;
    }

    if ( spBook )
    {
        Excel::SheetsPtr pNewSheets = spBook->Sheets;

        if ( !pNewSheets )
        {
            return false;
        }

        if ((pNewSheets->Count < nSheetIndex) && (m_spSheet != NULL))
        {
            //输出的EXCEL有多页,模板文件也有多页时,输出文件的sheet不够,自动添加
            m_spSheet = pNewSheets->Add( vtMissing, _variant_t((IDispatch*)m_spSheet ), 1L );
            m_spSheet = pNewSheets->GetItem( nSheetIndex );
        }
        else
        {
            m_spSheet = spBook->ActiveSheet;  //只输出一个sheet
        }

        if( m_spSheet )
        {
            Excel::RangePtr pRangeNew = m_spSheet->Cells->GetItem( 1L, 1L );

            _variant_t vDesc((IDispatch*)pRangeNew );
            pRange->Copy( vDesc );

            if( pbk )
            {
                pbk->Close( VARIANT_FALSE );
            }

            return true;
        }
    }

    if( pbk )
    {
        pbk->Close();
    }

    return false;
}

bool CXExcel::FreezeRow( const int nRow )
{
    _ASSERT( m_spSheet != 0 );

    CString strTemp = _T( "" );
    strTemp.Format( _T( "%d:%d" ), nRow, nRow );

    return FreezeWindows( strTemp );
}

bool CXExcel::FreezeWindows( const CString &sGps )
{
    _ASSERT( m_spSheet != 0 );

    Excel::RangePtr pRange = GetRange( sGps );

    if ( pRange )
    {
        pRange->Select();

        m_spXLApp->ActiveWindow->FreezePanes = VARIANT_TRUE;

        return true;
    }

    return false;
}

/**
 *	插入复制的行（行号从1开始）
 */
void CXExcel::CopyRows( int beg_row, int end_row, int insert_row, _variant_t Shift /* = Excel::xlDown*/ )
{
    CString sRng;

    sRng.Format( _T( "%d:%d" ), beg_row, end_row );

    Excel::RangePtr pRange = GetRange( sRng );
    if ( pRange )
    {
        InsertRow( insert_row, end_row - beg_row + 1 );

        sRng.Format( _T( "%d:%d" ), insert_row, insert_row );
        Excel::RangePtr pInsertRange = GetRange( sRng );

        _variant_t vDesc((IDispatch*)pInsertRange );
        pRange->Copy( vDesc );
    }
}

/**
 *	在当前行的某个方向插入新行（行号从1开始）
 */
void CXExcel::InsertRow( int iRow, int count /* = 1*/, _variant_t Shift /* = Excel::xlDown*/ )
{
    CString sRng;

    sRng.Format( _T( "%d:%d" ), iRow, iRow );

    Excel::RangePtr pRange = GetRange( sRng );
    if ( pRange )
    {
        for ( int i = 0; i < count; i++ )
        {
            pRange->Insert( Shift );
        }
    }
}

#endif // WIN32
