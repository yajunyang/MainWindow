#include "ui_dialogpro.h"
#include "dialogpro.h"
#include <QLineEdit>

DialogPro::DialogPro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPro)
{
    ui->setupUi(this);

    dialogNewVal = new QDialog(this);
    dialogUpDiff = new QDialog(this);
    dialogLowDiff = new QDialog(this);

    setDialogNewVal();///
    setDialogUpDiff();
    setDialogLowDiff();

    setSmoothParamToDefault();
    setDilateParamToDefault();
    setErodeParamToDefault();
    setThresholdParamToDefault();
    setFloodFillParamToDefault();

    setLimit();

    connectOkandCancel();
    errorConnecColorChange();
}

DialogPro::~DialogPro()
{
    delete ui;
}

void DialogPro::setLimit()
{
    ///  Smooth
    QRegExp regExpSmooth("[1-9]");
    QRegExp regExpSmoothDouble("[0-9].[0-9]");
    ui->smoothParam1->setValidator(new QRegExpValidator(regExpSmooth, this));
    ui->smoothParam2->setValidator(new QRegExpValidator(regExpSmooth, this));
    ui->smoothParam3->setValidator(new QRegExpValidator(regExpSmoothDouble, this));
    ui->smoothParam4->setValidator(new QRegExpValidator(regExpSmoothDouble, this));

    /// ThresHold
    QRegExp regExpThresHold("[0-9][0-9][0-9]");
    ui->threshhold->setValidator(new QRegExpValidator(regExpThresHold, this));
    ui->threshMaxValue->setValidator(new QRegExpValidator(regExpThresHold, this));

    /// FloodFill
    QRegExp regExpFloodFill("[4, 8]");
    QRegExp regExpSeed("[0-9][0-9][0-9][0-9]");
    ui->floodFillFlags->setValidator(new QRegExpValidator(regExpFloodFill, this));
    ui->floodFillSeedPointx->setValidator(new QRegExpValidator(regExpSeed, this));
    ui->floodFillSeedPointy->setValidator(new QRegExpValidator(regExpSeed, this));
    red->setValidator(new QRegExpValidator(regExpThresHold, this));
    green->setValidator(new QRegExpValidator(regExpThresHold, this));
    blue->setValidator(new QRegExpValidator(regExpThresHold, this));
    lo1->setValidator(new QRegExpValidator(regExpThresHold, this));
    lo2->setValidator(new QRegExpValidator(regExpThresHold, this));
    lo3->setValidator(new QRegExpValidator(regExpThresHold, this));
    up1->setValidator(new QRegExpValidator(regExpThresHold, this));
    up2->setValidator(new QRegExpValidator(regExpThresHold, this));
    up3->setValidator(new QRegExpValidator(regExpThresHold, this));

    /// Dilate
    QRegExp regExp("[0-9]");
    ui->dilateColsEdit->setValidator(new QRegExpValidator(regExpSmooth, this));
    ui->dilateRowsEdit->setValidator(new QRegExpValidator(regExpSmooth, this));
    ui->dilateAnchor_xEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->dilateAnchor_yEdit->setValidator(new QRegExpValidator(regExp, this));

    /// Erode
    ui->erodeColsEdit->setValidator(new QRegExpValidator(regExpSmooth, this));
    ui->erodeRowsEdit->setValidator(new QRegExpValidator(regExpSmooth, this));
    ui->erodeAnchor_xEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->erodeAnchor_yEdit->setValidator(new QRegExpValidator(regExp, this));

       // ![1]
}

/// ![Refresh]
void DialogPro::refreshCurrentErode()   // 1  pass
{
    mcvErode.cols = ui->erodeColsEdit->text().toInt();
    mcvErode.rows = ui->erodeRowsEdit->text().toInt();
    mcvErode.ancher_x = ui->erodeAnchor_xEdit->text().toInt();
    mcvErode.ancher_y = ui->erodeAnchor_yEdit->text().toInt();

    int i = ui->erodeShape->currentIndex();
    switch(i)
    {
    case 0:
        mcvErode.shape = CV_SHAPE_RECT;
        break;
    case 1:
        mcvErode.shape = CV_SHAPE_CROSS;
        break;
    case 2:
        mcvErode.shape = CV_SHAPE_ELLIPSE;
        break;
    default:
        mcvErode.shape = CV_SHAPE_CUSTOM;
    }

    mcvErode.values = ui->erodeValues->text().toInt();
    mcvErode.iterations = ui->erodeIterationsEdit->text().toInt();
}

