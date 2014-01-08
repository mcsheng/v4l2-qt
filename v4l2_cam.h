/*************************************************************************
	> File Name: v4l2_cam.h
	> Author: mcsheng
	> Created Time: 2013年07月16日 星期二 22时15分12秒
 ************************************************************************/
#ifndef _V4L2_CAM_H
#define _V4L2_CAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h> 

#include <fcntl.h> 
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h> 
#include <linux/videodev2.h>
/*定义帧缓冲类型*/
struct buffer {
        void * start;
        size_t length;
};

/*v4l2的初始化*/
/*成功则返回0,失败则返回-1*/
/*v4l2_buffers为二级指针*/
int v4l2_init(int *fd,struct buffer **v4l2_buffers,char **format,int width,int heith);

/*关闭摄像头，释放资源*/
void v4l2_close(int *fd,int n_buffers,struct buffer *v4l2_buffers,char **format);

/*从摄像头中读取一帧数据*/
void v4l2_read_frame(int *fd,struct buffer *v4l2_buffers,unsigned char **v4l2_data);

/*取像并将摄像头读出来的yuyv422格式数据转换为yuv420p*/
void rf_yuyv422_yuv420p(int *fd ,struct buffer *v4l2_buffers,unsigned char *yuv420p_data,int width,int height);
#endif
