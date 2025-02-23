#include "rulesetcreate.h"
#include "ui_rulesetcreate.h"
#include <string>
#include <vector>
#include "filtercreate.h"
#include <algorithm>
#include "editfilterscreen.h"
#include <QMessageBox>


rulesetCreate::rulesetCreate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<std::pair<std::string,std::string>>), filters_labels(new std::vector<std::pair<QTextEdit*,QTextEdit*>>)
    , createdframe(new std::vector<ClickableFrame*>)
{
    ui->setupUi(this);

    QWidget *filtersWidget = new QWidget();
    filtersLayout = new QVBoxLayout(filtersWidget);
    filtersWidget->setLayout(filtersLayout);
    ui->scrollArea_2->setWidget(filtersWidget);
    ui->scrollArea_2->setWidgetResizable(true);

    // qDebug() << "First element address:" << QString::fromStdString((*filters)[0]);
    // ui->lineEdit_2->setProperty("source","test");
    connect(ui->add_filter,&QPushButton::clicked,this,&rulesetCreate::addFilters);
    connect(ui->edit_filter,&QPushButton::clicked,this,&rulesetCreate::editFilter);
    connect(ui->delete_filter,&QPushButton::clicked,this,&rulesetCreate::deleteFilter);
}


rulesetCreate::~rulesetCreate()
{
    if (filters) {
        delete filters;
        filters = nullptr;
    }
    delete ui;
}


void rulesetCreate::addFilters(){
    // qDebug() << ui->sourceArea->focusWidget()->property("source");
    filtercreate *filt_create_screen = new filtercreate(this);
    filt_create_screen->exec();
    qDebug() << "aaa";
    filters_labels->push_back(filt_create_screen->get_labels());
    createdframe->push_back(filt_create_screen->getFrame());
}

void rulesetCreate::addSource(){

}

void rulesetCreate::addEntry(){

}

void rulesetCreate::editFilter(){
    auto search_pointer = std::find(filters->begin(), filters->end(), std::make_pair(selected_filter.toStdString(), selected_type.toStdString()));
    if (search_pointer != filters->end()) {
        editfilterscreen *screen = new editfilterscreen(this, selected_filter, selected_type);
        screen->exec();
        int index = std::distance(filters->begin(), search_pointer);
        if (index < filters->size()) {
            (*filters)[index] = std::make_pair(screen->getFilter().toStdString(), screen->getType().toStdString());
            (*filters_labels)[index].first->setText(screen->getFilter());
            (*filters_labels)[index].second->setText(screen->getType());
            (*createdframe)[index]->setFilter(screen->getFilter());
            (*createdframe)[index]->setType(screen->getType());
        }
    }
}


void rulesetCreate::selectDestination(){

}

void rulesetCreate::deleteSource(){

}

void rulesetCreate::deleteFilter(){
    auto search_pointer = std::find(filters->begin(), filters->end(), std::make_pair(selected_filter.toStdString(), selected_type.toStdString()));
    if (search_pointer != filters->end()) {
        QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete?",
                                      QMessageBox::Yes|QMessageBox::No);
        if(confirm == QMessageBox::Yes){
            int index = std::distance(filters->begin(), search_pointer);
            if (index < filters->size()) {
                (*createdframe)[index]->deleteLater();
                (*filters).erase((*filters).begin() + index);
                (*filters_labels).erase((*filters_labels).begin()+index);
                (*createdframe).erase((*createdframe).begin()+index);
            }
        }
    }
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
