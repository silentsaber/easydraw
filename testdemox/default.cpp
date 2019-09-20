#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>

void MainWindow::DEFAULT_PRESS(QMouseEvent *event)//按下，当前这个点染色
{
    int x=event->x();
    int y=event->y();
    curpoint=lastpoint=event->pos();//当前点
    drawpix(x,y);

//    //防抖
//    ctrpoint.clear();
//    ctrpoint.push_back(event->pos());
//    memset(vis,0,sizeof(vis));
//    vis[x][y]=1;
}
void MainWindow::DEFAULT_MOVE(QMouseEvent *event)//按下，当前这个点染色
{
    lastpoint=curpoint;//上一次的点
    curpoint=event->pos();//当前的点
    int x=event->x();
    int y=event->y();
    drawline(lastpoint,curpoint);
//    if(!vis[x][y])
//    {
//        vis[x][y]=1;
//        ctrpoint.push_back(event->pos());
//    }
}

void MainWindow::DEFAULT_UP(QMouseEvent *event)//按下，当前这个点染色
{
    int x=event->x();
    int y=event->y();
    drawpix(x,y);
//    if(!vis[x][y])
//    {
//        vis[x][y]=1;
//        ctrpoint.push_back(event->pos());
//    }
//    maxlevel=10;
//    drawhighbezier(ctrpoint.size(),ctrpoint,1);
}

void MainWindow::bigpoint(QPoint &p)//画一个大点，为控制点
{
    int x=p.x(),y=p.y();
    for(int i=x-3;i<=x+3;i++)
    {
        for(int j=y-3;j<=y+3;j++)
        {
            if(i>0&&i<datawidth&&j>0&&j<dataheight)
            {
                drawpix(i,j);
            }
        }
    }
}
