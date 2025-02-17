#include "rulesetcreate.h"
#include "ui_rulesetcreate.h"
#include <QDebug>
#include <string>
#include <vector>
#include "filtercreate.h"


rulesetCreate::rulesetCreate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<std::string>)  // Initialize filters
{
    ui->setupUi(this);

    QWidget *filtersWidget = new QWidget();
    filtersLayout = new QVBoxLayout(filtersWidget);
    filtersWidget->setLayout(filtersLayout);
    ui->scrollArea_2->setWidget(filtersWidget);
    ui->scrollArea_2->setWidgetResizable(true);

    filters->push_back("aaaa");
    qDebug() << "First element address:" << QString::fromStdString((*filters)[0]);
    // ui->lineEdit_2->setProperty("source","test");
    connect(ui->add_filter,&QPushButton::clicked,this,&rulesetCreate::addFilters);
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
    // qDebug() << ui->sourceArea->focusWidget()->property("source");
    filtercreate *filt_create_screen = new filtercreate(this);
    filt_create_screen->show();
    this->setEnabled(false);
    filt_create_screen->setEnabled(true);
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

QVBoxLayout *rulesetCreate::getFiltersLayout()
{
    return this->filtersLayout;
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
