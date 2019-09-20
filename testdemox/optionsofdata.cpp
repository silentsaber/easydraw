#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include<QDebug>

//void MainWindow::newtest(QString aFileName)//处理8位图变为24位后保存
//{
//    QVector<QRgb>  colorTable;
//    for(int k=0;k<256;++k)
//    {
//           colorTable.push_back( qRgb(k,k,k) );
//    }


//    img=QImage(data,datawidth,dataheight,QImage::Format_Indexed8);

//    img.setColorTable(colorTable);
//    img.save(aFileName,"BMP",100);
//    QPixmap pix = QPixmap::fromImage(img);
//    pix.save(aFileName,"BMP",100);

//}
//void MainWindow::deldata()
//{
//    for(int i=0;i<datawidth;i++)delete[]data[i];
//    delete []data;
//    data=NULL;
//}
//void MainWindow::newdata()
//{
//    data=new unsigned char *[datawidth];
//    for(int i=0;i<datawidth;i++)data[i]=new unsigned char[dataheight];
//}
void MainWindow::getrealwidth()
{
    if(depth==8)realwidth=(datawidth*8+31)/32*4;
    if(depth==24)realwidth=(datawidth*24+31)/32*4;
    if(depth==32)realwidth=(datawidth*32+31)/32*4;
}
void MainWindow::uiset()
{
    ui->TopView->setChecked(false);
    ui->ObliqueView->setChecked(false);
    ui->FrontView->setChecked(false);

    //无绘制不可旋转
    ui->rotclockwise->setEnabled(false);
    ui->rotreverseclockwise->setEnabled(false);

    //不识别轮廓无法拟合
    ui->menu_4->setEnabled(false);
}
void MainWindow::init(bool flag)
{
    if(flag==false)
    {
        uiset();
        while(!preimg.isEmpty())preimg.pop();
        while(!reimg.isEmpty())reimg.pop();
        model=DEFAULT;//默认画笔
        color=QColor::fromRgb(0,0,0).rgba();//黑·笔
        showteapot=false;
        show3Drotate=false;
        frac=3;//默认三阶贝塞尔
        rotcnt=3;//旋转次数
        base=20;//默认低精度
    }
    mouse_press=false;
    reversecol=false;
    ctrpoint.clear();//清除
    archstate=0;//初始化

//    QVector<QRgb>  colorTable;
//    for(int k=0;k<256;++k)
//    {
//           colorTable.push_back( qRgb(k,k,k) );
//    }

//    color=0;
    //实际的图像数据并不是以width为真实宽度
    datawidth=990;
    dataheight=640;
//    depth=8;//初始位宽
    depth=24;//初始位宽

    getrealwidth();//获取真实宽度
    lens=realwidth*dataheight*3;//24位图出图
    if(data!=nullptr)delete []data;
    data=new unsigned char[lens];
    for(int i=0;i<lens;i++)data[i]=255;//全置为白色

    img=QImage(data,datawidth,dataheight,QImage::Format_RGB888);

//    img.setColorTable(colorTable);

//    img.setPixel(400,400,color);
    //qDebug()<<img.size()<<img.sizeInBytes()<<lens<<color<<endl;
//    for(int i=0;i<100;i++)img.setPixel(50,i,color);
    if(scene!=nullptr)delete scene;
    scene = new QGraphicsScene;

    scene->addPixmap(QPixmap::fromImage(img));

    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->show();
//    qDebug()<<"1"<<endl;

}
