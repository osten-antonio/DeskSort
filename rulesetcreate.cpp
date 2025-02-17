#include "rulesetcreate.h"
#include "ui_rulesetcreate.h"
#include <QDebug>
#include<string>
#include <vector>

rulesetCreate::rulesetCreate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<std::string>)  // Initialize filters
{
    ui->setupUi(this);


    filters->push_back("aaaa");
    qDebug() << "First element address:" << QString::fromStdString((*filters)[0]);
    ui->lineEdit_2->setProperty("source","test");
    connect(ui->add_source,&QPushButton::clicked,this,&rulesetCreate::addFilters);
}


rulesetCreate::~rulesetCreate()
{
    if (filters) {
        delete filters;
        filters = nullptr; // Set it to null to avoid accidental use after deletion
    }
    delete ui;
}

void rulesetCreate::addFilters(){
    qDebug() << ui->sourceArea->focusWidget()->property("source");
}

void rulesetCreate::addSource(){

}

void rulesetCreate::addEntry(){

}

void rulesetCreate::editFilter(){

}

void rulesetCreate::selectDestination(){

}

void rulesetCreate::deleteSource(){

}

void rulesetCreate::deleteFilter(){

}




/*    void addFilters();
    void addSource();
    void editFilter();
    void editSource();
    void selectDestination();
    void deleteSource();
    void deleteFilter();
    void addEntry();
*/
