#ifndef FILTERCREATE_H
#define FILTERCREATE_H

#include <QDialog>
#include "rulesetcreate.h"
#include "clickableframe.h"
#include <QLabel>
#include <QTextEdit>

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
    ClickableFrame* getFrame();
    std::pair<QTextEdit*,QTextEdit*> get_labels();

private:
    Ui::filtercreate *ui;
    rulesetCreate* create_screen;
    void selectFilter();
    ClickableFrame* created_container;
    QTextEdit *type = new QTextEdit();
    QTextEdit *filter = new QTextEdit();
};

#endif // FILTERCREATE_H
