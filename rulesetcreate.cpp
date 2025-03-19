#include "rulesetcreate.h"
#include "ui_rulesetcreate.h"
#include <string>
#include <vector>
#include "filtercreate.h"
#include <algorithm>
#include "editfilterscreen.h"
#include <QMessageBox>
#include <QFileDialog>




rulesetCreate::rulesetCreate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<filterPair>), filters_labels(new std::vector<std::pair<QTextEdit*,QTextEdit*>>)
    , createdframe(new std::vector<ClickableFrame*>), sources(new std::vector<std::string>), source_labels(new std::vector<QTextEdit*>)
    , sourceframe(new std::vector<ClickableFrame*>)

{
    ui->setupUi(this);

    QWidget *filtersWidget = new QWidget();
    filtersLayout = new QVBoxLayout(filtersWidget);
    filtersWidget->setLayout(filtersLayout);
    ui->scrollArea_2->setWidget(filtersWidget);
    ui->scrollArea_2->setWidgetResizable(true);


    QWidget *sourcesWidget = new QWidget();
    sourcesLayout = new QVBoxLayout(sourcesWidget);
    sourcesWidget->setLayout(sourcesLayout);
    ui->sourceArea->setWidget(sourcesWidget);
    ui->sourceArea->setWidgetResizable(true);

    // qDebug() << "First element address:" << QString::fromStdString((*filters)[0]);
    // ui->lineEdit_2->setProperty("source","test");
    connect(ui->add_filter,&QPushButton::clicked,this,&rulesetCreate::addFilters);
    connect(ui->edit_filter,&QPushButton::clicked,this,&rulesetCreate::editFilter);
    connect(ui->delete_filter,&QPushButton::clicked,this,&rulesetCreate::deleteFilter);
    connect(ui->selectDestination,&QPushButton::clicked,this,&rulesetCreate::selectDestination);
    connect(ui->add_source,&QPushButton::clicked,this,&rulesetCreate::addSource);
    connect(ui->edit_source,&QPushButton::clicked,this,&rulesetCreate::editSource);
    connect(ui->delete_source,&QPushButton::clicked,this,&rulesetCreate::deleteSource);
    connect(ui->pushButton,&QPushButton::clicked,this,&rulesetCreate::addEntry);
    // connect(ui->edit_source,)
}

