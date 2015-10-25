#include "dialogtrans.h"
#include "ui_dialogtrans.h"

DialogTrans::DialogTrans(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTrans)
{
    ui->setupUi(this);

    ui->sobelAperture->setCurrentIndex(1);

    connectSignalToSlot();
    connectSetParamToDefault();
}

DialogTrans::~DialogTrans()
{
    delete ui;
}

/// ![0]
McvImgTransJuanji DialogTrans::visitCurrentJuanji() /// 1
{
    return mcvJuanji;
}

McvImgTransSobel DialogTrans::visitCurrentSobel()  /// 2
{
    return mcvSobel;
}

McvImgTransSchar DialogTrans::visitCurrentSchar()  /// 3
{
    return mcvSchar;
}

McvImgTransLaplace DialogTrans::visitCurrentLaplace() /// 4
{
    return mcvLaplace;
}

McvImgTransCanny DialogTrans::visitCurrentCanny() /// 5
{
    return mcvCanny;
}

McvImgtransHoughLine DialogTrans::visitCurrentHoughLine() /// 6
{
    return mcvHoughLine;
}

McvImgTransHoughCircle DialogTrans::visitCurrentHoughCircle() /// 7
{
    return mcvHoughCircle;
}
/// ![0]


/// ![4]
void DialogTrans::informTransJuanji() /// 1
{
    refreshCurrentJuanji();
    if(isJuanjiDataOk())
        emit signalTransJuanji();
    else return;
}

void DialogTrans::informTransSobel()  /// 2
{
    refreshCurrentSobel();
    if(isSobelDataOk())
        emit signalTransSobel();
    else return;
}

void DialogTrans::informTransSchar()  /// 3
{
    refreshCurrentSchar();
    if(isScharDataOk())
        emit signalTransSchar();
    else return;
}

void DialogTrans::informTrannsLaplace()  /// 4
{
    refreshCurrentLaplace();
    if(isLaplaceDataOk())
        emit signalTransLaplace();
    else return;
}

void DialogTrans::informTransCanny() /// 5
{
    refreshCurrentCanny();
    if(isCannyDataDataOk())
        emit signalTransCanny();
    else return;
}

void DialogTrans::informTransHoughLine() /// 6
{
    refreshCurrentHoughLine();
    if(isHoughLineDataOk())
        emit signalTransHoughLine();
    else return;
}

void DialogTrans::informTransHoughCircle() /// 7
{
    refreshCurrentHoughCircle();
    if(isHoughCircleDataOk())
        emit signalTransHoughCircle();
    else return;
}
/// ![4]


/// ![2]
bool DialogTrans::isJuanjiDataOk()/// 1
{
    bool iskernelEmpty = (ui->kernel0_0->text().isEmpty() |
            ui->kernel0_1->text().isEmpty() |
            ui->kernel0_2->text().isEmpty() |
            ui->kernel1_0->text().isEmpty() |
            ui->kernel1_1->text().isEmpty() |
            ui->kernel1_2->text().isEmpty() |
            ui->kernel2_0->text().isEmpty() |
            ui->kernel2_1->text().isEmpty() |
            ui->kernel2_2->text().isEmpty() |
            ui->kernel_x->text().isEmpty()  |
            ui->kernel_y->text().isEmpty());
    if(iskernelEmpty) return false;
    else return true;
}

bool DialogTrans::isSobelDataOk() /// 2
{
    return true;
}

bool DialogTrans::isScharDataOk()/// 3
{
    bool isScharEmpty = (ui->scharrKernel0_0->text().isEmpty() |
            ui->scharrKernel0_1->text().isEmpty() |
            ui->scharrKernel0_2->text().isEmpty() |
            ui->scharrKernel1_0->text().isEmpty() |
            ui->scharrKernel1_1->text().isEmpty() |
            ui->scharrKernel1_2->text().isEmpty() |
            ui->scharrKernel2_0->text().isEmpty() |
            ui->scharrKernel2_1->text().isEmpty() |
            ui->scharrKernel2_2->text().isEmpty() );
    if(isScharEmpty) return false;
    else return true;
}

bool DialogTrans::isLaplaceDataOk()/// 4
{
    return true;
}

bool DialogTrans::isCannyDataDataOk()/// 5
{
    bool isCannyEmpty = ui->cannyHighThresh->text().isEmpty() |
            ui->cannyLowThresh->text().isEmpty();
    if(isCannyEmpty) return false;
    else return true;
}

bool DialogTrans::isHoughLineDataOk()/// 6
{
    bool isHoughLineEmpty = ui->houghLineRho->text().isEmpty() |
            ui->houghLineTheta->text().isEmpty() |
            ui->houghLineThreshold->text().isEmpty() |
            ui->houghLineParam1->text().isEmpty() |
            ui->houghLineParam2->text().isEmpty();
    if(isHoughLineEmpty)
        return false;
    else return true;
}

