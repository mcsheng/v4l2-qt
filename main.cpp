/*************************************************************************
	> File Name: main.cpp
	> Author: mcsheng
	> Created Time: 2014年01月07日 星期二 12时20分45秒
 ************************************************************************/
#include<QApplication>
#include"V4l2Cam.h"
#include<QTextCodec>
int main(int argc,char *argv[ ])
{
	QApplication app(argc,argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	V4l2Cam cam;
	cam.show( );
	return app.exec( );
}
