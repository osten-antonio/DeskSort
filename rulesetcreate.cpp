#include "rulesetcreate.h"
#include "ui_rulesetcreate.h"
#include <string>
#include <vector>
#include "filtercreate.h"
#include <algorithm>
#include "editfilterscreen.h"

rulesetCreate::rulesetCreate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<std::pair<std::string,std::string>>)  // Initialize filters
{
    ui->setupUi(this);

    QWidget *filtersWidget = new QWidget();
    filtersLayout = new QVBoxLayout(filtersWidget);
    filtersWidget->setLayout(filtersLayout);
    ui->scrollArea_2->setWidget(filtersWidget);
    ui->scrollArea_2->setWidgetResizable(true);

    filters->push_back({"aaaa","aaa"});
    // qDebug() << "First element address:" << QString::fromStdString((*filters)[0]);
    // ui->lineEdit_2->setProperty("source","test");
    connect(ui->add_filter,&QPushButton::clicked,this,&rulesetCreate::addFilters);
    connect(ui->edit_filter,&QPushButton::clicked,this,&rulesetCreate::editFilter);
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
    auto search_pointer = std::find(filters->begin(),filters->end(), std::make_pair(selected_filter.toStdString(),selected_type.toStdString()));
    if(search_pointer != filters->end()){
        editfilterscreen *screen = new editfilterscreen(this,selected_filter,selected_type);
        screen->exec();
        int index = std::distance(filters->begin(),search_pointer);
        qDebug() << "outside";
        (*filters)[index]=std::make_pair(screen->getFilter().toStdString(),screen->getType().toStdString());
        qDebug() << filters[index];
    }
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

void rulesetCreate::setSelectedFilter(QString filter){
    selected_filter = filter;
}

void rulesetCreate::setSelectedType(QString type){
    selected_type = type;
}

QString rulesetCreate::getSelectedFilter(){
    return selected_type;
}

QString rulesetCreate::getSelectedType(){
    return selected_type;
}

void rulesetCreate::addFilter(QString filter, QString type){
    filters->push_back(std::make_pair(filter.toStdString(),type.toStdString()));
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
