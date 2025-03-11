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


    for (int i = 0; i < 10; ++i) {
        rule_set_container *customWidget = new rule_set_container(this);  // Create your custom widget
        // std::string temp = ;
        char** destinations = get_destination();


        customWidget->setDestination(std::to_string(i));
        customWidget->setSources(sample_sources);
        customWidget->setFixedSize(771, 191);  // Set a fixed size for each widget
        layout->addWidget(customWidget);  // Add the custom widget to the layout
    }

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