// update
rulesetCreate::rulesetCreate(std::vector<filterPair> *filters_args,std::vector<std::string> *sources_args
                             , std::string destination, QWidget *parent)    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<filterPair>), filters_labels(new std::vector<std::pair<QTextEdit*,QTextEdit*>>)
    , createdframe(new std::vector<ClickableFrame*>), sources(new std::vector<std::string>), source_labels(new std::vector<QTextEdit*>)
    , sourceframe(new std::vector<ClickableFrame*>){

    // convert to c
    char* prev_destination_c = strdup(destination.c_str());
    char** prev_sources_c = new char*[sources_args->size()];
    int count = 1;
    for(int i =0;i<sources_args->size();i++){
        prev_sources_c[i] = strdup(sources_args->at(i).c_str());
    }
    filterPair *prev_filters_c = new filterPair[filters_args->size()];
    for(int i=0;i<filters_args->size();i++){
        prev_filters_c[i].filter=strdup(filters_args->at(i).filter);
        prev_filters_c[i].type=strdup(filters_args->at(i).type);
    }
    entry* prev_entry_arg = new entry;
    prev_entry_arg->source = prev_sources_c;
    prev_entry_arg->source_count=sources_args->size();
    prev_entry_arg->destination=prev_destination_c;
    prev_entry_arg->filters=prev_filters_c;
    prev_entry_arg->filter_count=filters_args->size();

    QPushButton *delete_button = new QPushButton(this);
    delete_button->setText("Delete");
    delete_button->setGeometry(28,390,80,24);

    ui->setupUi(this);
    ui->destinationLabel->setText(QString::fromStdString(destination));
    QWidget *filtersWidget = new QWidget();
    filtersLayout = new QVBoxLayout(filtersWidget);
    filtersWidget->setLayout(filtersLayout);
    ui->scrollArea_2->setWidget(filtersWidget);
    ui->scrollArea_2->setWidgetResizable(true);


    QWidget *sourcesWidget = new QWidget();
    sourcesLayout = new QVBoxLayout(sourcesWidget);
    sourcesWidget->setLayout(sourcesLayout);
    ui->sourceArea->setWidget(sourcesWidget);
    ui->sourceArea->setWidgetResizable(true);

    for(filterPair filter_pair:*filters_args){
        ClickableFrame *container = new ClickableFrame();
        container->setFixedSize(540,30);

        QHBoxLayout *containerLayout = new QHBoxLayout(container);
        containerLayout->setSpacing(0);
        containerLayout->setContentsMargins(0,0,0,0);
        QTextEdit *type = new QTextEdit();
        QTextEdit *filter = new QTextEdit();
        filter->setText(QString::fromStdString(filter_pair.filter));
        filter->setEnabled(false);
        filter->setFixedSize(382,30);
        filter->setAttribute(Qt::WA_TransparentForMouseEvents);
        containerLayout->addWidget(filter);

        type->setText(QString::fromStdString(filter_pair.type));
        type->setFixedSize(160,30);
        type->setEnabled(false);
        type->setAttribute(Qt::WA_TransparentForMouseEvents);
        container->setFilter(QString::fromStdString(filter_pair.filter));
        container->setType(QString::fromStdString(filter_pair.type));
        connect(container,&ClickableFrame::clicked,this,&rulesetCreate::selectFilter);
        containerLayout->addWidget(type);

        filtersLayout->addWidget(container);
        addFilter(QString::fromStdString(filter_pair.filter),QString::fromStdString(filter_pair.type));
        createdframe->push_back(container);
        filters_labels->push_back(std::make_pair(filter,type));
    }

    for(std::string source:*sources_args){
        sources->push_back(source);
        ClickableFrame* container = new ClickableFrame(this);
        container->setFixedSize(540,30);

        QTextEdit* source_label = new QTextEdit(container);
        source_label->setFixedSize(540,30);
        source_label->setText(QString::fromStdString(source));
        source_label->setEnabled(false);
        source_label->setAttribute(Qt::WA_TransparentForMouseEvents);
        std::string source_text = source_label->toPlainText().toStdString();
        container->setSource(QString::fromStdString(source));
        connect(container,&ClickableFrame::clicked,this,&rulesetCreate::selectSource);
        sourceframe->push_back(container);
        source_labels->push_back(source_label);
        sourcesLayout->addWidget(container);
    }

    ui->pushButton->setText("Edit");
    connect(ui->add_filter,&QPushButton::clicked,this,&rulesetCreate::addFilters);
    connect(ui->edit_filter,&QPushButton::clicked,this,&rulesetCreate::editFilter);
    connect(ui->delete_filter,&QPushButton::clicked,this,&rulesetCreate::deleteFilter);
    connect(ui->selectDestination,&QPushButton::clicked,this,&rulesetCreate::selectDestination);
    connect(ui->add_source,&QPushButton::clicked,this,&rulesetCreate::addSource);
    connect(ui->edit_source,&QPushButton::clicked,this,&rulesetCreate::editSource);
    connect(ui->delete_source,&QPushButton::clicked,this,&rulesetCreate::deleteSource);
    connect(ui->pushButton,&QPushButton::clicked,this,[this,prev_entry_arg](){
        editEntry(prev_entry_arg);
        this->deleteLater();
    });
    connect(delete_button,&QPushButton::clicked,this,[this,prev_entry_arg](){\
        QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete?",
                                    QMessageBox::Yes|QMessageBox::No);
        if(confirm == QMessageBox::Yes){
            int res = delete_entry(prev_entry_arg);
            this->deleteLater();
        }
    });
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
    for(int i = 0;i<filters_labels->size();i++){
        if(filters_labels->at(i) == filt_create_screen->get_labels()){ // fix here
            QMessageBox msgBox;
            msgBox.critical(0,"Error","Duplicate filter");
            break;
        }
    }
    filters_labels->push_back(filt_create_screen->get_labels());
    createdframe->push_back(filt_create_screen->getFrame());
}

