#ifndef FILTERCREATE_H
#define FILTERCREATE_H

#include <QDialog>

namespace Ui {
class filtercreate;
}

class filtercreate : public QDialog
{
    Q_OBJECT

public:
    explicit filtercreate(QWidget *parent = nullptr);
    ~filtercreate();

private:
    Ui::filtercreate *ui;
};

#endif // FILTERCREATE_H