void DialogPro::refreshCurrentDilate()   // 2 pass
{
    mcvDilate.cols = ui->dilateColsEdit->text().toInt();
    mcvDilate.rows= ui->dilateRowsEdit->text().toInt();
    mcvDilate.ancher_x = ui->dilateAnchor_xEdit->text().toInt();
    mcvDilate.ancher_y = ui->dilateAnchor_yEdit->text().toInt();

    int i = ui->dilateShape->currentIndex();
    switch(i)
    {
    case 0:
        mcvDilate.shape = CV_SHAPE_RECT;
        break;
    case 1:
        mcvDilate.shape = CV_SHAPE_CROSS;
        break;
    case 2:
        mcvDilate.shape = CV_SHAPE_ELLIPSE;
        break;
    default:
        mcvDilate.shape = CV_SHAPE_CUSTOM;
    }

    mcvDilate.values = ui->dilateValues->text().toInt();
    mcvDilate.iterations = ui->dilateIterationsEdit->text().toInt();
}

void DialogPro::refreshCurrentSmooth()   // 3 pass
{
    int i = ui->smoothtype->currentIndex();
    switch(i)
    {
    case 0:
        mcvSmooth.smoothType = CV_BLUR;
        break;
    case 1:
        mcvSmooth.smoothType = CV_BLUR_NO_SCALE;
        break;
    case 2:
        mcvSmooth.smoothType = CV_MEDIAN;
        break;
    case 3:
        mcvSmooth.smoothType = CV_GAUSSIAN;
        break;
    default:
        mcvSmooth.smoothType = CV_BILATERAL;
    }

    mcvSmooth.param1 = ui->smoothParam1->text().toInt();
    mcvSmooth.param2 = ui->smoothParam2->text().toInt();
    mcvSmooth.param3 = ui->smoothParam3->text().toDouble();
    mcvSmooth.param4 = ui->smoothParam4->text().toDouble();
}

void DialogPro::refreshCurrentFloodFill() // 4  pass
{
    mcvFloodFill.seedPoint.x = ui->floodFillSeedPointx->text().toInt();
    mcvFloodFill.seedPoint.y = ui->floodFillSeedPointy->text().toInt();
    mcvFloodFill.newVal = cvScalar(red->text().toDouble(),
                                       green->text().toDouble(),
                                       blue->text().toDouble());;
    mcvFloodFill.loDiff = cvScalar(lo1->text().toDouble(),
                                      lo2->text().toDouble(),
                                      lo3->text().toDouble());;
    mcvFloodFill.upDiff = cvScalar(up1->text().toDouble(),
                              up2->text().toDouble(),
                              up3->text().toDouble());
    mcvFloodFill.comp = NULL;
    mcvFloodFill.flags = ui->floodFillFlags->text().toInt();
    mcvFloodFill.mask = NULL;
}

void DialogPro::refreshCurrentThresHold() // 5 pass
{
    mcvThresHold.threshold = ui->threshhold->text().toDouble();
    mcvThresHold.max_value = ui->threshMaxValue->text().toDouble();

    int i = ui->thresholdType->currentIndex();
    switch(i)
    {
    case 0:
        mcvThresHold.threshold_type = CV_THRESH_BINARY;
        break;
    case 1:
        mcvThresHold.threshold_type = CV_THRESH_BINARY_INV;
        break;
    case 2:
        mcvThresHold.threshold_type = CV_THRESH_TRUNC;
        break;
    case 3:
        mcvThresHold.threshold_type = CV_THRESH_TOZERO_INV;
        break;
    default:
        mcvThresHold.threshold_type = CV_THRESH_TOZERO;
    }
}

