#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>

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
}

// ![0]
