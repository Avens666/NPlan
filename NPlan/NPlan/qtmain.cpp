//  File:        qtmain.cpp
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

/*===============================================
 Implement wayland / Qt
int InitWindow(int width, int height, int argc, char **argv);
int RunEventLoop(bool& bExit);
int RunModalEventLoop(bool& bExit);
void ReleaseWindow();

void glenvFlush();
void EndLoop(int nExitCode);
===============================================*/


#include <QApplication>
#include "qtmainwindow.h"
#include "message.h"

static bool s_bQuitLoop = false ;//退出消息循环
static long lTicks = 0;
long GetTickCount();
void SendTimerEvent(int num);

QApplication* pQtApp = NULL;
MainWindow* pWindow = NULL;

void SendQtMessage(int hWnd, int message, int wParam, int lParam);

int InitWindow(int width, int height, int argc, char *argv[])
{
    pQtApp = new QApplication (argc, argv);


    pWindow = new MainWindow;
    pWindow->setFixedSize(width, height);
    pWindow->show();

    return 0;
}


int RunModalEventLoop(bool& bExit)
{

//    while (!bExit)
//    {
//        pQtApp->processEvents ();
//        usleep(1000);

//    }


    while (!bExit && !s_bQuitLoop)
    {

        pQtApp->processEvents ();

        if(GetTickCount() - lTicks > 10)
        {
           SendTimerEvent(0);
           lTicks = GetTickCount();
        }

        usleep(20000);
    }

    return 0;

}


int RunEventLoop(bool& bExit)
{

    // pQtApp->exec();

    // same as wayland / gtk
    return RunModalEventLoop(bExit);



}

void EndLoop(int nExitCode)
{
   // pQtApp->exit(nExitCode);

     s_bQuitLoop = true;
}


void ReleaseWindow()
{
    delete pWindow;
    pWindow = NULL;

    delete  pQtApp;
    pQtApp = NULL;


}


void glenvFlush()
{
    if(pWindow)
    {
        pWindow->updateWindow();
        //qDebug("swapBuffers");
    }


}

//void flipScreen(char* pSrcBuf)
//{
//    if(pWindow)
//    {
//        pWindow->updateWindow(pSrcBuf);
//        //qDebug("swapBuffers");
//    }

//}

extern int g_iScreenWidth;
extern int g_iScreenHeight;

extern bool g_bExit ;

int main( int argc, char *argv[] )
{

    //	启动wanyland窗口 / Qt Window
     if (0 != InitWindow(g_iScreenWidth, g_iScreenHeight, argc, argv))
     {
         return 0;
     }



    RunEventLoop(g_bExit);


    ReleaseWindow();

}



