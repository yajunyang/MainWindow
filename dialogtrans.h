#ifndef DIALOGTRANS_H
#define DIALOGTRANS_H

#include <QDialog>
#include "InterFaceTrans.h"

namespace Ui {
class DialogTrans;
}

class DialogTrans : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTrans(QWidget *parent = 0);
    ~DialogTrans();

    // ![0]
    McvImgTransJuanji visitCurrentJuanji(); /// 1
    McvImgTransSobel visitCurrentSobel();   /// 2
    McvImgTransSchar visitCurrentSchar();   /// 3
    McvImgTransLaplace visitCurrentLaplace();  /// 4
    McvImgTransCanny visitCurrentCanny(); /// 5
    McvImgtransHoughLine visitCurrentHoughLine(); /// 6
    McvImgTransHoughCircle visitCurrentHoughCircle(); /// 7
    // ![0]

private:
    Ui::DialogTrans *ui;

    void connectSignalToSlot();
    void connectSetParamToDefault();

    /// ![1]
    McvImgTransJuanji mcvJuanji;  /// 1
    McvImgTransSobel mcvSobel;    /// 2
    McvImgTransSchar mcvSchar;    /// 3
    McvImgTransLaplace mcvLaplace; /// 4
    McvImgTransCanny mcvCanny;   /// 5
    McvImgtransHoughLine mcvHoughLine; /// 6
    McvImgTransHoughCircle mcvHoughCircle; /// 7
    /// ![1]

    /// ![2]
    bool isJuanjiDataOk(); /// 1
    bool isSobelDataOk();  /// 2
    bool isScharDataOk(); /// 3
    bool isLaplaceDataOk(); /// 4
    bool isCannyDataDataOk(); /// 5
    bool isHoughLineDataOk(); /// 6
    bool isHoughCircleDataOk(); /// 7
    /// ![2]

    /// ![3]
    void refreshCurrentJuanji();    /// 1
    void refreshCurrentSobel();   /// 2
    void refreshCurrentSchar();   /// 3
    void refreshCurrentLaplace();/// 4
    void refreshCurrentCanny();   /// 5
    void refreshCurrentHoughLine();     /// 6
    void refreshCurrentHoughCircle(); /// 7
    /// ![3]

private slots:
    /// ![4]
    void informTransJuanji(); /// 1
    void informTransSobel();  /// 2
    void informTransSchar();  /// 3
    void informTrannsLaplace();  /// 4
    void informTransCanny(); /// 5
    void informTransHoughLine(); /// 6
    void informTransHoughCircle(); /// 7
    /// ![4]

    /// ![6]
    void setJuanjiParamToDufault(); /// 1
    void setSobelParamToDufault(); /// 2
    void setScharParamToDufault(); /// 3
    void setLaplaceParamToDufault(); /// 4
    void setCannyParamToDufault(); /// 5
    void setHoughLineParamToDufault(); /// 6
    void setHoughCircleParamToDufault(); /// 7
     /// ![6]

signals:
    /// ![5]
    void signalTransJuanji();  /// 1
    void signalTransSobel();   /// 2
    void signalTransSchar();   /// 3
    void signalTransLaplace(); /// 4
    void signalTransCanny();   /// 5
    void signalTransHoughLine(); /// 6
    void signalTransHoughCircle(); /// 7
    /// ![5]
};

#endif // DIALOGTRANS_H
