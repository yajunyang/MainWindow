#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T21:47:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainWindow
TEMPLATE = app
QT += printsupport


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogpro.cpp \
    dialogtrans.cpp \
    linkPro.cpp \
    mythread.cpp \
    linktrans.cpp \
    dialogcustom.cpp \
    linkcustom.cpp \
    dialogvideo.cpp

HEADERS  += mainwindow.h \
    dialogpro.h \
    dialogtrans.h \
    InterFacePro.h \
    InterFaceTrans.h \
    linkPro.h \
    mythread.h \
    linktrans.h \
    dialogcustom.h \
    linkcustom.h \
    dialogvideo.h

FORMS    += mainwindow.ui \
    dialogpro.ui \
    dialogtrans.ui \
    dialogcustom.ui \
    dialogvideo.ui



INCLUDEPATH += D:\Program\opencv\build\include\opencv\
               D:\Program\opencv\build\include\opencv2\
               D:\Program\opencv\build\include

LIBS += -LD:\Program\opencv\build\x86\vc10\lib\
        -lopencv_core247 \
        -lopencv_highgui247 \
        -lopencv_imgproc247 \
        -lopencv_features2d247 \
        -lopencv_calib3d247


INCLUDEPATH += D:\Program\osg\include

LIBS += D:\Program\osg\lib\osgViewerd.lib\
        D:\Program\osg\lib\osgDBd.lib\
        D:\Program\osg\lib\OpenThreadsd.lib\
        D:\Program\osg\lib\osgd.lib\
        D:\Program\osg\lib\osgGAd.lib