bool DialogTrans::isHoughCircleDataOk()/// 7
{
    bool isHoughCircleEmpty = (ui->houghCircleDp->text().isEmpty() |
            ui->houghCircleMin_dst->text().isEmpty() |
            ui->houghCircleParam1->text().isEmpty()  |
            ui->houghCircleParam2->text().isEmpty()  |
            ui->houghCircleMin_radius->text().isEmpty() |
            ui->houghCircleMax_radius->text().isEmpty());
    if(isHoughCircleEmpty)
        return false;
    else return true;
}
/// ![2]

/// ![3]
void DialogTrans::refreshCurrentJuanji()    /// 1
{
    mcvJuanji.kernel = cvCreateMat(3, 3, CV_64FC1);
    cvZero(mcvJuanji.kernel);

    double i0j0 = ui->kernel0_0->text().toDouble();
    double i0j1 = ui->kernel0_1->text().toDouble();
    double i0j2 = ui->kernel0_2->text().toDouble();

    double i1j0 = ui->kernel1_0->text().toDouble();
    double i1j1 = ui->kernel1_1->text().toDouble();
    double i1j2 = ui->kernel1_2->text().toDouble();

    double i2j0 = ui->kernel2_0->text().toDouble();
    double i2j1 = ui->kernel2_1->text().toDouble();
    double i2j2 = ui->kernel2_2->text().toDouble();

    cvmSet(mcvJuanji.kernel, 0, 0, i0j0);
    cvmSet(mcvJuanji.kernel, 0, 1, i0j1);
    cvmSet(mcvJuanji.kernel, 0, 2, i0j2);

    cvmSet(mcvJuanji.kernel, 1, 0, i1j0);
    cvmSet(mcvJuanji.kernel, 1, 1, i1j1);
    cvmSet(mcvJuanji.kernel, 1, 2, i1j2);

    cvmSet(mcvJuanji.kernel, 2, 0, i2j0);
    cvmSet(mcvJuanji.kernel, 2, 1, i2j1);
    cvmSet(mcvJuanji.kernel, 2, 2, i2j2);

    mcvJuanji.ancher.x = ui->kernel_x->text().toInt();
    mcvJuanji.ancher.y = ui->kernel_y->text().toInt();
}

void DialogTrans::refreshCurrentSobel()   /// 2
{
    mcvSobel.xorder = ui->sobelX->currentIndex();
    mcvSobel.yorder = ui->sobelY->currentIndex();

    mcvSobel.aperture_size = (ui->sobelAperture->currentIndex() * 2 + 1);
}

void DialogTrans::refreshCurrentSchar()  /// 3
{
    mcvSchar.kernel = cvCreateMat(3, 3, CV_64FC1);
    cvZero(mcvSchar.kernel);

    double i0j0 = ui->scharrKernel0_0->text().toDouble();
    double i0j1 = ui->scharrKernel0_1->text().toDouble();
    double i0j2 = ui->scharrKernel0_2->text().toDouble();

    double i1j0 = ui->scharrKernel1_0->text().toDouble();
    double i1j1 = ui->scharrKernel1_1->text().toDouble();
    double i1j2 = ui->scharrKernel1_2->text().toDouble();

    double i2j0 = ui->scharrKernel2_0->text().toDouble();
    double i2j1 = ui->scharrKernel2_1->text().toDouble();
    double i2j2 = ui->scharrKernel2_2->text().toDouble();

    cvmSet(mcvSchar.kernel, 0, 0, i0j0);
    cvmSet(mcvSchar.kernel, 0, 1, i0j1);
    cvmSet(mcvSchar.kernel, 0, 2, i0j2);

    cvmSet(mcvSchar.kernel, 1, 0, i1j0);
    cvmSet(mcvSchar.kernel, 1, 1, i1j1);
    cvmSet(mcvSchar.kernel, 1, 2, i1j2);

    cvmSet(mcvSchar.kernel, 2, 0, i2j0);
    cvmSet(mcvSchar.kernel, 2, 1, i2j1);
    cvmSet(mcvSchar.kernel, 2, 2, i2j2);
}

void DialogTrans::refreshCurrentLaplace() /// 4
{
    mcvLaplace.apertureSize = (ui->laplaceAperture->currentIndex()) * 2 + 1;
}

void DialogTrans::refreshCurrentCanny()  /// 5
{
    mcvCanny.highThresh = ui->cannyHighThresh->text().toDouble();
    mcvCanny.lowThresh = ui->cannyLowThresh->text().toDouble();
    mcvCanny.apertureSize = (ui->cannyAperture->currentIndex() * 2 + 1);
}

void DialogTrans::refreshCurrentHoughLine()  /// 6
{
    int method = ui->houghLineMethod->currentIndex();
    switch(method)
    {
    case 0:
        mcvHoughLine.method = CV_HOUGH_STANDARD;
        break;
    case 1:
        mcvHoughLine.method = CV_HOUGH_PROBABILISTIC;
        break;
    default:
        mcvHoughLine.method = CV_HOUGH_MULTI_SCALE;
    }

    mcvHoughLine.rho = ui->houghLineRho->text().toDouble();
    mcvHoughLine.theta = ui->houghLineTheta->text().toDouble();
    mcvHoughLine.threshold = ui->houghLineThreshold->text().toInt();
    mcvHoughLine.param1 = ui->houghLineParam1->text().toDouble();
    mcvHoughLine.param2 = ui->houghLineParam2->text().toDouble();
}

