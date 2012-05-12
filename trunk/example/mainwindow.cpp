#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "TheoraVideoManager.h"
#include "TheoraVideoClip.h"
#include "TheoraVideoFrame.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TheoraVideoManager * mgr = new TheoraVideoManager();
    TheoraVideoClip* clip = mgr->createVideoClip("D:/temp/sample2.ogg", TH_RGB, 4);

	clip->waitForCache();

    TheoraVideoFrame* f = clip->getNextFrame();

	if(f)
	{
		unsigned char* data = f->getBuffer();

		QImage res (clip->getWidth(), clip->getHeight(), QImage::Format_RGB32);

		unsigned char* img_data = res.bits();

		int w = clip->getWidth();
		int h = clip->getHeight();

		for(int i = 0; i < w*h*3; i += 3)
		{
			unsigned char r = data[i+0];
			unsigned char g = data[i+1];
			unsigned char b = data[i+2];

			img_data[i+0] = r;
			img_data[i+1] = g;
			img_data[i+2] = b;
		}
		
		res.save("test.png");
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}
