#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include "rulesetCreate.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    containerWidget(new QWidget())
{
    ui->setupUi(this);

    drawEntries();
    ui->mainScroll->setWidget(containerWidget);
    ui->mainScroll->setWidgetResizable(true);

    connect(ui->newRuleset,&QPushButton::clicked,this,&MainWindow::openCreateWindow);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openCreateWindow() {
    rulesetCreate *createWindow  = new rulesetCreate(this); // Edit params later
    connect(createWindow, &QObject::destroyed, this, [this]() {
        this->setEnabled(true);
        drawEntries();
    });
    createWindow->show();
    createWindow->raise();
    this->setEnabled(false);
    createWindow->setEnabled(true);
    createWindow->setFocus();

}
void MainWindow::drawEntries(){
    if (containerWidget->layout()) {
        QLayout* oldLayout = containerWidget->layout(); // since container widget has a layout attached already, remove old layout first to prevent crash
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) { // checks for the first item in the layout
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }
    QVBoxLayout *layout = new QVBoxLayout(containerWidget);
    int destination_size=0;
    char** destinations = get_destination(&destination_size);
    if(destinations!=NULL){
        for(int i =0;i<destination_size;i++){
            rule_set_container *container = new rule_set_container(this);
            if(destinations[i]!=NULL){
                container->setDestination(destinations[i]);
            }
            int source_size=0;
            char** sources = get_sources(destinations[i],&source_size);

            std::vector<std::string> sources_vector;
            sources_vector.assign(sources,sources+source_size);
            free(sources);

            int filter_size=0;
            filterPair* filters = get_filters(destinations[i],&filter_size);
            std::vector<filterPair> filters_vector;
            filters_vector.assign(filters,filters+filter_size);
            container->setFilters(&filters_vector);
            free(filters);
            container->setSources(&sources_vector);

            container->setFixedSize(771, 191);
            layout->addWidget(container);
        }
    }
    containerWidget->setLayout(layout);
}

