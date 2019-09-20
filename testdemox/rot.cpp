#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>


void MainWindow::rot_clock(double x)
{
    theta+=x;
    if(theta>=2*PI)theta-=2*PI;
//    qDebug()<<theta<<cos(theta)<<endl;
}
void MainWindow::rot_reverse(double x)
{
    theta-=x;
    if(theta<0)theta+=2*PI;
//    qDebug()<<theta<<cos(theta)<<endl;
}
