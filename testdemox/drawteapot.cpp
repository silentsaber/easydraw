#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QtMath>
#include <QDebug>
#include <QMouseEvent>

int Factorial(int a)//阶乘函数
{
    int m=1;
    for(int i=1;i<=a;i++)
         m *= i;
    return m;

}
double Bernstein(const int &n, const int &i,double t)//Bernstein基函数
{
    return double(Factorial(n)/(Factorial(i)*Factorial(n-i))*pow(t,i)*pow(1-t,n-i));
}

double MainWindow::rotateZ(double a,double b,double c)
{
    return -a*sin(theta)+c*cos(theta);
}
double MainWindow::rotateX(double a,double b,double c)
{
    return a*cos(theta)+c*sin(theta);
}
double  MainWindow::touying(int Sign,double a,double b,double c,int flag)
{// project the three-dimensional graphics onto the plane
//    qDebug()<<"1:"<<a<<b<<c<<endl;
    double xa=rotateX(a,b,c);//对a,b,c投影变换
    double xc=rotateZ(a,b,c);
    a=xa;
    c=xc;
//    qDebug()<<"2:"<<a<<b<<c<<endl;
    double d;
   //Front view
    if(Sign==1)
    {
     if(1==flag){d=a;}
     if(2==flag){d=b;}
    }
   // Top view
    if(Sign==2)
    {
    if(1==flag){d=a;}
    if(2==flag){d=c;}
    }

    //Isometric view
    if(Sign==3)
    {
    if(1==flag){d=0.707*a-0.707*c;}
    if(2==flag){d=-0.408*a+0.816*b-0.408*c;}
    }
    if(Sign==2&&flag==2)return d-1;
    else return d;
}


void  MainWindow::teapotface(double poinx[],double poiny[],double poinz[])//Draw Bezier surface of teapot
{
double a,b,c,x,y,u,v;
double x0[100][100],y0[100][100];
int i,j;
for( u=0;u<=1.05;u=u+0.05)
    {
         for(v=0;v<=1.05;v=v+0.05)
         { a=0;b=0;c=0;x=0;y=0;
          for(i=0;i<=3;i++)
          {
            double m=0,n=0,l=0;
            for(j=0;j<=3;j++)
            {
                        m+=Bernstein(3,j,u)*poinx[j+4*i];
                        n+=Bernstein(3,j,u)*poiny[j+4*i];
                        l+=Bernstein(3,j,u)*poinz[j+4*i];
            }
                    a+=m*Bernstein(3,i,v);
                    b+=n*Bernstein(3,i,v);
                    c+=l*Bernstein(3,i,v);
                    x=touying(Sign,a,b,c,1);
                    y=touying(Sign,a,b,c,2);
          }
         lastpoint=curpoint;
         if(v==0)
         {
             lastpoint=QPoint(100*x+400,100*y+400);
         }
             curpoint=QPoint(100*x+400,100*y+400);
             drawline(lastpoint,curpoint);
         }

}
//Reverse Draw
for( u=0;u<=1.05;u=u+0.05)
    {
         for(v=0;v<=1.05;v=v+0.05)
         { a=0;b=0;c=0;x=0;y=0;
          for(j=0;j<=3;j++)
          {
            double m=0,n=0,l=0;
            for(i=0;i<=3;i++)
            {
                        m+=Bernstein(3,i,u)*poinx[j+4*i];
                        n+=Bernstein(3,i,u)*poiny[j+4*i];
                        l+=Bernstein(3,i,u)*poinz[j+4*i];
            }
                    a+=m*Bernstein(3,j,v);
                    b+=n*Bernstein(3,j,v);
                    c+=l*Bernstein(3,j,v);
                    x=touying(Sign,a,b,c,1);
                    y=touying(Sign,a,b,c,2);
          }
        lastpoint=curpoint;
         if(v==0)
         {
             lastpoint=QPoint(100*x+400,100*y+400);
         }
             curpoint=QPoint(100*x+400,100*y+400);
             drawline(lastpoint,curpoint);
         }



}

}

void MainWindow::teapot(double poinx[],double poiny[])//Bezier curve drawing
{
     double x,y,a,b,c,m,n,r,t;
     int h=20;
     for(t=0;t<=1.05;t+=1.0/h)
     {
        m=0;n=0;x=0;y=0;

        for(int i=0;i<=3;i++)
        {
            m+=poinx[i]*Bernstein(3,i,t);
            n+=poiny[i]*Bernstein(3,i,t);
        }
        for(int j=0;j<=20;j++)
        {
          r=3.1415926*2*j/20;
          a=(m-0.0)*cos(r);    //The curve of the lid is rotated in space, derived surfaces;
          b=n;
          c=(m-0.0)*sin(r);
          x=touying(Sign,a,b,c,1);
          y=touying(Sign,a,b,c,2);
          lastpoint=curpoint;
          if(j==0)
          {
              lastpoint=QPoint(100*x+400,100*y+400);
          }
          curpoint=QPoint(100*x+400,100*y+400);
          drawline(lastpoint,curpoint);
        }

     }
    for(int j=0;j<=20;j++)
    {
     for( t=0;t<=1.05;t+=1.0/h)
     {
        m=0;n=0;x=0;y=0;

        for(int i=0;i<=3;i++)
        {
            m+=poinx[i]*Bernstein(3,i,t);
            n+=poiny[i]*Bernstein(3,i,t);
        }

          r=3.1415926*2*j/20;
          a=(m-0.0)*cos(r);    //The curve of the lid is rotated in space, derived surfaces;
          b=n;
          c=(m-0.0)*sin(r);
          x=touying(Sign,a,b,c,1);
          y=touying(Sign,a,b,c,2);
          lastpoint=curpoint;
          if(t==0)
          {
               lastpoint=QPoint(100*x+400,100*y+400);
          }
          curpoint=QPoint(100*x+400,100*y+400);
          drawline(lastpoint,curpoint);
     }

    }
}