void DialogPro::refreshCurrentResize()    // 6 pass
{
    int i = ui->resizeSetInterPolation->currentIndex();
    switch(i)
    {
    case 0:
       mcvResize.interpolation = CV_INTER_LINEAR;
       break;
    case 1:
        mcvResize.interpolation = CV_INTER_NN;
        break;
    case 2:
        mcvResize.interpolation = CV_INTER_AREA;
        break;
    default:
        mcvResize.interpolation = CV_INTER_CUBIC;
    }

}

void DialogPro::refreshCurrentPyup()     // 7 pass
{
    mcvPyup.filter = CV_GAUSSIAN_5x5;
}

void DialogPro::refreshCurrentPydown()   // 8 pass
{
    mcvPydown.filter = CV_GAUSSIAN_5x5;
}
/// ![refresh]

/// ![SetSon-Son Dialog]
void DialogPro::setDialogNewVal()
{
    QGroupBox *groupBox = new QGroupBox(dialogNewVal);

    QLabel *label1 = new QLabel(groupBox);
    QLabel *label2 = new QLabel(groupBox);
    QLabel *label3 = new QLabel(groupBox);

    dialogNewVal->setWindowTitle("NewValParam");
    label1->setText("Red");
    label2->setText("Grren");
    label3->setText("Blue");

    red = new QLineEdit(groupBox);
    green = new QLineEdit(groupBox);
    blue = new QLineEdit(groupBox);

    okDialogNewVal = new QPushButton(groupBox);
    cancelDialogNewVal = new QPushButton(groupBox);

    groupBox->setGeometry(10, 10, 221, 161);

    label1->setGeometry(QRect(20, 20, 21, 16));
    label2->setGeometry(QRect(10, 50, 31, 16));
    label3->setGeometry(QRect(15, 80, 31, 20));

    red->setGeometry(QRect(50, 20, 91, 20));
    green->setGeometry(QRect(50, 50, 91, 20));
    blue->setGeometry(QRect(50, 80, 91, 20));

    cancelDialogNewVal->setGeometry(QRect(150, 100, 61, 23));
    okDialogNewVal->setGeometry(QRect(150, 130, 61, 23));

    cancelDialogNewVal->setText("Cancel");
    okDialogNewVal->setText("Ok");

    //// Link Button to Son-Son Dialog
    connect(ui->floodFillSetColorValue, SIGNAL(clicked()), dialogNewVal, SLOT(show()));
    connect(cancelDialogNewVal, SIGNAL(clicked()), dialogNewVal, SLOT(close()));

    connect(okDialogNewVal, SIGNAL(clicked()), this, SLOT(refreshCurrentFloodFill()));
    connect(okDialogNewVal, SIGNAL(clicked()), dialogNewVal, SLOT(close()));
}

void DialogPro::setDialogUpDiff()
{
    QGroupBox *groupBox = new QGroupBox(dialogUpDiff);

    QLabel *label1 = new QLabel(groupBox);
    QLabel *label2 = new QLabel(groupBox);
    QLabel *label3 = new QLabel(groupBox);

    dialogUpDiff->setWindowTitle("UpDiffParam");
    label1->setText("Up0");
    label2->setText("Up1");
    label3->setText("Up2");

    up1 = new QLineEdit(groupBox);
    up2 = new QLineEdit(groupBox);
    up3 = new QLineEdit(groupBox);
    okDialogUpDiff = new QPushButton(groupBox);
    cancelDialoUpDiff = new QPushButton(groupBox);

    groupBox->setGeometry(10, 10, 221, 161);

    label1->setGeometry(QRect(20, 20, 21, 16));
    label2->setGeometry(QRect(20, 50, 31, 16));
    label3->setGeometry(QRect(20, 80, 31, 20));

    up1->setGeometry(QRect(50, 20, 91, 20));
    up2->setGeometry(QRect(50, 50, 91, 20));
    up3->setGeometry(QRect(50, 80, 91, 20));

    cancelDialoUpDiff->setGeometry(QRect(150, 100, 61, 23));
    okDialogUpDiff->setGeometry(QRect(150, 130, 61, 23));

    cancelDialoUpDiff->setText("Cancel");
    okDialogUpDiff->setText("Ok");

    connect(ui->floodFillSetUpDiff, SIGNAL(clicked()), dialogUpDiff, SLOT(show()));
    connect(cancelDialoUpDiff, SIGNAL(clicked()), dialogUpDiff, SLOT(close()));
    connect(okDialogUpDiff, SIGNAL(clicked()), this, SLOT(refreshCurrentFloodFill()));
    connect(okDialogUpDiff, SIGNAL(clicked()), dialogUpDiff, SLOT(close()));
}

