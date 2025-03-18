#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include "rulesetCreate.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *containerWidget = new QWidget();  // Stores all of the rule_set_container

    QVBoxLayout *layout = new QVBoxLayout(containerWidget);
    std::vector<std::string> *sample_sources = new std::vector<std::string>();
    for (int i = 0; i < 20; ++i) {
        sample_sources->push_back(std::to_string(i));
    }
    int destination_size=0;
    char** destinations = get_destination(&destination_size);
    qDebug() << destination_size;
    if(destinations!=NULL){
        for(int i =0;i<destination_size;i++){
            qDebug() << destinations[i];
            rule_set_container *container = new rule_set_container(this);
            if(destinations[i]!=NULL){
                container->setDestination(destinations[i]);
            }
            int source_size=0;
            char** sources = get_sources(destinations[i],&source_size);
            for (int j = 0; j < source_size; j++) {
                if (sources[j] != NULL) {
                    qDebug() << " - sources[" << j << "]: " << sources[j];
                } else {
                    qDebug() << "⚠️ sources[" << j << "] is NULL!";
                }
            }
            std::vector<std::string> sources_vector;
            sources_vector.assign(sources,sources+source_size);
            free(sources);
            qDebug() << "🔹 sources_vector (size:" << sources_vector.size() << ")";
            for (const auto& source : sources_vector) {
                qDebug() << " - " << QString::fromStdString(source);
            }

            int filter_size=0;
            filterPair* filters = get_filters(destinations[i],&filter_size);
            std::vector<filterPair> filters_vector;
            filters_vector.assign(filters,filters+filter_size);
            qDebug() << "--";
            qDebug() << filter_size;
            container->setFilters(&filters_vector);
            free(filters);
            container->setSources(&sources_vector);

            container->setFixedSize(771, 191);
            layout->addWidget(container);
        }
    }
    /*
    for (int i = 0; i < 10; ++i) {
        rule_set_container *customWidget = new rule_set_container(this);  // Create your custom widget
        // std::string temp = ;
        char** destinations = get_destination();


        customWidget->setDestination(std::to_string(i));
        customWidget->setSources(sample_sources);
        customWidget->setFixedSize(771, 191);  // Set a fixed size for each widget
        layout->addWidget(customWidget);  // Add the custom widget to the layout
    }
*/

    containerWidget->setLayout(layout);

    ui->mainScroll->setWidget(containerWidget);
    ui->mainScroll->setWidgetResizable(true);

    connect(ui->newRuleset,&QPushButton::clicked,this,&MainWindow::openCreateWindow);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openCreateWindow() {
    qDebug() << "test";
    rulesetCreate *createWindow  = new rulesetCreate(this); // Edit params later
    createWindow->show();
    createWindow->raise();
    this->setEnabled(false);
    createWindow->setEnabled(true);
    createWindow->setFocus();
    qDebug() << "in";
}

// Create refresh mainwindow function
