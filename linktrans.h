#ifndef LINKTRANS_H
#define LINKTRANS_H

#include <QObject>
#include "dialogtrans.h"
#include "mythread.h"
#include <cv.h>
#include <highgui.h>

const int Max_buffer1 = 100;

#define JUANJI 1
#define SOBEL  2
#define SSCHAR 3
#define LAPLACE 4
#define HOUGHLINE 5
#define HOUGHCIRCLE 6
#define CANNY 7

class LinkTrans : public QObject
{
    Q_OBJECT
public:
    explicit LinkTrans(QObject *parent = 0);
    ~LinkTrans();

    void getOpenImageFile(const QString openImageFile);/// 外界初始化内部数据接口

public slots:
    /// ![0]
    bool transistmJuanji(); /// 1
    bool transistmSobel();  /// 2
    bool transistmSchar(); /// 3
    bool transistmLaplace(); /// 4
    bool transistmCanny(); /// 5
    bool transistmHoughLine(); /// 6
    bool transistmPyrHoughCircle(); /// 7
    /// ![0]
    void showTransDialog();

private:
    /// ![1]
    DialogTrans *dialogTra;

    ushort count_junaji; /// 1
    ushort count_sobel;  /// 2
    ushort count_schar;  /// 3
    ushort count_laplace; /// 4
    ushort count_canny; /// 5
    ushort count_houghline; /// 6
    ushort count_houghcircle; /// 7

    QList<QString> bufferJuanji; /// 1
    QList<QString> bufferSobel; /// 2
    QList<QString> bufferSchar;  /// 3
    QList<QString> bufferLaplace; /// 4
    QList<QString> bufferCanny; /// 5
    QList<QString> bufferHoughLine; /// 6
    QList<QString> bufferHoughCircle; /// 7
    /// ![1]

    bool imageIsEmpty();

    MyThread thread1;
    MyThread thread2;
    MyThread thread3;
    MyThread thread4;
    MyThread thread5;
    MyThread thread6;
    MyThread thread7;
    void startThread(int choice);

    QString generateRode(QString fileName);
    bool generateFile(QString fileRoadandFileName, QString image, ushort &count, QList<QString> &buffer);

    IplImage *imageSrc;
    IplImage *imageDst;

    QString file;
    QString saveFileName;
};

#endif // LINKTRANS_H