void MainWindow::play()
{
    init(true);//第一步，清空画布，但不重置按钮

    double spoutx[16]={1.7,3.1,2.4,3.3,1.7,3.1,2.4,3.3,1.7,2.6,2.3,2.7,1.7,2.6,2.3,2.7};
    double spouty[16]={-0.4,-0.675,-1.875,-2.2,-0.4,-0.675,-1.875,-2.2,-1.27,-1.275,-1.95,-2.25,-1.27,-1.275,-1.95,-2.25};
    double spoutz[16]={0,0,0,0,0.66,0.66,0.25,0.25,0.66,0.66,0.25,0.25,0,0,0,0};
    double spoutzf[16]={0,0,0,0,-0.66,-0.66,-0.25,-0.25,-0.66,-0.66,-0.25,-0.25,0,0,0,0};
    double spoutxl[16]={2.7,2.8,2.9,2.8,2.7,2.8,2.9,2.8,3.3,3.525,3.4,3.2,3.3,3.525,3.4,3.2};
    double spoutyl[16]={-2.25,-2.325,-2.325,-2.25,-2.25,-2.325,-2.325,-2.25,-2.2,-2.34375,-2.3625,-2.25,-2.2,-2.34375,-2.3625,-2.25};
    double spoutzl[16]={0,0,0,0,0.25,0.25,0.15,0.15,0.25,0.25,0.15,0.15,0,0,0,0};
    double spoutzlf[16]={0,0,0,0,-0.25,-0.25,-0.15,-0.15,-0.25,-0.25,-0.15,-0.15,0,0,0,0};
    teapotface(spoutx,spouty,spoutz);
    teapotface(spoutx,spouty,spoutzf);
    teapotface(spoutxl,spoutyl,spoutzl);
    teapotface(spoutxl,spoutyl,spoutzlf);
    //Painting lid
    double lidx[7]={0.0,0.8,0.0,0.2,0.4,1.3,1.3};
    double lidy[7]={-3.0,-3.0,-2.7,-2.55,-2.4,-2.4,-2.25};
    teapot(lidx, lidy);
    teapot(&lidx[3], &lidy[3]);
    //painting pot's Body
    double bodyx[10]={1.4,1.3375,1.4375,1.5,1.75,2.0,2.0,2.0,1.5,1.5};
    double bodyy[10]={-2.25,-2.38125,-2.38125,-2.25,-1.725,-1.2,-0.75,-0.3,-0.075,-0.0};
    teapot(bodyx, bodyy);
    teapot(&bodyx[3], &bodyy[3]);
    teapot(&bodyx[6], &bodyy[6]);
    //painting pot's Bottom
    double bottomx[4]={1.5,1.0,0.5,0.0};
    double bottomy[4]={0,0,0,0};
    teapot(bottomx, bottomy);
    teapot(bottomx, bottomy);
    //painting pot's Handle
    double handlex[16]={-1.6,-2.3,-2.7,-2.7,-1.6,-2.3,-2.7,-2.7,-1.5,-2.5,-3.0,-3.0,-1.5,-2.5,-3.0,-3.0};
    double handley[16]={-1.875,-1.875,-1.875,-1.65,-1.875,-1.875,-1.875,-1.65,-2.1,-2.1,-2.1,-1.65,-2.1,-2.1,-2.1,-1.65};
    double handlez[16]={0,0,0,0,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0,0,0,0};
    double handlezf[16]={0,0,0,0,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,-0.3,0,0,0,0};
    double handlexl[16]={-2.7,-2.7,-2.5,-2.0,-2.7,-2.7,-2.5,-2.0,-3.0,-3.0,-2.65,-1.90,-3.0,-3.0,-2.65,-1.90};
    double handleyl[16]={-1.65,-1.425,-0.975,-0.75,-1.65,-1.425,-0.975,-0.75,-1.65,-1.2,-0.7875,-0.45,-1.65,-1.2,-0.7875,-0.45};
    teapotface(handlex,handley,handlez);
    teapotface(handlex,handley,handlezf);
    teapotface(handlexl,handleyl,handlez);
    teapotface(handlexl,handleyl,handlezf);
    fflush();
}


void MainWindow::OnIso()
{
    Sign=3;
    play();

}

void MainWindow::OnTop()
{
    Sign=2;
    play();

}

void MainWindow::OnFront()
{
    Sign=1;
    play();

}


void MainWindow::DRAWTEAPOT()
{
    Sign=1;
    play();

}

