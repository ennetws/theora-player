#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QThread>
#include <QTimer>

#include "TheoraVideoManager.h"
#include "TheoraVideoClip.h"
#include "TheoraVideoFrame.h"

#include "global.h"
#include "VideoThread.h"

class VideoWidget: public QWidget{
    Q_OBJECT

public:
    VideoWidget(QWidget * parent = 0) : QWidget(parent)
    {
        mgr = NULL;
        clip = NULL;
        currentFrame = NULL;

        this->setAutoFillBackground(false);
    }

    void loadVideo(QString fileName)
    {
        mgr = new TheoraVideoManager();
        clip = mgr->createVideoClip(qPrintable(fileName), TH_RGB, 16);
        clip->setAutoRestart(1); // looping
        clip->waitForCache();

        img = new QImage(clip->getWidth(), clip->getHeight(), QImage::Format_RGB32);

        w = clip->getWidth();
        h = clip->getHeight();

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(animate()));
        timer->start(40);
    }

    void playVideo()
    {
        clip->play();
        started = true;
    }

public:
    TheoraVideoManager * mgr;
    TheoraVideoClip* clip;
    TheoraVideoFrame* currentFrame;

    QImage * img;
    int w;
    int h;

    bool started;

protected:

    void paintEvent (QPaintEvent *)
    {
        if(currentFrame)
        {
            unsigned char* data = currentFrame->getBuffer();
            unsigned char* img_data = img->bits();

            for(int i = 0; i < w*h*3; i += 3)
            {
                int pixelIdx = (i / 3) * 4;

                img_data[pixelIdx+0] = data[i+0]; // r
                img_data[pixelIdx+1] = data[i+1]; // g
                img_data[pixelIdx+2] = data[i+2]; // b
            }

            QPainter painter(this);
            painter.drawImage(QRect(this->rect()),*img);
        }
    }

    void updateFrame(float time_increase)
    {
        if (started)
        {
            // let's wait until the system caches up a few frames on startup
            if (clip->getNumReadyFrames() < clip->getNumPrecachedFrames()*0.5f)
                return;
            started=0;
        }
        mgr->update(time_increase);
    }

public slots:

    void animate()
    {
        TheoraVideoFrame* nextFrame = clip->getNextFrame();
        if(nextFrame && nextFrame != currentFrame)
        {
            currentFrame = nextFrame;
            repaint();
        }

        static unsigned long time = GetTickCount();
        unsigned long t = GetTickCount();

        float diff = (t-time) / 1000.0f;
        if (diff > 0.25f) diff=0.05f; // prevent spikes (usually happen on app load)
        updateFrame(diff);

        // FPS stuff
        static unsigned int fps_timer = time, fps_counter = 0;
        if (t - fps_timer >= 250){
            //char title[512],debug[256]="";
            //sprintf(title,"%d FPS; %s",fps_counter*4,debug);
            fps_counter=0;
            fps_timer=t;
        }
        else fps_counter++;

        time = t;
    }

private:


};