void DialogPro::setDialogLowDiff()
{
    QGroupBox *groupBox = new QGroupBox(dialogLowDiff);

    QLabel *label1 = new QLabel(groupBox);
    QLabel *label2 = new QLabel(groupBox);
    QLabel *label3 = new QLabel(groupBox);

    dialogLowDiff->setWindowTitle("LowDiffParam");
    label1->setText("Low0");
    label2->setText("Low1");
    label3->setText("Low2");

    lo1 = new QLineEdit(groupBox);
    lo2 = new QLineEdit(groupBox);
    lo3 = new QLineEdit(groupBox);
    okDialogLowDiff = new QPushButton(groupBox);
    cancelDialogLowDiff = new QPushButton(groupBox);

    groupBox->setGeometry(10, 10, 221, 161);

    label1->setGeometry(QRect(10, 20, 24, 16));
    label2->setGeometry(QRect(10, 50, 31, 16));
    label3->setGeometry(QRect(10, 80, 31, 20));

    lo1->setGeometry(QRect(50, 20, 91, 20));
    lo2->setGeometry(QRect(50, 50, 91, 20));
    lo3->setGeometry(QRect(50, 80, 91, 20));

    cancelDialogLowDiff->setGeometry(QRect(150, 100, 61, 23));
    okDialogLowDiff->setGeometry(QRect(150, 130, 61, 23));

    okDialogLowDiff->setText("OK");
    cancelDialogLowDiff->setText("Cancel");

    connect(ui->floodFillSetLoDiff, SIGNAL(clicked()), dialogLowDiff, SLOT(show()));

    connect(cancelDialogLowDiff, SIGNAL(clicked()), dialogLowDiff, SLOT(close()));
    connect(okDialogLowDiff, SIGNAL(clicked()), this, SLOT(refreshCurrentFloodFill()));
    connect(okDialogLowDiff, SIGNAL(clicked()), dialogLowDiff, SLOT(close()));
}
/// ![SetSon-Son Dialog]


/// ![Reset]
void DialogPro::setSmoothParamToDefault() // pass
{
    ui->smoothParam1->setText("3");
    ui->smoothParam2->setText("0");
    ui->smoothParam3->setText("0");
    ui->smoothParam4->setText("0");
    ui->smoothtype->setCurrentIndex(0);
}

void DialogPro::setDilateParamToDefault() // pass
{
    ui->dilateIterationsEdit->setText("1");
    ui->dilateAnchor_xEdit->setText("1");
    ui->dilateAnchor_yEdit->setText("1");
    ui->dilateColsEdit->setText("3");
    ui->dilateRowsEdit->setText("3");
    ui->dilateShape->setCurrentIndex(0);
    ui->dilateValues->setText("NULL");
}

void DialogPro::setErodeParamToDefault()
{
    ui->erodeIterationsEdit->setText("1");
    ui->erodeAnchor_xEdit->setText("1");
    ui->erodeAnchor_yEdit->setText("1");
    ui->erodeColsEdit->setText("3");
    ui->erodeRowsEdit->setText("3");
    ui->erodeShape->setCurrentIndex(0);
    ui->erodeValues->setText("NULL");
}

void DialogPro::setThresholdParamToDefault()
{
      ui->thresholdType->setCurrentIndex(0);
      ui->threshMaxValue->setText("100");
      ui->threshhold->setText("100");
}

