#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtPrintSupport/QPrinter>
#include <QMainWindow>
#include "linkPro.h"
#include "linktrans.h"

class QLabel;
class QScrollBar;
class QScrollArea;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // ![0]
    QString imageFile;
    QLabel *imageLabel;
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void updataActions();

    void connectUiToFunction();

    QScrollArea *scrollArea;
    double scaleFactor;
    QPrinter printer;
    // ![0]

    LinkPro *linkpro;
    LinkTrans *linktrans;

private slots:
    void open();
    void save();
    void saveAs();

    void level_set();

    void zoomIn();
    void zoomOut();
    void normlSize();
    void fitToWindow();
    void print();
    void about();
};

#endif // MAINWINDOW_H
