//  File:        qtmainwindow.cpp
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
#include "qtmainwindow.h"
#include <stdio.h>
#include <QApplication>
#include <QMouseEvent>
#include "message.h"
#include "LINUXTCHAR.h"

extern int g_iScreenWidth;
extern int g_iScreenHeight;



void CreateQtSoftDeviceSurface(QLabel* pLabel);

void SendQtMessage(int hWnd, int message, int wParam, int lParam);

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent, Qt::FramelessWindowHint/*Qt::WStyle_Customize | Qt::WStyle_NoBorder*/)
{



//#ifdef SOFTRENDER
//    m_softwidget = new KSoftWidget(this);
//    setCentralWidget(m_softwidget);
//#else
//    m_glwidget = new KGLWidget(this);
//    setCentralWidget(m_glwidget);
//#endif


    m_label = new QLabel(this);
    //m_label->setFixedSize(g_iScreenWidth, g_iScreenHeight);


    setCentralWidget(m_label);
    CreateQtSoftDeviceSurface(m_label);
   // startTimer(10);

//    m_image = new QImage(g_iScreenWidth, g_iScreenHeight, QImage::Format_RGB16);
//    m_image->fill(Qt::cyan);

//    m_label->setPixmap(QPixmap::fromImage (*m_image));

      this->setMouseTracking(true);
    m_bPress = false;

}


MainWindow::~MainWindow()
{

}

void MainWindow::mousePressEvent(QMouseEvent* event)
{

    int message = KMSG_LBBUTTONDOWN;

    Qt::MouseButton button = event->button();
    if(button == Qt::LeftButton)
    {
        message = KMSG_LBBUTTONDOWN;
    }
    else if(button == Qt::RightButton)
    {
        message = KMSG_RBBUTTONDOWN;
    }
    else if(button == Qt::MiddleButton)
    {
        message = KMSG_MBBUTTONDOWN;
    }



    SendQtMessage(0, message, event->pos().x(), event->pos().y());
    m_bPress = true;
}

// 只有鼠标按下才触发
void MainWindow::mouseMoveEvent(QMouseEvent* event)
{

    SendQtMessage(0, KMSG_MOUSEMOVE, event->pos().x(), event->pos().y());
    //printf("mouseMoveEvent ....\n");
}

bool MainWindow::event(QEvent * event)
{
   // printf("event type %d\n", event->type());

    if(!m_bPress && QEvent::HoverMove ==  event->type())
    {
        SendQtMessage(0, KMSG_MOUSEMOVE, ((QHoverEvent*)event)->pos().x(), ((QHoverEvent*)event)->pos().y());
    }

    return QMainWindow::event(event);

}


void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    int message = KMSG_LBBUTTONUP;

    Qt::MouseButton button = event->button();
    if(button == Qt::LeftButton)
    {
        message = KMSG_LBBUTTONUP;
    }
    else if(button == Qt::RightButton)
    {
        message = KMSG_RBBUTTONUP;
    }
    else if(button == Qt::MiddleButton)
    {
        message = KMSG_MBBUTTONUP;
    }

    SendQtMessage(0, message, event->pos().x(), event->pos().y());

    m_bPress = false;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    SendQtMessage(0, KMSG_LBUTTONDBLCLK, event->pos().x(), event->pos().y());
}


void MainWindow::wheelEvent(QWheelEvent * event)
{
    int wParam = MAKELONG(event->pos().x(), event->pos().y());
    SendQtMessage(0, KMSG_MOUSEWHEEL, wParam, event->delta());


}


void MainWindow::timerEvent(QTimerEvent *event)
{
    qDebug("timerEvent");
//    SendTimerEvent(0);

}

void SwitchWindow(kn_bool b);


int ConvertQtKeyToWinKey(int iQtKey)
{
    int QtKeyTable[] = {
        Qt::Key_Tab,
        Qt::Key_Up,
        Qt::Key_Down,
        Qt::Key_Left,
        Qt::Key_Right,
        Qt::Key_Escape,
        Qt::Key_Return,
        Qt::Key_Back,
        Qt::Key_PageUp,
        Qt::Key_PageDown

    };
    int WinKeyTable[] = {
        '\t',
        VK_UP,
        VK_DOWN,
        VK_LEFT,
        VK_RIGHT,
        VK_ESCAPE,
        VK_RETURN,
        VK_BACK,
        VK_PRIOR,
        VK_NEXT};


    ASSERT(sizeof(QtKeyTable) == sizeof(WinKeyTable));
    int count = sizeof(QtKeyTable) / sizeof(QtKeyTable[0]);

    for(int i = 0; i< count; i++)
    {
        if(QtKeyTable[i] == iQtKey)
        {
            return WinKeyTable[i];
        }
    }

    return iQtKey;




}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    int iWinKey = ConvertQtKeyToWinKey(event->key());

    SendQtMessage(0, KMSG_KEYDOWN, iWinKey, event->count());

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int iWinKey = ConvertQtKeyToWinKey(event->key());
    SendQtMessage(0, KMSG_KEYUP, iWinKey, event->count());
}


void MainWindow::updateWindow(char* pSrcBuf)
{
//#ifdef SOFTRENDER
//    if(m_softwidget)
//    {
//        m_softwidget->flip(pSrcBuf);
//    }
//#else

//    if(m_glwidget)
//    {
//        m_glwidget->swapBuffers();
//    }
//#endif
}

#include "KKoteiMessage.h"

void SendTimerEvent(int num)
{
    KMessage* pMsg = GetKMessage();
    if(pMsg)
    {
        //SendQtMessage(0, KMSG_USER, pMsg->m_msg_type, pMsg->m_wParam);
        SendQtMessage(0, KMSG_USER, pMsg, 0);
        //SAFE_DELETE(pMsg);
    }

    SendQtMessage(0, KMSG_TIMER, GetTickCount() / 10, 0);


}


