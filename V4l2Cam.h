/*************************************************************************
	> File Name: V4l2Cam.h
	> Author: mcsheng
	> Created Time: 2014年01月07日 星期二 12时22分39秒
 ************************************************************************/
#ifndef V4L2CAM_H_
#define V4L2CAM_H_
#include<iostream>
#include<QWidget>
#include<QLabel>
#include<v4l2_cam.h>
#include<QVBoxLayout>
#include<QTimer>
#include<QPainter>
#include<QLineEdit>
#include<QPushButton>
#include<QThread>
#include<QMutex>
#include<QReadWriteLock>
#include<QString>
#include<QDebug>
class Test:public QObject
{
	Q_OBJECT
public:
	Test( );
	~Test( );
public:
	QTimer *timer;
	int fd;
	struct buffer *v4l2_buffers;
	int width;
	int height;
	int n_buffers;
	unsigned char *v4l2_data;
	unsigned char *v_data;
	char *format;
	unsigned char *rgb;
	int formatInt;
private:
	QMutex mutex;
private slots:
	void testTime(void);
	void testThread(void);
	void switchVideo(const QString &);
signals:
	void oneFrame(void);
};
class V4l2Cam:public QWidget
{
	Q_OBJECT
public:
	V4l2Cam(QWidget *parents = 0);
	~V4l2Cam( );
private:
	QLabel      *camLabel;
	QImage      *img;
	QTimer      *timer;
	QLabel      *videoLabel;
	QLineEdit   *videoEdit;
	QPushButton *videoButton;
	QThread     *thread;
	Test        *test;
	unsigned char *previousFrame;
	unsigned char *differFrame;
	unsigned char *tmpFrame;
	int frameNum;
	int yuyv422_to_rgb888_buffer(unsigned char *yuyv422, unsigned char *rgb, unsigned int width, unsigned int height);
	int yuyv422_to_rgb_pixel(int y, int u, int v);
	//翻转函数 rgb翻转
	bool HFlip(unsigned char* rgb, int width, int height);
	bool Gray(unsigned char* rgb, int width, int height);
	bool Binarization(unsigned char* rgb, int width, int height,int value);
	bool Substruction(unsigned char* previousFrame,unsigned char *currentFrame,unsigned char *differFrame, int width, int height);
private slots:
	void cam_show(void);
	void paintEvent(QPaintEvent *);
	void switchVideo(void);
signals:
	void switchSignal(const QString &);
};

#endif