void rulesetCreate::addSource(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty()){
        auto search_pointer = std::find(sources->begin(), sources->end(), dir.toStdString());
        if(dir.toStdString()==destination){
            QMessageBox msgBox;
            msgBox.critical(0,"Error","Source and destination is the same");
        }else{
            if(search_pointer == sources->end()){
                sources->push_back(dir.toStdString());
                ClickableFrame* container = new ClickableFrame(this);
                container->setFixedSize(540,30);

                QTextEdit* source_label = new QTextEdit(container);
                source_label->setFixedSize(540,30);
                source_label->setText(dir);
                source_label->setEnabled(false);
                source_label->setAttribute(Qt::WA_TransparentForMouseEvents);
                std::string source_text = source_label->toPlainText().toStdString();
                container->setSource(dir);
                connect(container,&ClickableFrame::clicked,this,&rulesetCreate::selectSource);
                sourceframe->push_back(container);
                source_labels->push_back(source_label);
                sourcesLayout->addWidget(container);
            }
        }
    }
}

void rulesetCreate::selectSource(){
    ClickableFrame *clickedFrame = qobject_cast<ClickableFrame *>(sender());
    selected_source=clickedFrame->getSource().toStdString();
}

void rulesetCreate::editEntry(entry* prev_entry) {
    char* destination_c = strdup(ui->destinationLabel->text().toStdString().c_str());
    char** sources_c = new char*[sources->size()];
    int count = 1;
    for(int i =0;i<sources->size();i++){
        sources_c[i] = strdup(sources->at(i).c_str());
    }
    filterPair *filters_c = new filterPair[filters->size()];
    for(int i=0;i<filters->size();i++){
        filters_c[i].filter=strdup(filters->at(i).filter);
        filters_c[i].type=strdup(filters->at(i).type);
    }
    entry* entry_arg = new entry;
    entry_arg->source = sources_c;
    entry_arg->source_count=sources->size();
    entry_arg->destination=destination_c;
    entry_arg->filters=filters_c;
    entry_arg->filter_count=filters->size();
    int res = update_entry(entry_arg,prev_entry);
    QMessageBox msgBox;
    switch(res){
        case -30:
            msgBox.critical(0,"Error","Filter is empty");
            break;
        case -40:
            msgBox.critical(0,"Error","Source is empty");
            break;
        case -50:
            msgBox.critical(0,"Error","Destination is empty");
            break;
        case -200:
            msgBox.critical(0,"Error","Destination already exist");
            break;
        case 0:
            this->destroy();
            break;
        default:
           msgBox.critical(0,"Error","Database error");
           break;
    }

}


void rulesetCreate::addEntry() {
    // Convert C++ datatype to C ffs
    /* std::vector<std::pair<std::string,std::string>>* filters;
     * std::vector<std::string>* sources;
     * std::string destination;
    */
    // write_entry()

    char* destination_c = strdup(destination.c_str());
    char** sources_c = new char*[sources->size()];
    int count = 1;
    for(int i =0;i<sources->size();i++){
        sources_c[i] = strdup(sources->at(i).c_str());
    }
    filterPair *filters_c = new filterPair[filters->size()];
    for(int i=0;i<filters->size();i++){
        filters_c[i].filter=strdup(filters->at(i).filter);
        filters_c[i].type=strdup(filters->at(i).type);
    }
    entry* entry_arg = new entry;
    entry_arg->source = sources_c;
    entry_arg->source_count=sources->size();
    entry_arg->destination=destination_c;
    entry_arg->filters=filters_c;
    entry_arg->filter_count=filters->size();
    int res = write_entry(entry_arg,false);
    QMessageBox msgBox;
    switch(res){
        case -30:
            msgBox.critical(0,"Error","Filter is empty");
            break;
        case -40:
            msgBox.critical(0,"Error","Source is empty");
            break;
        case -50:
            msgBox.critical(0,"Error","Destination is empty");
            break;
        case -200:
            msgBox.critical(0,"Error","Destination already exist");
            break;
        case 0:
            this->destroy();
            break;
        default:
            msgBox.critical(0,"Error","Database error");
            break;
    }
    this->deleteLater();
}




