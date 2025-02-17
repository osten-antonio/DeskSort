#ifndef FILTERCREATE_H
#define FILTERCREATE_H

#include <QDialog>
#include "rulesetcreate.h"

namespace Ui {
class filtercreate;
}

class filtercreate : public QDialog
{
    Q_OBJECT

public:
    explicit filtercreate(rulesetCreate* create_screen,QWidget *parent = nullptr);
    ~filtercreate();
    void addFilter();

private:
    Ui::filtercreate *ui;
    rulesetCreate* create_screen;
};

#endif // FILTERCREATE_H
