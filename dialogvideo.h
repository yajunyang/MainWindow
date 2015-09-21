#ifndef DIALOGVIDEO_H
#define DIALOGVIDEO_H

#include <QDialog>

namespace Ui {
class DialogVideo;
}

class DialogVideo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVideo(QWidget *parent = 0);
    ~DialogVideo();

private:
    Ui::DialogVideo *ui;
};

#endif // DIALOGVIDEO_H
