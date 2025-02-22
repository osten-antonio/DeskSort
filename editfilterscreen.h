#ifndef EDITFILTERSCREEN_H
#define EDITFILTERSCREEN_H

#include <QDialog>
#include "rulesetcreate.h"

namespace Ui {
class editfilterscreen;
}

class editfilterscreen : public QDialog
{
    Q_OBJECT

public:
    explicit editfilterscreen(rulesetCreate *creationscreen, QString filter_to_edit,QString type_to_edit,QWidget *parent = nullptr);
    ~editfilterscreen();
    void confirmedit();
    QString getFilter();
    QString getType();
    void setFilter(QString filter);
    void setType(QString type);
private:
    Ui::editfilterscreen *ui;
    QString filter;
    QString type;
};

#endif // EDITFILTERSCREEN_H
