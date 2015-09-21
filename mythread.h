#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QCoreApplication>
#include <QString>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Image>
#include <osg/Texture2D>

#include <QtGui>


class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void run();
    void updataFilename(QString file);
signals:

public slots:

private:
    osg::ref_ptr<osg::Node> CraetePlane(QString fileName);
    QString filename;
};

#endif // MYTHREAD_H
