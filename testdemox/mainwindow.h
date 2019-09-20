#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QEvent>
#include <QStack>
#include <QLabel>


namespace Ui {
#define DEFAULT -1//画笔
#define DRAWLINE 0
#define DRAWARCH 1//曲线，贝塞尔曲线拟合
#define DRAWPAINT 2//喷桶
class MainWindow;
}
const double PI=3.1415926535;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    unsigned char *data;//数组img，一个动态空间
    unsigned char *udata;//图像的指针
    int depth;//图片位宽
    int lens;//图片总长
    int datawidth;
    int realwidth;//实际数据对应的宽度
    int dataheight;
    int model;//绘制模式
    bool mouse_press;
    bool reversecol;//是否反色 0：白底  1：黑底
    bool fontweight;//字体宽度
    QImage img;//图片img
    QImage simg;//抗锯齿图片
    QImage imgtmp;//图片暂存
    QStack<QImage>preimg;//上一次图片，用于撤回
    QStack<QImage>reimg;//用于重做
    QStack<QPoint>unpaint;//区域填充（种子填充算法）所用的栈

    QGraphicsScene *scene;//绘制场景
    QRgb color;//当前颜色，初始化为黑色
    QPoint lastpoint;//上一次的点
    QPoint curpoint;//当前点
    QVector<QPoint>ctrpoint;//贝塞尔曲线控制点

    const int dx[8]={-1,-1,-1,0,0,1,1,1};
    const int dy[8]={-1,0,1,-1,1,-1,0,1};
    bool vis[1000][650];//最开始
    int base;//基准值，与精度有关
    //8个方向拓展

    int maxlevel;//贝赛尔曲线递归层数
//    QPoint ctrpoint1;//贝塞尔曲线控制点1
//    QPoint ctrpoint2;//贝塞尔曲线控制点2
    int archstate;//用于记录贝赛尔曲线设置的状态
    int frac;//用来记录贝塞尔函数的阶数


    double Sign;
    double theta;
    bool showteapot;//旋转茶壶标记
    bool show3Drotate;//是否旋转
    int rotcnt;//旋转次数

    void newtest(QString aFileName);
    void getrealwidth();
    void init(bool flag=false);//初始化，和新建一个效果
    void uiset();//初始化按钮是否可用

protected:
    void fflush();//刷新img
    void drawpix(int x,int y,bool flag=false);//在(x,y)处画一个点，颜色为color
    void drawline(QPoint,QPoint);
    void bigpoint(QPoint &p);//画一个大点
    void showarch();//显示3阶贝赛尔曲线和控制点、线
    void drawbezier(QPoint ,QPoint ,QPoint ,QPoint );//绘制3阶贝赛尔曲线
    void drawhighbezier(int cnt,QVector<QPoint>ps,int level);//绘制高阶贝赛尔曲线 cnt多少个点 cnt-1阶贝赛尔曲线
    void lineoutline(bool ok=false);//实现直线拟合轮廓
    void linetogetline(int,int,QRgb,bool ok=false,int cnt=0);//递归连通图访问实现轮廓拟合 从0开始计数
    void rot_clock(double x=PI/12);//顺时针旋转
    void rot_reverse(double x=PI/12);//逆时针旋转
    void DEFAULT_PRESS(QMouseEvent *event);
    void DEFAULT_MOVE(QMouseEvent *event);
    void DEFAULT_UP(QMouseEvent *event);
    void DRAWLINE_PRESS(QMouseEvent *event);
    void DRAWLINE_MOVE(QMouseEvent *event);
    void DRAWLINE_UP(QMouseEvent *event);
    void DRAWARCH_PRESS(QMouseEvent *event);
    void DRAWARCH_MOVE(QMouseEvent *event);
    void DRAWARCH_UP(QMouseEvent *event);
    void DRAWPAINT_PRESS(QMouseEvent *event);
    void DRAWSCAN();//扫描线
    void DRAWTEAPOT();//绘制茶壶
    void OnIso();
    void OnTop();
    void OnFront();
    void play();
    void teapot(double poinx[],double poiny[]);
    void teapotface(double poinx[],double poiny[],double poinz[]);
    double touying(int Sign,double a,double b,double c,int flag);
    double rotateX(double a,double b,double c);
    double rotateZ(double a,double b,double c);
    void show3D();
//    void Bernstein(const int &n, const int &i,double t)//Bernstein基函数;
    bool checkoutline(int j,int i);//检查是否在内部
    bool innerdis_point(QPoint a,QPoint b);
public slots:
    bool eventFilter(QObject *obj,QEvent *event);
private slots:

    void on_toolButton_2_clicked();

    void on_toolButton_1_clicked();

    void on_open_triggered();

    void on_newone_triggered();

    void on_exit_triggered();

    void on_resave_triggered();

    void on_toolButton_0_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_undo_triggered();

    void on_toolButton_clicked();

    void on_redo_triggered();

    void on_graygraph_triggered();

    void on_twocolor_triggered();

    void on_reversecol_triggered();

    void on_getoutline_triggered();


    void on_frac_triggered();

    void on_fontweight_triggered();

//    void on_lineoutline_triggered();

    void on_loweps_triggered();

    void on_mideps_triggered();

    void on_higheps_triggered();

    void on_toolButton_5_clicked();

    void on_rotclockwise_triggered();

    void on_rotreverseclockwise_triggered();

    void on_teapot_triggered();

    void on_FrontView_triggered();

    void on_TopView_triggered();

    void on_ObliqueView_triggered();

    void on_announce_triggered();

    void on_action3D_triggered();

    void on_actionrotate_triggered();

    void on_rotatecnt_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
