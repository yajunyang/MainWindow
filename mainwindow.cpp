#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>

#include "Image2.h"
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <cstdio>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    ui->dockWidget->setWidget(scrollArea);
    ui->dockWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    imageLabel->setGeometry((scrollArea->geometry().width() - imageLabel->geometry().width()) / 2,
                            (scrollArea->geometry().height() - imageLabel->geometry().height()) / 2,
                            0, 0);
    this->setCentralWidget(ui->dockWidget);
    this->setWindowTitle(tr("Ant"));
    this->resize(700, 600);
    connectUiToFunction();

    linkpro = new LinkPro(this);
    linktrans = new LinkTrans(this);

    connect(ui->actionImgPro, SIGNAL(triggered()), linkpro, SLOT(showProDialog()));
    connect(ui->actionImgTrans, SIGNAL(triggered()), linktrans, SLOT(showTransDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


// ![0]
void MainWindow::open()
{
    imageFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                             "/home",
                                     tr("Images (*.png *.xpm *.jpg)"));
    qDebug(imageFile.toStdString().c_str());

    if(!imageFile.isEmpty())
    {
       if(ui->dockWidget->isHidden())
           ui->dockWidget->setVisible(true);

       linkpro->getOpenImageFile(imageFile);
       linktrans->getOpenImageFile(imageFile);
       QImage image(imageFile);
       if(image.isNull())
       {
           QMessageBox::information(this, tr("Image Process"), tr("Cannot't load %1.").arg(imageFile));
           return;
       }
       imageLabel->setPixmap(QPixmap::fromImage(image));
       scaleFactor = 1.0;

       ui->actionPrint->setEnabled(true);
       ui->actionFit_to_Window->setEnabled(true);
       updataActions();

       if(!ui->actionFit_to_Window->isChecked())
           imageLabel->adjustSize();

       ui->dockWidget->setMaximumWidth(QImage(imageFile).width());
       ui->dockWidget->setMaximumHeight(QImage(imageFile).height());
    }
}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{

}

void MainWindow::level_set()
{
    IplImage * image=cvLoadImage("E:/yang.jpg",1);// 声纳.jpg    1-28.BMP
    CvSize size=cvGetSize(image);
    int i,j;
    IplImage * image_gray_a=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
    IplImage * display1=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
    IplImage * display2=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
    IplImage * display3=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
    IplImage * display_contour=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
    IplImage * imgDisplay=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);

    IplImage * image32=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * imgGauss=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * image_laplace=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * imgG=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * imgU=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * imgU_temp=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * Ix=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * Iy=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);
    IplImage * Edge=cvCreateImage(cvGetSize(image),IPL_DEPTH_32F,1);


    int iterNum = 850;

    cvCvtColor(image,image_gray_a,CV_BGR2GRAY);
    cvConvertScale(image_gray_a,image32,1,0);
    cvSmooth(image32,imgGauss,CV_GAUSSIAN,0,0,1.5,0);

    Sobel(imgGauss,Ix,Iy);

    CvScalar cur,cur1,cur2;
    for (i=0; i<size.height; i++)
    {
        for(j=0; j<size.width; j++)
        {
            cur1 = cvGet2D(Ix,i,j);
            cur2 = cvGet2D(Iy,i,j);
            cur.val[0] = 1.0/(1.0+cur1.val[0]*cur1.val[0]+cur2.val[0]*cur2.val[0]);
            cvSet2D(imgG,i,j,cur);
        }
    }

    int w=15;
    int w2=0;
    double c0=14.0;
    //define initial level set function (LSF) as -c0, 0, c0 at points outside, on
    //the boundary, and inside of a region R, respectively.
    for (i=0; i<size.height; i++)
    {
        for(j=0; j<size.width; j++)
        {
            if (i<w || i>size.height-w-1 || j<w || j>size.width-w-1)
            {
                cur.val[0] = c0;
            }
            else if (i>w && i<size.height-w-1 && j>w && j<size.width-w-1)
            {
                cur.val[0] = -c0;
            }
            else cur.val[0] = 0;
            // Note: this can be commented out. The intial LSF does NOT necessarily need a zero level set.
            cvSet2D(imgU,i,j,cur);
        }
    }

    //ImgDraw(image, imgU, imgDisplay);
    //cvNamedWindow("LevelSet");
    //cvShowImage("LevelSet",imgDisplay);
    //cvWaitKey(0);
    double epsilon=1.5;//1.5
    double timestep=7;//7
    double lambda=10;//5
    double mu=0.2/timestep;
    double alf=10.5;//1.5
    for (int k=0;k<iterNum;k++)
    {
        Evolution2(imgU,imgG,lambda,mu,alf,epsilon,timestep,1);

        if (k%20==0)
        {
            ImgDraw(image, imgU, imgDisplay);
            cvShowImage("LevelSet",imgDisplay);
            cvWaitKey(20);
        }
    }

    cvWaitKey(0);

    cvDestroyAllWindows();
    cvReleaseImage(&image_gray_a);
    cvReleaseImage(&display1);
    cvReleaseImage(&display2);
    cvReleaseImage(&display3);
    cvReleaseImage(&display_contour);
    cvReleaseImage(&imgDisplay);
    cvReleaseImage(&Ix);
    cvReleaseImage(&Iy);
    cvReleaseImage(&imgG);
    cvReleaseImage(&imgU);
    cvReleaseImage(&imgU_temp);
    cvReleaseImage(&imgGauss);
    cvReleaseImage(&image32);
    cvReleaseImage(&image_laplace);
    cvReleaseImage(&image);
}

void MainWindow::zoomIn()
{
    scaleImage(1.26);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normlSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::fitToWindow()
{
    bool fitToWindow = ui->actionFit_to_Window->isChecked();
    scrollArea->setWidgetResizable(ui->actionFit_to_Window);
    if (!fitToWindow) {
        normlSize();
    }
    updataActions();
}

void MainWindow::updataActions()
{
    ui->actionZoom_In_25->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionZoom_Out_25->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionNormal_Size->setEnabled(!ui->actionFit_to_Window->isChecked());
}

void MainWindow::print()
{
    Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);

    if(dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

   ui->actionZoom_In_25->setEnabled(scaleFactor < 3.0);
   ui->actionZoom_Out_25->setEnabled(scaleFactor > 0.333);
}

void MainWindow::about()
{

}

void MainWindow::connectUiToFunction()
{
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionZoom_In_25, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(ui->actionZoom_Out_25, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(ui->actionNormal_Size, SIGNAL(triggered()), this, SLOT(normlSize()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionFit_to_Window, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    connect(ui->actionLevel_Set_Segmentation, SIGNAL(triggered()), this, SLOT(level_set()));
}

// ![0]
