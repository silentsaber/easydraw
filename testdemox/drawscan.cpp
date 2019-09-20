#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>

void MainWindow::DRAWSCAN()
{
    QRgb col=QColor::fromRgb(255,255,255).rgba();//原初白色
    for(int i=0;i<dataheight;i++)
    {
        bool inside=false;
        QPoint st,ft;//扫描线进入或淡出
        bool upst=false,downst=false,upft=false,downft=false;//上下色
        for(int j=0;j<datawidth;j++)
        {
            if(img.pixel(j,i)!=col)//是轮廓上的点
            {
                if(img.pixel(j-1,i)==col)//左边为空，说明进入
                {
                    upst=(img.pixel(j-1,i-1)!=col)|(img.pixel(j,i-1)!=col)|(img.pixel(j+1,i-1)!=col);
                    downst=(img.pixel(j-1,i+1))|(img.pixel(j,i+1)!=col)|(img.pixel(j+1,i+1)!=col);
                }
                if(img.pixel(j+1,i)==col)//右边为空，说明彻底进入
                {
                    upft=(img.pixel(j-1,i-1)!=col)|(img.pixel(j,i-1)!=col)|(img.pixel(j+1,i-1)!=col);
                    downft=(img.pixel(j-1,i+1))|(img.pixel(j,i+1)!=col)|(img.pixel(j+1,i+1)!=col);
//                    if(upst==upft&&downst==downft)//全相同，说明
//                    {
//                        //
//                    }
                    if(upst==false&&upft==false&&downst==true&&downft==true)//同向，说明为角点
                    {

                    }
                    else if(downst==false&&downft==false&&upst==true&&upft==true)//同向，说明为角点
                    {

                    }
                    else {
                        inside^=1;
                    }
                }

//                if(j<datawidth-1&&img.pixel(j+1,i)==col)inside^=1;//取反

            }
            else //说明是白色，需要看是否需要填充
            {
                if(inside==true)
                {
                    drawpix(j,i);
                }
            }
        }
    }
    fflush();
}
