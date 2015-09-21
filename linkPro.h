#ifndef LINK_H
#define LINK_H

#define SMOOTH 1
#define DILATE 2
#define ERODE  3
#define RSIZE 4
#define FLOODFILL 5
#define PYRDOWN 6
#define PYRUP 7
#define THRESHOLD 8

#include <QObject>
#include "dialogpro.h"
#include "mythread.h"
#include <highgui.h>
#include <cv.h>

const int Max_buffer = 100;

class LinkPro : public QObject
{
    Q_OBJECT
public:
    explicit LinkPro(QObject *parent = 0);
    ~LinkPro();
    void getOpenImageFile(const QString openImageFile);/// 外界初始化内部数据接口
//signals:
//    void informThreadShowSmooth();
//    void informThreadShowilate();
//    void informThreadShowErode();
//    void informThreadShowResize();
//    void informThreadShowFloodFill();
//    void informThreadShowThreshold();
//    void informThreadShowPyrdown();
//    void informThreadShowPyrup();

public slots:
    /// ![0]
    bool transistmSmooth();
    bool transistmDilate();
    bool transistmErode();
    bool transistmResize();
    bool transistmFloodFill();
    bool transistmThreshold();
    bool transistmPyrDown();
    bool transistmPyrUp();
    /// ![0]
private slots:

//    void ThreadShowSmooth();
//    void ThreadShowilate();
//    void ThreadShowErode();
//    void ThreadShowResize();
//    void ThreadShowFloodFill();
//    void ThreadShowThreshold();
//    void ThreadShowPyrdown();
//    void ThreadShowPyrup();

    void showProDialog();

private:
    DialogPro *dialogPro;

    ushort count_smooth;  /// 1
    ushort count_dilate;  /// 2
    ushort count_erode;   /// 3
    ushort count_resize;  /// 4
    ushort count_floodfill; /// 5
    ushort count_threshold; /// 6
    ushort count_pyrdown; /// 7
    ushort count_pyrup; /// 8

    QList<QString> bufferSmooth; /// 1
    QList<QString> bufferDilate; /// 2
    QList<QString> bufferErode;  /// 3
    QList<QString> bufferResize; /// 4
    QList<QString> bufferFloodFill; /// 5
    QList<QString> bufferThreshold; /// 6
    QList<QString> bufferPyrDown; /// 7
    QList<QString> bufferPyrUp; /// 8

    bool imageIsEmpty();

    MyThread thread1;
    MyThread thread2;
    MyThread thread3;
    MyThread thread4;
    MyThread thread5;
    MyThread thread6;
    MyThread thread7;
    MyThread thread8;
    void startThread(int choice);

    QString generateRode(QString fileName);
    bool generateFile(QString fileRoadandFileName, QString image, ushort &count, QList<QString> &buffer);

    IplImage *imageSrc;
    IplImage *imageDst;
    IplConvKernel *B1;
    IplConvKernel *B2;
    QString file;

    QString saveFileName;
};

#endif // LINK_H
