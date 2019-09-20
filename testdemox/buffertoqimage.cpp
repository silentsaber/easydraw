/*
* Copyright (c) 2013,中科院苏州医工所
* All rights reserved.
*
* 文件名称：BufferToQImage.h
* 文件标识：见配置管理计划书
* 摘    要：将内存数据封装成QImage。
*
* 当前版本：1.0
* 作    者：LYC
* 完成日期：2013年6月27日
*/

#include "BufferToQImage.h"

BufferToQImage::BufferToQImage(void)
{
    for (int i = 0; i < 256; i++)
    {
        vcolorTable.append(qRgb(i, i, i));
    }
}

BufferToQImage::~BufferToQImage(void)
{

}

/*
* 函数名：Pk8bitGrayToQIm()
* 功能：将8bit灰度数据封装成QImage
* 参数：
*	pBuffer - 内存数据指针
*	bufWidth - 内存数据宽度
*	bufHight - 内存数据高度
* 返回值：QImage
* 作者：LYC
* 时间：2013.6.27
*/
QImage BufferToQImage::Pk8bitGrayToQIm(const BYTE *pBuffer, const int &bufWidth, const int &bufHight)
{
    //对参数的有效性进行检查
    assert((pBuffer != NULL) && (bufWidth>0) && (bufHight>0));

    int biBitCount = 8; //灰度图像像素bit数
    int lineByte = (bufWidth * biBitCount/8 + 3) / 4 * 4; //bmp行byte数（格式宽度，为4的倍数）

    if (bufWidth == lineByte) //判断图像宽度与格式宽度
    {
        QImage qIm = QImage(pBuffer, bufWidth, bufHight, QImage::Format_Indexed8);  //封装QImage
        qIm.setColorTable(vcolorTable); //设置颜色表

        return qIm;
    }
    else
    {
        BYTE *qImageBuffer = new BYTE[lineByte * bufHight]; //分配内存空间
        uchar *QImagePtr = qImageBuffer;

        for (int i = 0; i < bufHight; i++) //Copy line by line
        {
            memcpy(QImagePtr, pBuffer, bufWidth);
            QImagePtr += lineByte;
            pBuffer += bufWidth;
        }

        QImage qImage = QImage(qImageBuffer, bufWidth, bufHight, QImage::Format_Indexed8);  //封装QImage
        qImage.setColorTable(vcolorTable); //设置颜色表

        return qImage;
    }
}
