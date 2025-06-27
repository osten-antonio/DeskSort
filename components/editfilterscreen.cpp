#include "./headers/editfilterscreen.h"
#include "ui_editfilterscreen.h"
#include <QPushButton>
#include <qDebug>

editfilterscreen::editfilterscreen(rulesetCreate *creationscreen, QString filter_to_edit, QString type_to_edit,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editfilterscreen), creationscreen(creationscreen)
{
    ui->setupUi(this);
    setFilter(creationscreen->getSelectedFilter());
    setType(creationscreen->getSelectedType());
    qDebug() << "inside";
    ui->lineEdit->setText(filter_to_edit);
    ui->comboBox->setCurrentText(type_to_edit);
    this->creationscreen = creationscreen;
    connect(ui->pushButton,&QPushButton::clicked,this,&editfilterscreen::confirmedit);
}

editfilterscreen::~editfilterscreen()
{
    delete ui;
}

void editfilterscreen::setFilter(QString filter){
    this->filter = filter;
}

void editfilterscreen::setType(QString type){
    this->type = type;
}

QString editfilterscreen::getFilter(){
    return filter;
}

QString editfilterscreen::getType(){
    return type;
}

void editfilterscreen::confirmedit(){
    //add validation later
    setFilter(ui->lineEdit->text());
    setType(ui->comboBox->currentText());
    creationscreen->setSelectedFilter(ui->lineEdit->text());
    creationscreen->setSelectedType(ui->comboBox->currentText());
    this->accept();
}
