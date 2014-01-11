/*************************************************************************
	> File Name: V4l2Cam.cpp
	> Author: mcsheng
	> Created Time: 2014年01月07日 星期二 12时24分31秒
 ************************************************************************/
#include"V4l2Cam.h"
#include<unistd.h>
Test::Test( ):QObject( )
{
	timer = new QTimer( ); 

	//初始化为0
	formatInt = 0;
	width = 640;
	height = 480;
	rgb = new unsigned char[width*height*3];
	n_buffers = v4l2_init(&fd,&v4l2_buffers,&format,width,height);
	if(strcmp(format,"JPEG") == 0)
	{
		formatInt = 1;
	}
	else if(strcmp(format,"YUYV") == 0)
	{
		formatInt = 2;
	}
	v4l2_data = new unsigned char[(int)(v4l2_buffers->length)];
	v_data = new unsigned char[(int)(v4l2_buffers->length)];
}
void Test::testTime(void)
{
	timer->start(40);
	connect(timer,SIGNAL(timeout( )),this,
				SLOT(testThread( )));
}
void Test::testThread(void)
{
	v4l2_read_frame(&fd,v4l2_buffers,&v4l2_data);
	memcpy(v_data,v4l2_data,v4l2_buffers->length);
	//std::cout << v4l2_buffers->length << std::endl;
	//usleep(500000);
	emit oneFrame( );
}
void Test::switchVideo(const QString &videoString)
{
	//qDebug( ) << videoString;
	std::cout << "h" << std::endl;
}
Test::~Test( )
{
	delete[] rgb;
	v4l2_close(&fd,n_buffers,v4l2_buffers,&format);

}
V4l2Cam::V4l2Cam(QWidget *parents):QWidget(parents)
{

	camLabel = new QLabel( );
	videoLabel = new QLabel(tr("摄像头") );
	videoEdit = new QLineEdit( );
	videoButton = new QPushButton(tr("切换"));
	connect(videoButton,SIGNAL(clicked( )),this,SLOT(switchVideo( )));
	QHBoxLayout *underLayout = new QHBoxLayout( );

	underLayout->addWidget(videoLabel);
	underLayout->addWidget(videoEdit);
	underLayout->addWidget(videoButton);

	QVBoxLayout *allLayout = new QVBoxLayout;
	allLayout->addWidget(camLabel);
	allLayout->addLayout(underLayout);
	setLayout(allLayout);
	//timer = new QTimer( );

	thread = new QThread;
	test = new Test;
	frameNum = 0;
	previousFrame = new unsigned char[test->width*test->height*3];
	differFrame = new unsigned char[test->width*test->height*3];
	tmpFrame = new unsigned char[test->width*test->height*3];
	test->moveToThread(thread);
	connect(test,SIGNAL(oneFrame( )),this,SLOT(update( )));
	connect(thread,SIGNAL(started( )),test,SLOT(testTime( )));
	connect(this,SIGNAL(switchSignal(const QString &)),test,
				SLOT(switchVideo(const QString &)));
	thread->start( );
	if(test->formatInt == 1)
	{
		img = new QImage( );
	}
	else if(test->formatInt == 2)
	{
		img = new QImage(test->rgb,test->width,test->height,QImage::Format_RGB888);
	}
	//QObject::connect(timer,SIGNAL(timeout( )),this,
				//SLOT(cam_show( )));
	//QObject::connect(timer,SIGNAL(timeout( )),this,
				//SLOT(update( )));
	//timer->start(10);
}

