#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>

void MainWindow::DRAWPAINT_PRESS(QMouseEvent *event)//油漆桶事件&&种子填充算法
{
    int xl,xr,i;
    int x,y;//坐标
    bool spanNeddFill;
    QPoint pt=event->pos();
    while(!unpaint.isEmpty())unpaint.pop();
    unpaint.push(pt);
    QRgb col=QColor::fromRgb(255,255,255).rgba();//原初白色
    while(!unpaint.isEmpty())
    {
        pt=unpaint.pop();
        y=pt.y();
        x=pt.x();
        while(img.pixel(x,y)==col)//向右填充
        {
            drawpix(x,y);
            x++;
            if(x>=990)break;//避免风险
        }
        xr=x-1;
        x=pt.x()-1;
        while(img.pixel(x,y)==col)
        {
            drawpix(x,y);
            x--;
            if(x<=-1)break;//避免风险
        }
        xl=x+1;

        //处理上面一条扫描线
        x=xl;
        y=y+1;
        while(x<=xr&&y<640)
        {
            spanNeddFill=false;
            while(img.pixel(x,y)==col)
            {
                spanNeddFill=true;
                x++;
                if(x>=990)break;//避免风险
            }
            if(spanNeddFill)
            {
                pt.setX(x-1);
                pt.setY(y);
                unpaint.push(pt);
                spanNeddFill=false;
            }
            while(x<=xr&&img.pixel(x,y)!=col)x++;
        }

        //处理下面一条扫描线
        x=xl;
        y=y-2;
        while(x<=xr&&y>-1)
        {
            spanNeddFill=false;
            while(img.pixel(x,y)==col)
            {
                spanNeddFill=true;
                x++;
                if(x>=990)break;//避免风险
            }
            if(spanNeddFill)
            {
                pt.setX(x-1);
                pt.setY(y);
                unpaint.push(pt);
                spanNeddFill=false;
            }
            while(x<=xr&&img.pixel(x,y)!=col)x++;
        }
    }
}
