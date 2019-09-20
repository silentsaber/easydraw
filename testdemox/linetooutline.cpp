#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>

bool innerdis_points(QPoint a,QPoint b)//判断两个点之间的距离是否小于某一值
{
    int tmp=abs(a.x()-b.x())+abs(a.y()-b.y());//两点之间的距离
    if(tmp<=50)return true;
    return false;
}
void MainWindow::linetogetline(int x,int y,QRgb col,bool ok,int cnt)
{
    vis[x][y]=1;//访问标记
    if(ctrpoint.size()==base)//恰好为base时，开始高阶贝塞尔递归
    {
        QPoint tmpp=ctrpoint[base-1];
        if(!ok)for(int i=base-1;i>0;i--)
        {
            if(innerdis_points(ctrpoint[0],ctrpoint[i])==true)
            {
    //            maxlevel=2;//最高阶
//                drawhighbezier(ctrpoint.size(),ctrpoint,1);//高阶贝塞尔拟合
                tmpp=ctrpoint[i];
                drawline(ctrpoint[0],ctrpoint[i]);
                break;
            }
        }
        else drawhighbezier(ctrpoint.size(),ctrpoint,1);//高阶贝塞尔拟合

        ctrpoint.clear();
        ctrpoint.push_back(tmpp);
    }
//    bool ok=false;
    for(int i=0;i<8;i++)
    {
        int nx=x+dx[i];
        int ny=y+dy[i];
        if(nx>=0&&nx<datawidth&&ny>=0&&ny<dataheight&&!vis[nx][ny]&&imgtmp.pixel(nx,ny)==col)
        {
            if(!ok)
            {
                ctrpoint.push_back(QPoint(nx,ny));
                linetogetline(nx,ny,col,ok);
            }
            else {
//                if(cnt<5)linetogetline(nx,ny,col,ok,cnt+1);
//                else
//                {
                    ctrpoint.push_back(QPoint(nx,ny));
                    linetogetline(nx,ny,col,ok,0);
//                }
            }
            break;
        }
    }
}
void MainWindow::lineoutline(bool ok)//必须是二值化的图，且仅对于提取轮廓后的图处理
{
//    ctrpoint.clear();


    imgtmp=img.copy();//保存img

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

    memset(vis,0,sizeof(vis));
    QRgb col=QColor::fromRgb(0,0,0).rgba();//黑色

    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
//            qDebug()<<j<<i<<endl;
//            qDebug()<<img.pixel(j,i)<<endl;
            if(imgtmp.pixel(j,i)==col&&vis[j][i]==0)//black&&not visited
            {

                ctrpoint.clear();
                ctrpoint.push_back(QPoint(j,i));
                linetogetline(j,i,col,ok);//拓展新的图像&&连通图

                if(!ctrpoint.isEmpty())
                {
                     QPoint tmpp=ctrpoint[ctrpoint.size()-1];
//                    maxlevel=2;
//                    drawhighbezier(ctrpoint.size(),ctrpoint,1);
                    if(!ok)for(int k=ctrpoint.size()-1;k>0;k--)
                    {
                        if(innerdis_points(ctrpoint[0],ctrpoint[k])==true)
                        {
                            drawline(ctrpoint[0],ctrpoint[k]);
                            tmpp=ctrpoint[k];
                            break;
                        }
                    }
                    else  drawhighbezier(ctrpoint.size(),ctrpoint,1);
                    ctrpoint.clear();
                    ctrpoint.push_back(tmpp);
                }

            }
        }
    }

    fflush();//刷新

}