void DialogTrans::refreshCurrentHoughCircle()  /// 7
{
  mcvHoughCircle.method = CV_HOUGH_GRADIENT;
  mcvHoughCircle.dp = ui->houghCircleDp->text().toDouble();
  mcvHoughCircle.min_dist =  ui->houghCircleMin_dst->text().toDouble();
  mcvHoughCircle.param1 = ui->houghCircleParam1->text().toDouble();
  mcvHoughCircle.param2 = ui->houghCircleParam2->text().toDouble();
  mcvHoughCircle.min_radius = ui->houghCircleMin_radius->text().toInt();
  mcvHoughCircle.max_radius = ui->houghCircleMax_radius->text().toInt();
}
/// ![3]


/// ![6]
void DialogTrans::setJuanjiParamToDufault() /// 1
{
    ui->kernel0_0->setText("1");
    ui->kernel0_1->setText("-2");
    ui->kernel0_2->setText("1");

    ui->kernel1_0->setText("2");
    ui->kernel1_1->setText("-4");
    ui->kernel1_2->setText("2");

    ui->kernel2_0->setText("1");
    ui->kernel2_1->setText("-2");
    ui->kernel2_2->setText("1");

    ui->kernel_x->setText("-1");
    ui->kernel_y->setText("-1");
}

void DialogTrans::setSobelParamToDufault() /// 2
{
    ui->sobelX->setCurrentIndex(1);
    ui->sobelY->setCurrentIndex(1);
    ui->sobelAperture->setCurrentIndex(1);
}

void DialogTrans::setScharParamToDufault() /// 3
{
    ui->scharrKernel0_0->setText("-3");
    ui->scharrKernel0_1->setText("0");
    ui->scharrKernel0_2->setText("3");

    ui->scharrKernel1_0->setText("-10");
    ui->scharrKernel1_1->setText("0");
    ui->scharrKernel1_2->setText("10");

    ui->scharrKernel2_0->setText("-3");
    ui->scharrKernel2_1->setText("0");
    ui->scharrKernel2_2->setText("3");
}

void DialogTrans::setLaplaceParamToDufault() /// 4
{
    ui->laplaceAperture->setCurrentIndex(1);
}

void DialogTrans::setCannyParamToDufault() /// 5
{
    ui->cannyLowThresh->setText("60.0");
    ui->cannyHighThresh->setText("150.0");
    ui->cannyAperture->setCurrentIndex(1);
}

void DialogTrans::setHoughLineParamToDufault() /// 6
{
    ui->houghLineMethod->setCurrentIndex(0);
    ui->houghLineRho->setText("1.0");
    ui->houghLineTheta->setText("0.1");
    ui->houghLineThreshold->setText("100");
    ui->houghLineParam1->setText("0");
    ui->houghLineParam2->setText("0");
}

void DialogTrans::setHoughCircleParamToDufault() /// 7
{
    ui->houghCircleMethod->setCurrentIndex(0);
    ui->houghCircleDp->setText("1.5");
    ui->houghCircleMin_dst->setText("2.0");
    ui->houghLineParam1->setText("100.0");
    ui->houghCircleParam2->setText("300.0");
    ui->houghCircleMin_radius->setText("0");
    ui->houghCircleMax_radius->setText("0");
}
/// ![6]


void DialogTrans::connectSignalToSlot()
{
    connect(ui->okJuanji, SIGNAL(clicked()), this, SLOT(informTransJuanji()));
    connect(ui->okSobel, SIGNAL(clicked()), this, SLOT(informTransSobel()));
    connect(ui->okScharr, SIGNAL(clicked()), this, SLOT(informTransSchar()));
    connect(ui->okCanny, SIGNAL(clicked()), this, SLOT(informTransCanny()));
    connect(ui->okHoughLine, SIGNAL(clicked()), this, SLOT(informTransHoughLine()));
    connect(ui->okHoughCircle, SIGNAL(clicked()), this, SLOT(informTransHoughCircle()));
    connect(ui->cancelLaplace, SIGNAL(clicked()), this, SLOT(informTrannsLaplace()));
}

void DialogTrans::connectSetParamToDefault()
{
    connect(ui->cancelJuanji, SIGNAL(clicked()), this, SLOT(setJuanjiParamToDufault()));
    connect(ui->cancelSobel, SIGNAL(clicked()), this, SLOT(setSobelParamToDufault()));
    connect(ui->cancelScharr, SIGNAL(clicked()), this, SLOT(setScharParamToDufault()));
    connect(ui->cancelLaplace, SIGNAL(clicked()), this, SLOT(setLaplaceParamToDufault()));
    connect(ui->cancelHoughLine, SIGNAL(clicked()), this, SLOT(setHoughLineParamToDufault()));
    connect(ui->cancelHoughCircle, SIGNAL(clicked()), this, SLOT(setHoughCircleParamToDufault()));
    connect(ui->cancelCanny, SIGNAL(clicked()), this, SLOT(setCannyParamToDufault()));
}