void V4l2Cam::cam_show(void)
{
	/*
	//jpeg
	if(formatInt == 1)
	{
		v4l2_read_frame(&fd,v4l2_buffers,&v4l2_data);
		//std::cout << v4l2_buffers->length << std::endl;
		img->loadFromData(v4l2_data,v4l2_buffers->length,"JPG");
		camLabel->setPixmap(QPixmap::fromImage(*img));
	}
	//yuyv422
	else if(formatInt == 2)
	{
		v4l2_read_frame(&fd,v4l2_buffers,&v4l2_data);
		//yuyv422转rgb
		yuyv422_to_rgb888_buffer(v4l2_data,rgb,width,height);
		//std::cout << v4l2_buffers->length << std::endl;
		//img->loadFromData(rgb,width*height*3*sizeof(char));
		camLabel->setPixmap(QPixmap::fromImage(*img));
	}*/
}
//重构父类的虚函数,实现绘图,利用paintEvent函数显示效果好
//update( ) ->  paintEvent
void V4l2Cam::paintEvent(QPaintEvent *)
{
	//jpeg
	if(test->formatInt == 1)
	{
		//v4l2_read_frame(&(test->fd),test->v4l2_buffers,&(test->v4l2_data));
		//std::cout << v4l2_buffers->length << std::endl;
		//usleep(500000);
		//std::cout << "p" << test->v4l2_buffers->length << std::endl;
		img->loadFromData(test->v_data,test->v4l2_buffers->length-40,"JPG");
		camLabel->setPixmap(QPixmap::fromImage(*img));
	}
	//yuyv422
	else if(test->formatInt == 2)
	{
		//v4l2_read_frame(&(test->fd),test->v4l2_buffers,&(test->v4l2_data));
		//yuyv422转rgb
		//mutex互斥量锁定
		//mutex.lock( );
		frameNum++;
		if(frameNum == 5)
		{
			yuyv422_to_rgb888_buffer(test->v4l2_data,test->rgb,test->width,test->height);
			memcpy(previousFrame,test->rgb,test->width*test->height*3);
			//frameNum++;
		}
		else
		{
			yuyv422_to_rgb888_buffer(test->v4l2_data,test->rgb,test->width,test->height);
			memcpy(tmpFrame,test->rgb,test->width*test->height*3);
			//水平翻转
			//HFlip(test->rgb,test->width,test->height);
			//灰度图
			Gray(previousFrame,test->width,test->height);
			Gray(test->rgb,test->width,test->height);
			//相减
			Substruction(test->rgb,previousFrame,differFrame,test->width,test->height);
			memcpy(previousFrame,tmpFrame,test->width*test->height*3);
			//二值化
			Binarization(differFrame,test->width,test->height,100); 
			//Binarization(differFrame,test->width,test->height,10); 
			img->loadFromData(differFrame,test->width*test->height*3*sizeof(char));
			camLabel->setPixmap(QPixmap::fromImage(*img));
		}
		//mutex.unlock( );
	}
}

int V4l2Cam::yuyv422_to_rgb888_buffer(unsigned char *yuyv422, unsigned char *rgb, unsigned int width, unsigned int height)
{
	unsigned int in, out = 0;
 	unsigned int pixel_16;
 	unsigned char pixel_24[3];
 	unsigned int pixel32;
 	int y0, u, y1, v;
 	for(in = 0; in < width * height * 2; in += 4) 
	{
		pixel_16 =
			yuyv422[in + 3] << 24 |
			yuyv422[in + 2] << 16 |
			yuyv422[in + 1] <<  8 |
			yuyv422[in + 0];
 	 	y0 = (pixel_16 & 0x000000ff);
 	 	u  = (pixel_16 & 0x0000ff00) >>  8;
 	 	y1 = (pixel_16 & 0x00ff0000) >> 16;
 	 	v  = (pixel_16 & 0xff000000) >> 24;
 	 	pixel32 = yuyv422_to_rgb_pixel(y0, u, v);
 	 	pixel_24[0] = (pixel32 & 0x000000ff);
 	 	pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
 	 	pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
 	 	rgb[out++] = pixel_24[0];
 	 	rgb[out++] = pixel_24[1];
 	 	rgb[out++] = pixel_24[2];
 	 	pixel32 = yuyv422_to_rgb_pixel(y1, u, v);
 	 	pixel_24[0] = (pixel32 & 0x000000ff);
 	 	pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
 	 	pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
 	 	rgb[out++] = pixel_24[0];
 	 	rgb[out++] = pixel_24[1];
 	 	rgb[out++] = pixel_24[2];
 	}	
 	return 0;
}	

int V4l2Cam::yuyv422_to_rgb_pixel(int y, int u, int v)
{
	unsigned int pixel32 = 0;
 	unsigned char *pixel = (unsigned char *)&pixel32;
 	int r, g, b;
 	r = y + (1.370705 * (v-128));
 	g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 	b = y + (1.732446 * (u-128));
 	if(r > 255) r = 255;
 	if(g > 255) g = 255;
 	if(b > 255) b = 255;
 	if(r < 0) r = 0;
 	if(g < 0) g = 0;
 	if(b < 0) b = 0;
 	pixel[0] = r * 220 / 256;
 	pixel[1] = g * 220 / 256;
 	pixel[2] = b * 220 / 256;
 	return pixel32;
}	

