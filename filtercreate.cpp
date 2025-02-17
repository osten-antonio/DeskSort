#include "filtercreate.h"
#include "ui_filtercreate.h"

filtercreate::filtercreate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::filtercreate)
{
    ui->setupUi(this);
}

filtercreate::~filtercreate()
{
    delete ui;
}
