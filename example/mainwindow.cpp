#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include "VideoWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    VideoWidget * v = new VideoWidget();
    ui->centralLayout->addWidget(v);

    QString fileName = QFileDialog::getOpenFileName(this, "Open Video", "", "Video Files (*.ogg)");

    v->loadVideo(fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}
