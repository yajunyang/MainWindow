#ifndef DIALOGPRO_H
#define DIALOGPRO_H

#include <QDialog>
#include "InterFacePro.h"

class QLineEdit;

namespace Ui {
class DialogPro;
}

class DialogPro : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPro(QWidget *parent = 0);
    ~DialogPro();

    /// ![0]
    McvImgProErode visitCurrentErode();
    McvImgProDilate visitCurrentDilate();
    MCvImgProSmooth visitCurrentSmooth();
    MCvImgProFloodFill visitCurrentFloodFill();
    MCvImgProThresHold visitCurrentThresHold();
    McvImgProResize visitCurrentResize();
    McvImgProPyup visitCurrentPyup();
    McvImgProPydown visitCurrentPydown();
    /// ![0]

private:
    Ui::DialogPro *ui;

    void setLimit();

    /// ![00]
    McvImgProErode mcvErode;
    McvImgProDilate mcvDilate;
    MCvImgProSmooth mcvSmooth;
    MCvImgProFloodFill mcvFloodFill;
    MCvImgProThresHold mcvThresHold;
    McvImgProResize mcvResize;
    McvImgProPyup mcvPyup;
    McvImgProPydown mcvPydown;

    QDialog *dialogNewVal;
    QLineEdit *red;
    QLineEdit *green;
    QLineEdit *blue;
    QPushButton *okDialogNewVal;
    QPushButton *cancelDialogNewVal;

    QDialog *dialogUpDiff;
    QLineEdit *up1;
    QLineEdit *up2;
    QLineEdit *up3;
    QPushButton *okDialogUpDiff;
    QPushButton *cancelDialoUpDiff;

    QDialog *dialogLowDiff;
    QLineEdit *lo1;
    QLineEdit *lo2;
    QLineEdit *lo3;
    QPushButton *okDialogLowDiff;
    QPushButton *cancelDialogLowDiff;

    bool isSmoothDataOk();
    bool isErodeDataOk();
    bool isDilateDataOk();
    bool isFloodFillDataOk();
    bool isThresHoldDataOk();

    void refreshCurrentErode();    // 1
    void refreshCurrentDilate();   // 2
    void refreshCurrentSmooth();   // 3
    void refreshCurrentThresHold();// 5
    void refreshCurrentResize();   // 6
    void refreshCurrentPyup();     // 7
    void refreshCurrentPydown();   // 8

    void setDialogNewVal();
    void setDialogUpDiff();
    void setDialogLowDiff();


    void connectOkandCancel();
    void errorConnecColorChange();

private slots:
    void refreshCurrentFloodFill();// 4
    /// ![00]


    /// ![1]
    void setSmoothParamToDefault();
    void setDilateParamToDefault();
    void setErodeParamToDefault();
    void setThresholdParamToDefault();
    void setFloodFillParamToDefault();
    /// ![1]


    /// ![2]
    void informTransSmooth();
    void informTransDilate();
    void informTransErode();
    void informTrannsResize();
    void informTransFloodFill();
    void informTransThreshold();
    void informTranPyrDown();
    void informTransPyrUp();
    /// ![2]

    void tempErodeErrorColor();
    void tempDilateErrorColor();

signals:
    void signalTransSmooth();
    void signalTransDilate();
    void signalTransErode();
    void signalTransResize();
    void signalTransFloodFill();
    void signalTransThreshold();
    void signalTransPyrDown();
    void signalTransPyrUp();
};

#endif // DIALOGPRO_H