void DialogPro::setFloodFillParamToDefault()
{
      red->setText("127");
      green->setText("127");
      blue->setText("127");

      up1->setText("0");
      up2->setText("0");
      up3->setText("0");

      lo1->setText("0");
      lo2->setText("0");
      lo3->setText("0");

      ui->floodFillSeedPointx->setText("0");
      ui->floodFillSeedPointy->setText("0");
      ui->floodFillFlags->setText("4");
      ui->floodFillSetMask->setCurrentIndex(0);
}
/// ![Reset]


/// ![Inform]
void DialogPro::informTransSmooth() // pass
{
    refreshCurrentSmooth();
    if(isSmoothDataOk())
    {
        emit signalTransSmooth();
    }
}

void DialogPro::informTransErode()/// pass
{
    refreshCurrentErode(); // Refresh data first
    if(isErodeDataOk())
    {
        emit signalTransErode();
    }
}

void DialogPro::informTransDilate()
{
    refreshCurrentDilate();
    if(isDilateDataOk())
    {
        emit signalTransDilate();
    }
}

void DialogPro::informTrannsResize()
{
    refreshCurrentResize();
    emit signalTransResize();
}

void DialogPro::informTransFloodFill()
{
   refreshCurrentFloodFill();
   if(isFloodFillDataOk())
   {
        emit signalTransFloodFill();
   }
}

void DialogPro::informTransThreshold()
{
    refreshCurrentThresHold();
    if(isThresHoldDataOk())
    {
        emit signalTransThreshold();
    }
}

void DialogPro::informTranPyrDown()
{
    refreshCurrentPydown();
    emit signalTransPyrDown();
}

void DialogPro::informTransPyrUp()
{
    refreshCurrentPyup();
    emit signalTransPyrUp();
}
/// ![inform]


/// ![visit]
McvImgProErode DialogPro::visitCurrentErode()
{
    return mcvErode;
}

McvImgProDilate DialogPro::visitCurrentDilate()
{
    return mcvDilate;
}

MCvImgProSmooth DialogPro::visitCurrentSmooth()
{
    return mcvSmooth;
}

MCvImgProFloodFill DialogPro::visitCurrentFloodFill()
{
    return mcvFloodFill;
}

MCvImgProThresHold DialogPro::visitCurrentThresHold()
{
    return mcvThresHold;
}

McvImgProResize DialogPro::visitCurrentResize()
{
    return mcvResize;
}

McvImgProPyup DialogPro::visitCurrentPyup()
{
    return mcvPyup;
}

McvImgProPydown DialogPro::visitCurrentPydown()
{
    return mcvPydown;
}
/// ![visit]


/// ![isOk]
bool DialogPro::isSmoothDataOk()
{
   return true;
}

bool DialogPro::isErodeDataOk()
{
    bool i = mcvErode.ancher_x < mcvErode.cols ? true : false;
    bool j = mcvErode.ancher_y < mcvErode.rows ? true : false;

    if(i && j) return true;
    else return false;
}

bool DialogPro::isDilateDataOk()
{
    bool i = mcvDilate.ancher_x < mcvDilate.cols ? true : false;
    bool j = mcvDilate.ancher_y < mcvDilate.rows ? true : false;
    if(i && j)return true;
    else return false;
}

bool DialogPro::isFloodFillDataOk()
{
    if(ui->floodFillSeedPointx->text().isEmpty()) return false;
    if(ui->floodFillSeedPointy->text().isEmpty()) return false;
    if(ui->floodFillFlags->text().isEmpty()) return false;
    if(red->text().isEmpty() || mcvFloodFill.newVal.val[0] > 255) return false;
    if(green->text().isEmpty() || mcvFloodFill.newVal.val[1] > 255) return false;
    if(blue->text().isEmpty() || mcvFloodFill.newVal.val[2] > 255) return false;
    if(lo1->text().isEmpty() || mcvFloodFill.loDiff.val[0] > 255) return false;
    if(lo2->text().isEmpty() || mcvFloodFill.loDiff.val[1] > 255) return false;
    if(lo3->text().isEmpty() || mcvFloodFill.loDiff.val[2] > 255) return false;
    if(up1->text().isEmpty() || mcvFloodFill.upDiff.val[0] > 255) return false;
    if(up2->text().isEmpty() || mcvFloodFill.upDiff.val[1] > 255) return false;
    if(up3->text().isEmpty() || mcvFloodFill.upDiff.val[2] > 255) return false;
    return true;
}

