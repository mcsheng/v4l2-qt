/*************************************************************************
	> File Name: v4l2_cam.c
	> Author: mcsheng
	> Created Time: 2013年07月16日 星期二 21时01分22秒
 ************************************************************************/

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

#include "v4l2_cam.h"

/*初始化摄像头*/
/* fd 为设备文件描述符，buffers为帧缓冲,width为设置宽度，height为设置高度*/
int v4l2_init(int *fd,struct buffer **v4l2_buffers,char **format,int width,int height)
{
	/*用于存储摄像头的各项参数*/
    struct v4l2_capability cap;
	/*保存要设置摄像头的格式*/
    struct v4l2_format fmt;
	/*保存摄像头所支持的摄像格式*/
	struct v4l2_fmtdesc fmtdesc;
    unsigned int i;
    enum v4l2_buf_type type;
	/*用于向驱动申请帧缓冲*/
    struct v4l2_requestbuffers req;
	int ret = 0;
	int n_buffers = 0;
	/*打开视频设备文件*/
	//*fd = open ("/dev/video0", O_RDWR /* required */);
	*fd = open ("/dev/video0", O_RDWR /* required */);
	if((*fd < 0))
	{
		perror("can't open the /dev/video0");
		exit(-1);
	}
	/*获取摄像头的各项参数*/
	ret = ioctl(*fd, VIDIOC_QUERYCAP,&cap);
	if(ret < 0)
	{
		perror("VIDIOC_QUERYCAP:can't get video capability");
		exit(-1);
	}

	memset(&fmtdesc,0,sizeof(fmtdesc));
	fmtdesc.index = 0;
	fmtdesc.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	char fmt_data[100];
    /*获取当前驱动支持的视频格式*/
	while ((ret = ioctl(*fd, VIDIOC_ENUM_FMT, &fmtdesc)) == 0)
	{
		  printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",fmtdesc.pixelformat & 0xFF, (fmtdesc.pixelformat >> 8) & 0xFF,(fmtdesc.pixelformat >> 16) & 0xFF, (fmtdesc.pixelformat >> 24) & 0xFF,fmtdesc.description);
		  fmt_data[0+fmtdesc.index*5] = fmtdesc.pixelformat & 0xFF;
		  fmt_data[1+fmtdesc.index*5] = (fmtdesc.pixelformat >> 8)  & 0xFF;
		  fmt_data[2+fmtdesc.index*5] = (fmtdesc.pixelformat >> 16) & 0xFF;
		  fmt_data[3+fmtdesc.index*5] = (fmtdesc.pixelformat >> 24) & 0xFF;
		  fmt_data[4+fmtdesc.index*5] = '\0';
		  fmtdesc.index++;
	}
    /*将摄像头支持的格式保存取出*/
	(*format) = (char *)malloc(fmtdesc.index * 5 * sizeof(char));
	memcpy((char*)*format,fmt_data,fmtdesc.index*5*sizeof(char));

	/*清空fmt中各项参数，为重新设置做准备*/
	memset(&fmt,0,sizeof(fmt));
	/*数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE*/
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	/*宽，必须是16的倍数*/
    fmt.fmt.pix.width = width;
    //fmt.fmt.pix.width = 640;
	/*高，必须是16的倍数*/
    //fmt.fmt.pix.height = 480;
    fmt.fmt.pix.height = height;
	/*设置要捕获的格式，此要和摄像头一致*/
	/*视频数据存储类型//V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YVU420;//V4L2_PIX_FMT_YUYV*/
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    /*设置当前驱动的频捕获格式*/
    ret = ioctl (*fd, VIDIOC_S_FMT, &fmt); 
    if(ret < 0)
	{
        perror("failture VIDIOC_S_FMT");
		exit(-1);
	}

	/*清零*/
	memset(&req,0,sizeof(req));
	/*缓存数量，也就是说在缓存队列里保持多少张照片*/
	/*1代表只申请一个，一般不超过5个*/
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	/*映射方式*/
    req.memory = V4L2_MEMORY_MMAP;//或V4L2_MEMORY_USERPTR
	/*向驱动申请帧缓冲*/
    ret = ioctl (*fd, VIDIOC_REQBUFS, &req); 
	if(ret < 0)
	{
		perror("failture VIDIOC_REQBUFS");
		exit(-1);
	}
	/*若的数量少于1,也退出*/
    if (req.count < 1)
	{
        perror("Insufficient buffer memory");
		exit(-1);
	}

	/*内存中建立对应空间*/
    (*v4l2_buffers) = (struct buffer*)calloc (req.count, sizeof (*v4l2_buffers));

    for (n_buffers = 0; n_buffers < req.count; n_buffers++)
    { 
		/*驱动中的一帧*/
        struct v4l2_buffer buf; 
        memset(&buf,0,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;
		/*把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址*/
		ret = ioctl (*fd, VIDIOC_QUERYBUF, &buf);
		if (ret < 0)
		{
			perror ("VIDIOC_QUERYBUF error");
			exit(-1);
		}
		(*v4l2_buffers)[n_buffers].length = buf.length;
		/*通过mmap建立映射关系,返回映射区的起始地址*/
		(*v4l2_buffers)[n_buffers].start = mmap (NULL /* start anywhere */,buf.length,PROT_READ | PROT_WRITE /* required */,MAP_SHARED /* recommended */,*fd, buf.m.offset);

		if (MAP_FAILED == (*v4l2_buffers)[n_buffers].start)
		{
            perror("mmap failed");
			exit(-1);
		}
	}
	/*将申请到的帧缓冲全部入队列，以存放采集到的数据*/
    for (i = 0; i < n_buffers; ++i)
    {
		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		/*把数据从缓存中读取出来*/
		ret =  ioctl (*fd, VIDIOC_QBUF, &buf);
		if (ret < 0)//申请到的缓冲进入列队
		{
			perror("VIDIOC_QBUF failed");
			exit(-1);
		}
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    /*开始视频的采集*/           
	ret = ioctl(*fd, VIDIOC_STREAMON, &type);
    if (ret < 0 ) //开始捕捉图像数据
	{
	   perror("VIDIOC_STREAMON failed");
	   exit(-1);
	}

	return n_buffers;

}
/*关闭摄像头，释放资源*/
void v4l2_close(int *fd,int n_buffers,struct buffer *v4l2_buffers,char **format)
{
	int i = 0;
    enum v4l2_buf_type type;
	/*释放帧缓冲*/
    for (i = 0; i < n_buffers; i++)
       if (-1 == munmap (v4l2_buffers->start, v4l2_buffers->length))
            printf ("v4l2_buffers %d munmap error\n",i);
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	/*停止视频的采集*/
	if (-1 == ioctl(*fd, VIDIOC_STREAMOFF, &type)) 
			printf("VIDIOC_STREAMOFF\n");
	/*关闭视频设备*/
    close (*fd);
	//释放format
	free(*format);
}
/*从摄像头中读取一帧数据*/
void v4l2_read_frame(int *fd,struct buffer *v4l2_buffers,unsigned char **v4l2_data)
{
    struct v4l2_buffer buf;
	int ret = 0;

    memset(&buf,0,sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

	/*出队列以取得已采集数据的帧缓冲，取得原始采集数据*/
	ret = ioctl (*fd, VIDIOC_DQBUF, &buf);
	if(ret < 0)
		printf("failure read v4l2_buffers\n");
	else
	{
		(*v4l2_data) = (unsigned char *)v4l2_buffers[buf.index].start;
		//printf("read v4l2_frame success\n");
	}

	/*将缓冲重新入队列尾,这样可以循环采集*/ 
	/*再将其入列*/
	ret=ioctl (*fd, VIDIOC_QBUF, &buf); 
	if(ret < 0)
		perror("failture VIDIOC_QBUF");
}

/*取像并将摄像头读出来的yuyv422格式数据转换为yuv420p*/
void rf_yuyv422_yuv420p(int *fd ,struct buffer *v4l2_buffers,unsigned char *yuv420p_data,int width,int height)
{
    struct v4l2_buffer buf;
	int ret = 0;
	unsigned char *v4l2_data = NULL;
	/*FILE *fp;
	fp = fopen("test.jpg","wb");
	if(fp == NULL)
	{
		printf("can't create test.jpg\n");
		exit(-1);
	}*/

    memset(&buf,0,sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
	
	//v4l2_data = malloc(65536*sizeof(char));
	/*if(v4l2_data == NULL)
	{
		printf("in rf_yuyv422_yuv420p malloc error\n");
		exit(-1);
	}*/
		//printf("hello\n");

	/*出队列以取得已采集数据的帧缓冲，取得原始采集数据*/
	ret = ioctl (*fd, VIDIOC_DQBUF, &buf);
	if(ret < 0)
		printf("failure read v4l2_buffers\n");
	else
	{
		v4l2_data = (unsigned char *)v4l2_buffers[buf.index].start;
		//int ret = 0;
		//ret = fwrite(v4l2_buffers[buf.index].start,v4l2_buffers[buf.index].length,1,fp);
		//printf("write ret is %d\n",ret);
		//printf("read v4l2_frame success\n");
	}

	/*开始转换格式*/
    unsigned char * pyuv420u=(yuv420p_data)+width*height;
    unsigned char * pyuv420v=pyuv420u+(width*height)/4;
    int uv_count=0;
    int i,j;
    for(i=0;i<height;i+=2)
     for(j=0;j<width;j+=2)
     {

         memcpy((yuv420p_data)+i*width+j,v4l2_data+i*width*2+j*2,1);
         memcpy((yuv420p_data)+(i+1)*width+j,v4l2_data+(i+1)*width*2+j*2,1);
         memcpy((yuv420p_data)+i*width+(j+1),v4l2_data+i*width*2+(j+1)*2,1);
         memcpy((yuv420p_data)+(i+1)*width+(j+1),v4l2_data+(i+1)*width*2+(j+1)*2,1);
          //保留U 分量
         memcpy(pyuv420u+uv_count,v4l2_data+i*width*2+j*2+1,1);
             //保留V分量;
         memcpy(pyuv420v+uv_count,v4l2_data+(i+1)*width*2+(j+1)*2+1,1);
         uv_count++;
		 //printf("uv_count is %d\n",uv_count);

     }
		//printf("hello\n");
	/*将缓冲重新入队列尾,这样可以循环采集*/ 
	/*再将其入列*/
	ret=ioctl (*fd, VIDIOC_QBUF, &buf); 
	if(ret < 0)
		perror("failture VIDIOC_QBUF");

	//free(v4l2_data);
}