bool V4l2Cam::HFlip(unsigned char* rgb, int width, int height)
{
	// 输入参数合法性判断
	if(rgb==NULL||width<=0||height<=0)return false;

	// 每行图像数据的字节数
	//int LBytes  = (width*24+31)/32*4;
	int LBytes  = width*3;

	// 临时RGB图像指针
	unsigned char *rgbtmp  = (unsigned char*)malloc(LBytes*height);

	if(rgbtmp == NULL)
	{
		return false;
	}
	else
	{
		//清零
		memset(rgbtmp , 0 , LBytes*height);
	}

	int tmpLine = 0;
	// 每行
	for(int i = 0; i < height; i++)
	{
		tmpLine = i*LBytes;
		// 每列
		for(int j = 0; j < width; j++)
		{
			//memcpy((rgbtmp + LBytes * (height - 1 - i) + 3*(width - 1 -j)) , (rgb + LBytes * (height - 1 - i) + 3*j) , 3);
			//memcpy(rgbtmp+i*LBytes+j*3,rgb+(width-1-j)*3+i*LBytes,3);
			//下面这个复制比上面的乘法运算更少
			memcpy(rgbtmp+tmpLine+j*3,rgb+(width-1-j)*3+tmpLine,3);
			//实现垂直翻转
			//memcpy(rgbtmp+tmpLine+j*3,rgb+j*3+(height-i)*LBytes,3);
		}
	}
	memcpy(rgb , rgbtmp , LBytes*height);
	free(rgbtmp);
	return true;
}

bool V4l2Cam::Gray(unsigned char* rgb, int width, int height)
{
	// 输入参数合法性判断
	if(rgb==NULL||width<=0||height<=0)return false;

	// 每行图像数据的字节数
	//int LBytes  = (width*24+31)/32*4;
	// 每行
	unsigned char gray; 
	for(int i = 0; i < height; i++)
	{
		// 每列
		for(int j = 0; j < width; j++)
		{
			//浮点算法,效果最好,经典公式法
			/*gray = (*rgb)*0.11+(*(rgb+1))*0.59+(*(rgb+2))*0.3;
			*(rgb) = gray; 
			*(rgb+1) = gray;
			*(rgb+2) = gray;*/
			//仅取绿色,效果较差
			gray = (*(rgb+1));
			*(rgb) = gray; 
			*(rgb+1) = gray;
			*(rgb+2) = gray;
			//平均值法,效果中规中矩
			/*unsigned char p = ((*rgb)+(*(rgb+1))+(*(rgb+2)))/3;
			*(rgb) = p;//B 
			*(rgb+1) = p;//G
			*(rgb+2) = p;//R*/
			rgb = rgb + 3;
		}
	}
	return true;
}

//二值化
bool V4l2Cam::Binarization(unsigned char* rgb, int width, int height,int value)
{
	// 输入参数合法性判断
	if(rgb==NULL||width<=0||height<=0||value<0)return false;

	// 每行
	for(int i = 0; i < height; i++)
	{
		// 每列
		for(int j = 0; j < width; j++)
		{
			if(*rgb > value)
			{
				*rgb = 255;
				*(rgb+1) = 255;
				*(rgb+2) = 255;
			}
			else
			{
				*rgb = 0;
				*(rgb+1) = 0;
				*(rgb+2) = 0;
			}
			rgb = rgb + 3;
		}
	}
	return true;
}

bool V4l2Cam::Substruction(unsigned char* previousFrame,unsigned char *currentFrame,unsigned char *differFrame, int width, int height)
{
	// 输入参数合法性判断
	if(previousFrame==NULL||currentFrame==NULL||width<=0||height<=0)
	{
		std::cout << "a" << std::endl;
		return false;
	}

	unsigned char value;
	unsigned char *p = differFrame;
	// 每行
	for(int i = 0; i < height; i++)
	{
		// 每列
		for(int j = 0; j < width; j++)
		{ 
			value = (*currentFrame) - (*previousFrame);
			*differFrame = value;
			*(differFrame+1) = value; 
			*(differFrame+2) = value;
			previousFrame = previousFrame + 3;
			currentFrame = currentFrame + 3;
			differFrame = differFrame + 3;
		}
	}

	/*for(int i = 0; i < height; i++)
	{
		// 每列
		for(int j = 0; j < width; j++)
		{ 
			std::cout << (int)*(p++) << std::endl;
		}
	}*/
	return true;
}
 

void V4l2Cam::switchVideo(void)
{
	emit switchSignal(videoEdit->text( ));
}
V4l2Cam::~V4l2Cam( )
{
	//delete[] rgb;
	delete   camLabel;
	delete   img;
	delete   videoLabel;
	delete   videoEdit;
	delete   videoButton;
	delete[ ] previousFrame;
	//v4l2_close(&fd,n_buffers,v4l2_buffers,&format);
}

