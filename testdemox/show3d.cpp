#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>
#include <QPoint>
QColor imgcolor[1000][650];
void MainWindow::show3D()
{
    QRgb coltmp=color;
    color=QColor::fromRgb(112,53,67).rgba();
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            imgcolor[j][i]=imgtmp.pixel(j,i);
        }
    }
    if(data!=nullptr)delete []data;
    data=new unsigned char[lens];
    for(int i=0;i<lens;i++)data[i]=255;//全置为白色
    img=QImage(data,datawidth,dataheight,QImage::Format_RGB888);
    //纯色
    for(double z=0;z<15;z+=0.5)
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            if(imgcolor[j][i]==coltmp)//是有颜色
            {
                double a=j-500;
                double b=i-400;
                double c;
                double x,y;
//                a=a*cos(theta)+c*sin(theta);
//                c=-a*sin(theta)+c*cos(theta);
////                x=0.707*a-0.707*c;
////                y=-0.408*a+0.816*b-0.408*c;
//                x=a;
//                y=b;
//                lastpoint=QPoint(x+400,y+400);

                c=z;
                a=a*cos(theta)+c*sin(theta);
                c=-a*sin(theta)+c*cos(theta);
                x=0.707*a-0.707*c;
                y=-0.408*a+0.816*b-0.408*c;
//                x=a;
//                y=b;
//                curpoint=QPoint(x+400,y+400);
//                drawline(lastpoint,curpoint);
                drawpix(x+500,y+400);
//                qDebug()<<lastpoint<<curpoint<<j<<i<<endl;
            }
        }
    }
    fflush();
    color=coltmp;
}
