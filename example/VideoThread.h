#pragma once

#include <QWidget>
#include <QThread>

class VideoThread : public QThread{
    Q_OBJECT
public:
    VideoThread(QWidget * widget)
    {
        this->w = widget;
    }

    QWidget * w;

    void run()
    {
        forever{
            emit Update();
        }
    }
signals:
    void Update();
};
