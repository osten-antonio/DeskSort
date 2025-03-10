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
    , filters(new std::vector<std::pair<std::string,std::string>>), filters_labels(new std::vector<std::pair<QTextEdit*,QTextEdit*>>)
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
rulesetCreate::rulesetCreate(std::vector<std::pair<std::string,std::string>> *filters_args,std::vector<std::string> *sources_args
                             , std::string destination, QWidget *parent)    : QMainWindow(parent)
    , ui(new Ui::rulesetCreate)
    , filters(new std::vector<std::pair<std::string,std::string>>), filters_labels(new std::vector<std::pair<QTextEdit*,QTextEdit*>>)
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
        prev_filters_c[i].filter=strdup(filters_args->at(i).first.c_str());
        prev_filters_c[i].type=strdup(filters_args->at(i).second.c_str());
    }
    entry* prev_entry_arg;
    prev_entry_arg->source = prev_sources_c;
    prev_entry_arg->source_count=sources_args->size();
    prev_entry_arg->destination=prev_destination_c;
    prev_entry_arg->filters=prev_filters_c;
    prev_entry_arg->filter_count=filters_args->size();


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

    for(std::pair<std::string,std::string> filter_pair:*filters_args){ // test this later pls
        ClickableFrame *container = new ClickableFrame();
        container->setFixedSize(540,30);

        QHBoxLayout *containerLayout = new QHBoxLayout(container);
        containerLayout->setSpacing(0);
        containerLayout->setContentsMargins(0,0,0,0);
        QTextEdit *type = new QTextEdit();
        QTextEdit *filter = new QTextEdit();
        filter->setText(QString::fromStdString(filter_pair.first));
        filter->setEnabled(false);
        filter->setFixedSize(382,30);
        filter->setAttribute(Qt::WA_TransparentForMouseEvents);
        containerLayout->addWidget(filter);

        type->setText(QString::fromStdString(filter_pair.second));
        type->setFixedSize(160,30);
        type->setEnabled(false);
        type->setAttribute(Qt::WA_TransparentForMouseEvents);
        container->setFilter(QString::fromStdString(filter_pair.first));
        container->setType(QString::fromStdString(filter_pair.second));
        connect(container,&ClickableFrame::clicked,this,&rulesetCreate::selectFilter);
        containerLayout->addWidget(type);

        filtersLayout->addWidget(container);
        addFilter(QString::fromStdString(filter_pair.first),QString::fromStdString(filter_pair.second));
        createdframe->push_back(container);
        filters_labels->push_back(std::make_pair(filter,type));
    }

    for(std::string source:*sources_args){
        qDebug() << source;
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
    qDebug() << "aaa";
    filters_labels->push_back(filt_create_screen->get_labels());
    createdframe->push_back(filt_create_screen->getFrame());
}

void rulesetCreate::addSource(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    auto search_pointer = std::find(sources->begin(), sources->end(), dir.toStdString());
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

void rulesetCreate::selectSource(){
    ClickableFrame *clickedFrame = qobject_cast<ClickableFrame *>(sender());
    selected_source=clickedFrame->getSource().toStdString();
}

void rulesetCreate::editEntry(entry* prev_entry) {
    char* destination_c = strdup(destination.c_str());
    char** sources_c = new char*[sources->size()];
    int count = 1;
    for(int i =0;i<sources->size();i++){
        sources_c[i] = strdup(sources->at(i).c_str());
    }
    filterPair *filters_c = new filterPair[filters->size()];
    for(int i=0;i<filters->size();i++){
        filters_c[i].filter=strdup(filters->at(i).first.c_str());
        filters_c[i].type=strdup(filters->at(i).second.c_str());
    }
    entry* entry_arg;
    entry_arg->source = sources_c;
    entry_arg->source_count=sources->size();
    entry_arg->destination=destination_c;
    entry_arg->filters=filters_c;
    entry_arg->filter_count=filters->size();
    int res = write_entry(entry_arg);

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
        filters_c[i].filter=strdup(filters->at(i).first.c_str());
        filters_c[i].type=strdup(filters->at(i).second.c_str());
    }
    entry* entry_arg;
    entry_arg->source = sources_c;
    entry_arg->source_count=sources->size();
    entry_arg->destination=destination_c;
    entry_arg->filters=filters_c;
    entry_arg->filter_count=filters->size();
    int res = write_entry(entry_arg);
    qDebug() << res;
    this->destroy();
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

void rulesetCreate::editSource(){
    qDebug() << selected_source << "gljHGLB";
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
    ui->destinationLabel->setText(dir);
    destination = dir.toStdString();
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

void rulesetCreate::selectFilter(){
    ClickableFrame *clickedFrame = qobject_cast<ClickableFrame *>(sender());
    this->setSelectedFilter(clickedFrame->getFilter());
    this->setSelectedType(clickedFrame->getType());
    qDebug() << "kgmjnoisngmklgn";
}

