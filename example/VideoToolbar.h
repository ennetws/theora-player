#pragma once

#include <QWidget>
#include "ui_VideoToolbar.h"

class VideoToolbar : public QWidget{
    Q_OBJECT

public:
    Ui::VideoToolbarWidget * ui;

public:
    VideoToolbar() : ui(new Ui::VideoToolbarWidget)
    {
        ui->setupUi(this);

        ui->pauseLabel->setVisible(false);
        ui->playLabel->setVisible(false);
    }

public slots:
    void togglePlay()
    {

    }

};
