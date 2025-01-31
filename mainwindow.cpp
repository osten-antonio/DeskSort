#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *containerWidget = new QWidget();  // Stores all of the rule_set_container

    QVBoxLayout *layout = new QVBoxLayout(containerWidget);


    for (int i = 0; i < 10; ++i) {
        rule_set_container *customWidget = new rule_set_container(this);  // Create your custom widget
        customWidget->updateDestinationLabel(QString::number(i));
        customWidget->setFixedSize(771, 191);  // Set a fixed size for each widget
        layout->addWidget(customWidget);  // Add the custom widget to the layout

    }

    containerWidget->setLayout(layout);

    ui->mainScroll->setWidget(containerWidget);
    ui->mainScroll->setWidgetResizable(true);

}
MainWindow::~MainWindow()
{
    delete ui;
}