bool DialogPro::isThresHoldDataOk()
{
    if(mcvThresHold.max_value < 256 && mcvThresHold.threshold < 256)
        return true;
    else return false;
}
/// ![isOk]


/// ![Signal-Slot]
void DialogPro::connectOkandCancel()
{
    //// cancel

    connect(ui->cancelDilate, SIGNAL(clicked()), this, SLOT(setDilateParamToDefault()));
    connect(ui->cancelErode, SIGNAL(clicked()), this, SLOT(setErodeParamToDefault()));
    connect(ui->cancelSmooth, SIGNAL(clicked()), this, SLOT(setSmoothParamToDefault()));
    connect(ui->cancelThreshold, SIGNAL(clicked()), this, SLOT(setThresholdParamToDefault()));

    connect(ui->okDilate, SIGNAL(clicked()), this, SLOT(informTransDilate()));
    connect(ui->okErode, SIGNAL(clicked()), this, SLOT(informTransErode()));
    connect(ui->okfloodFill, SIGNAL(clicked()), this, SLOT(informTransFloodFill()));
    connect(ui->okPyrup, SIGNAL(clicked()), this, SLOT(informTransPyrUp()));
    connect(ui->okPyrDown, SIGNAL(clicked()), this, SLOT(informTranPyrDown()));
    connect(ui->okThreshold, SIGNAL(clicked()), this, SLOT(informTransThreshold()));
    connect(ui->okResize, SIGNAL(clicked()), this, SLOT(informTrannsResize()));
    connect(ui->okSmooth, SIGNAL(clicked()), this, SLOT(informTransSmooth()));
}

void DialogPro::errorConnecColorChange()
{
    connect(ui->dilateAnchor_xEdit, SIGNAL(textChanged(QString)), this, SLOT(tempDilateErrorColor()));
    connect(ui->dilateAnchor_yEdit, SIGNAL(textChanged(QString)), this, SLOT(tempDilateErrorColor()));
    connect(ui->dilateColsEdit, SIGNAL(textChanged(QString)), this, SLOT(tempDilateErrorColor()));
    connect(ui->dilateRowsEdit, SIGNAL(textChanged(QString)), this, SLOT(tempDilateErrorColor()));

    connect(ui->erodeAnchor_xEdit, SIGNAL(textChanged(QString)), this, SLOT(tempErodeErrorColor()));
    connect(ui->erodeAnchor_yEdit, SIGNAL(textChanged(QString)), this, SLOT(tempErodeErrorColor()));
    connect(ui->erodeColsEdit, SIGNAL(textChanged(QString)), this, SLOT(tempErodeErrorColor()));
    connect(ui->erodeRowsEdit, SIGNAL(textChanged(QString)), this, SLOT(tempErodeErrorColor()));
}
/// ![Signal-Slot]


/// ![temp]
void DialogPro::tempErodeErrorColor()
{
    /*
    if(!isErodeDataOk())
    {
        QPalette p(QColor(Qt::red));
        ui->cancelErode->setPalette(p);
        ui->cancelErode->setAutoFillBackground(true);
    }
    else
    {
        QPalette p(QColor(Qt::white));
        ui->cancelErode->setPalette(p);
        ui->cancelErode->setAutoFillBackground(true);
    }
    */
}

void DialogPro::tempDilateErrorColor()
{
    /*
    if(!isDilateDataOk())
    {
        QPalette p(QColor(Qt::green));
        ui->cancelDilate->setPalette(p);
        ui->cancelDilate->setAutoFillBackground(true);
    }
    else
    {
        QPalette p(QColor(Qt::black));
        ui->cancelDilate->setPalette(p);
        ui->cancelDilate->setAutoFillBackground(true);
    }
    */
}
/// ![temp]


