void rulesetCreate::editFilter(){
    int i;
    for(i =0; i < filters->size();i++){
        if (strcmp((*filters)[i].filter, selected_filter.toStdString().c_str()) == 0 &&
            strcmp((*filters)[i].type, selected_type.toStdString().c_str()) == 0) {
            break;
        }
    }
    if (i<filters->size()) {
        editfilterscreen *screen = new editfilterscreen(this, selected_filter, selected_type);
        screen->exec();
        free((*filters)[i].filter);
        free((*filters)[i].type);
        filterPair filter;
        filter.filter = strdup(screen->getFilter().toStdString().c_str());
        filter.type = strdup(screen->getType().toStdString().c_str());
        (*filters)[i] = filter;
        (*filters_labels)[i].first->setText(screen->getFilter());
        (*filters_labels)[i].second->setText(screen->getType());
        (*createdframe)[i]->setFilter(screen->getFilter());
        (*createdframe)[i]->setType(screen->getType());

    }
}

void rulesetCreate::editSource(){
    auto search_pointer = std::find(sources->begin(), sources->end(), selected_source);
    if (search_pointer != sources->end()) {
        int index = std::distance(sources->begin(),search_pointer);
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        (*sources)[index] = dir.toStdString();
        (*source_labels)[index]->setText(dir);
        (*sourceframe)[index]->setSource(dir);
    }
}


void rulesetCreate::selectDestination(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    auto search_pointer = std::find(sources->begin(), sources->end(), dir.toStdString());
    if(search_pointer != sources->end()){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Destination and source is the same");
    }else{
        if(!dir.isEmpty()){
            ui->destinationLabel->setText(dir);
            destination = dir.toStdString();
        }
    }
}

void rulesetCreate::deleteSource(){
    auto search_pointer = std::find(sources->begin(), sources->end(), selected_source);
    if (search_pointer != sources->end()) {
        QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete?",
                                                                    QMessageBox::Yes|QMessageBox::No);
        if(confirm == QMessageBox::Yes){
            int index = std::distance(sources->begin(), search_pointer);
            if (index < sources->size()) {
                (*sourceframe)[index]->deleteLater();
                (*sources).erase((*sources).begin() + index);
                (*source_labels).erase((*source_labels).begin()+index);
                (*sourceframe).erase((*sourceframe).begin()+index);
            }
        }
    }
}

void rulesetCreate::deleteFilter(){
    int i;
    for(i =0; i < filters->size();i++){
        if (strcmp((*filters)[i].filter, selected_filter.toStdString().c_str()) == 0 &&
            strcmp((*filters)[i].type, selected_type.toStdString().c_str()) == 0) {
            break;
        }
    }

    if (i < filters->size()) {
        QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete?",
                                      QMessageBox::Yes|QMessageBox::No);
        if(confirm == QMessageBox::Yes){


            (*createdframe)[i]->deleteLater();
            (*filters).erase((*filters).begin() + i);
            (*filters_labels).erase((*filters_labels).begin()+i);
            (*createdframe).erase((*createdframe).begin()+i);

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
    filterPair temp;
    temp.filter = strdup(filter.toStdString().c_str());
    temp.type = strdup(type.toStdString().c_str());
    filters->push_back(temp);
}

void rulesetCreate::selectFilter(){
    ClickableFrame *clickedFrame = qobject_cast<ClickableFrame *>(sender());
    this->setSelectedFilter(clickedFrame->getFilter());
    this->setSelectedType(clickedFrame->getType());
}

