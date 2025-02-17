#include "filtercreate.h"
#include "ui_filtercreate.h"
#include "rulesetcreate.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QTextEdit>


filtercreate::filtercreate(rulesetCreate *create_screen,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::filtercreate), create_screen(create_screen)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,filtercreate::addFilter);
}

filtercreate::~filtercreate()
{
    delete ui;
}

void filtercreate::addFilter(){
    // validation and add to database
    qDebug() << ui->lineEdit->text();
    qDebug() << ui->comboBox->currentText();
    this->destroy();

    create_screen->setEnabled(true);

    QVBoxLayout *filtersLayout = create_screen->getFiltersLayout();

    // Set mouse event for selecting specific entries, create class variable
    QFrame *container = new QFrame();
    container->setFixedSize(540,30);

    QHBoxLayout *containerLayout = new QHBoxLayout(container);
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(0,0,0,0);
    QTextEdit *filter = new QTextEdit();
    filter->setText(ui->lineEdit->text());
    filter->setEnabled(false);
    filter->setFixedSize(382,30);
    filter->setAttribute(Qt::WA_TransparentForMouseEvents);
    containerLayout->addWidget(filter);

    QTextEdit *type = new QTextEdit();
    type->setText(ui->comboBox->currentText());
    type->setFixedSize(160,30);
    type->setEnabled(false);
    type->setAttribute(Qt::WA_TransparentForMouseEvents);
    containerLayout->addWidget(type);

    filtersLayout->addWidget(container);

}

