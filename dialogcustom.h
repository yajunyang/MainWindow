#ifndef DIALOGCUSTOM_H
#define DIALOGCUSTOM_H

#include <QDialog>


namespace Ui {
class DialogCustom;
}

class DialogCustom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustom(QWidget *parent = 0);
    ~DialogCustom();

private:
    Ui::DialogCustom *ui;
};

#endif // DIALOGCUSTOM_H
