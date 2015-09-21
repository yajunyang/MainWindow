#include "dialogvideo.h"
#include "ui_dialogvideo.h"

DialogVideo::DialogVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVideo)
{
    ui->setupUi(this);
}

DialogVideo::~DialogVideo()
{
    delete ui;
}
