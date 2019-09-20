#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QColorDialog>
#include <QInputDialog>
#include <QApplication>
#include <QTime>
#include <conio.h>
/*
 *我思考了一下，还是决定直接对图像上的像素点进行操作
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);//禁止最大化
    setFixedSize(this->width(), this->height());//禁止拖动
    ui->graphicsView->viewport()->installEventFilter(this);//注册事件处理
    ui->graphicsView->viewport()->setMouseTracking(true);
    data=nullptr;
    scene=nullptr;
    fontweight=false;
    theta=0;//不旋转
    init();//初始化
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_0_clicked()
{
    model=DEFAULT;//画笔
}

void MainWindow::on_toolButton_1_clicked()
{
    model=DRAWLINE;//直线
}

void MainWindow::on_toolButton_2_clicked()
{
    model=DRAWARCH;//曲线
}


void MainWindow::on_open_triggered()//打开图片
{
        QString curPath=QDir::currentPath();//获取系统当前目录
        //获取应用程序的路径
        QString dlgTitle="选择一个文件"; //对话框标题
        QString filter="图片文件(*.jpg *.gif *.png *.bmp)"; //文件过滤器
        QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
        if (!aFileName.isEmpty())
        {
                img.load (aFileName);
//                if(img.width()>ui->graphicsView->width()-10||img.height()>ui->graphicsView->height()-10)
                img=img.scaled(ui->graphicsView->width()-10, ui->graphicsView->height()-10);
                datawidth=img.width();
                dataheight=img.height();

                depth=img.depth();//获取位宽
                lens=img.sizeInBytes();
                getrealwidth();//获取真实宽度

                if(depth==8)//8位图转24位图
                {
                    int tmpwidth=realwidth;//tmp
                    depth=24;
                    getrealwidth();
                    lens=realwidth*dataheight;//总长度
                    if(data!=nullptr)delete []data;
                    data=new unsigned char[lens];
                    udata=img.bits();
                    //qDebug()<<realwidth<<dataheight<<endl;
                    //qDebug()<<lens<<endl;
                    //qDebug()<<depth<<endl;
                    //qDebug()<<img.size()<<endl;
                    //qDebug()<<img.sizeInBytes()<<endl;
                    for(int i=0;i<dataheight;i++)
                    {
                        for(int j=0;j<datawidth;j++)
                        {
                            data[i*realwidth+3*j]=*(udata+i*tmpwidth+j);
                            data[1+i*realwidth+3*j]=*(udata+i*tmpwidth+j);
                            data[2+i*realwidth+3*j]=*(udata+i*tmpwidth+j);
                        }
                    }
                    img=QImage(data,datawidth,dataheight,QImage::Format_RGB888);
                }
//                lens=datawidth*dataheight*depth/8;//总bit数



//                if(data!=nullptr)delete[]data;
//                data=new unsigned char[lens];


//                qDebug()<<img.depth();
//                qDebug()<<qRed(img.pixel(0,0))<<qGreen(img.pixel(0,0))<<qBlue(img.pixel(0,0))<<endl;
//                for(int i=0;i<lens;i++)if(data[i]!=255)qDebug()<<i;


                if(scene!=nullptr)delete scene;
                scene = new QGraphicsScene;
                scene->addPixmap(QPixmap::fromImage(img));
                ui->graphicsView->setScene(scene);
                ui->graphicsView->resize(ui->graphicsView->width(), ui->graphicsView->height());
                ui->graphicsView->show();


        }
}

void MainWindow::on_newone_triggered()
{
    init();
}


void MainWindow::on_exit_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_resave_triggered()
{
    QString curPath=QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString dlgTitle="保存文件"; //对话框标题
    QString filter="图片文件(*.jpg);;图片文件(*.bmp)"; //文件过滤器
    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (!aFileName.isEmpty())
    {
//        if(depth==8)
//        {
//            newtest(aFileName);
//            return ;
//        }

//        else if(depth==24)img=QImage(data,datawidth,dataheight,QImage::Format_RGB888);
//        else if(depth==32)img=QImage(data,datawidth,dataheight,QImage::Format_RGB32);
        img.save(aFileName,"JPG",100);
    }
}



void MainWindow::on_toolButton_3_clicked()
{
    QColorDialog col;//调出颜色选择器对话框
    QColor c = col.getColor();
    color=qRgb(c.red(),c.green(),c.blue());
}

void MainWindow::on_toolButton_4_clicked()
{
    model=DRAWPAINT;//喷桶
}

void MainWindow::on_undo_triggered()//撤销
{
    archstate=0;
    if(preimg.isEmpty())return ;
    reimg.push(img.copy());//保存撤销前的状态
    img=preimg.pop().copy();

    fflush();
}

void MainWindow::on_toolButton_clicked()
{
    //不需要模式，直接全局一键填充扫描
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    DRAWSCAN();//一键扫描
}

void MainWindow::on_redo_triggered()//重做
{
    archstate=0;
    if(reimg.isEmpty())return ;
    preimg.push(img.copy());
    img=reimg.pop().copy();

    fflush();
}

void MainWindow::on_graygraph_triggered()//灰度图
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            QRgb col=img.pixel(j,i);
            int c=qRed(col)+qBlue(col)+qGreen(col);
            c/=3;
            col=QColor::fromRgb(c,c,c).rgba();
            img.setPixel(j,i,col);
        }
    }
    fflush();//刷新
}

void MainWindow::on_twocolor_triggered()//二值化
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            QRgb col=img.pixel(j,i);
            int c=qRed(col)+qBlue(col)+qGreen(col);
            c/=3;
            if(c>127)col=QColor::fromRgb(255,255,255).rgba();
            else col=QColor::fromRgb(0,0,0).rgba();
            img.setPixel(j,i,col);
        }
    }
    fflush();//刷新
}

void MainWindow::on_reversecol_triggered()//反色+二值化
{
    reversecol=true;
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            QRgb col=img.pixel(j,i);
            int c=qRed(col)+qBlue(col)+qGreen(col);
            c/=3;
            if(c<=127)col=QColor::fromRgb(255,255,255).rgba();
            else col=QColor::fromRgb(0,0,0).rgba();
            img.setPixel(j,i,col);
        }
    }
    fflush();//刷新
}

bool MainWindow::checkoutline(int j,int i)//true:在内部  false在外部
{
    if(j>0&&j<datawidth-1&&i>0&&i<dataheight-1)
    {
//         QRgb col=(reversecol)?QColor::fromRgb(255,255,255).rgba():QColor::fromRgb(0,0,0).rgba();//选择颜色
        QRgb col=QColor::fromRgb(0,0,0).rgba();//检测黑色
        if(imgtmp.pixel(j,i)==col&&imgtmp.pixel(j-1,i-1)==col&&imgtmp.pixel(j-1,i)==col&&imgtmp.pixel(j-1,i+1)==col&&imgtmp.pixel(j,i-1)==col&&imgtmp.pixel(j,i+1)==col&&imgtmp.pixel(j+1,i-1)==col&&imgtmp.pixel(j+1,i)==col&&imgtmp.pixel(j+1,i+1)==col)
        {
            return true;
        }
        else return false;
    }
    return false;
}
void MainWindow::on_getoutline_triggered()//识别轮廓
{
    imgtmp=img.copy();
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            if(checkoutline(j,i)==true)//在内部
            {
                img.setPixel(j,i,QColor::fromRgb(255,255,255).rgba());//设为白色
            }
        }
    }
    fflush();//刷新
    ui->menu_4->setEnabled(true);
}

void MainWindow::on_frac_triggered()//设置贝赛尔曲线阶数
{
    bool ok;
    int value1 = QInputDialog::getInt(this,QObject::tr("设置阶数"),
                                         QObject::tr("请设置贝塞尔曲线的阶数，范围为2~20"),frac,2,20,1,&ok);
       if(ok)
       {

           frac=value1;
          // qDebug()<<frac<<endl;
       }

}

void MainWindow::on_fontweight_triggered()//设置线条宽度
{
    fontweight^=1;
}

//void MainWindow::on_lineoutline_triggered()//直线拟合轮廓
//{
//    lineoutline();//直线拟合轮廓
//}

void MainWindow::on_loweps_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    base=20;
    lineoutline();
}

void MainWindow::on_mideps_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    base=10;
    lineoutline();
}

void MainWindow::on_higheps_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    base=4;
    lineoutline();
}

void MainWindow::on_toolButton_5_clicked()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    maxlevel=20;
    base=4;
//    for(int i=0;i<=4;i++)
        lineoutline(true);//真实改变&&贝赛尔曲线勾勒
}

void MainWindow::on_rotclockwise_triggered()//顺时针旋转
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    //顺时针旋转
    rot_clock();
    if(showteapot==true)play();
    else show3D();
}

void MainWindow::on_rotreverseclockwise_triggered()//逆时针旋转
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    //逆时针旋转
    rot_reverse();
    if(showteapot==true)play();
    else show3D();
}

void MainWindow::on_teapot_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    DRAWTEAPOT();
}

void MainWindow::on_FrontView_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    showteapot=true;//旋转茶壶
    OnFront();
    ui->TopView->setChecked(false);
    ui->ObliqueView->setChecked(false);
    ui->rotclockwise->setEnabled(true);
    ui->rotreverseclockwise->setEnabled(true);
}

void MainWindow::on_TopView_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    showteapot=true;
    OnTop();
    ui->FrontView->setChecked(false);
    ui->ObliqueView->setChecked(false);
    ui->rotclockwise->setEnabled(true);
    ui->rotreverseclockwise->setEnabled(true);
}

void MainWindow::on_ObliqueView_triggered()
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    showteapot=true;
    OnIso();
    ui->TopView->setChecked(false);
    ui->FrontView->setChecked(false);
    ui->rotclockwise->setEnabled(true);
    ui->rotreverseclockwise->setEnabled(true);
}

void MainWindow::on_announce_triggered()
{
    QString dlgTitle="声明";
    QString strInfo="author:nikelong \nemail:805898882@qq.com \nCopyright © 2019- All Rights Reserved";
    QMessageBox::about(this, dlgTitle, strInfo);
}

void MainWindow::on_action3D_triggered()//针对提取轮廓的物体三维显示
{
    if(preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
    imgtmp=img.copy();//暂存图像，对此图像进行各种处理
    show3D();
    ui->rotclockwise->setEnabled(true);
    ui->rotreverseclockwise->setEnabled(true);
}

void sleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
void MainWindow::on_actionrotate_triggered()
{
        int cnt=0;
        while(1)
        {
            rot_clock(PI/24);
//            show3D();
            if(showteapot==true)play();
            else show3D();
            sleep(10);
            cnt++;
            if(cnt==rotcnt*48)break;
        }

}

void MainWindow::on_rotatecnt_triggered()//设置旋转次数
{
    bool ok;
    int value1 = QInputDialog::getInt(this,QObject::tr("设置旋转次数"),
                                         QObject::tr("请设置三维旋转的次数，范围为1~999"),rotcnt,1,999,1,&ok);
       if(ok)
       {

           rotcnt=value1;
           //qDebug()<<rotcnt<<endl;
       }
}
