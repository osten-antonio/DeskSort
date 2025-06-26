#include "./headers/rule_set_container.h"
#include <QTextEdit>
#include "./headers/rulesetcreate.h"
#include "./headers/mainwindow.h"

rule_set_container::rule_set_container(MainWindow* parent, int top, int left)
    : QWidget(parent),
    mainWindow(parent),
    destination_label(new QLineEdit("", this)),
    source_area(new QScrollArea(this)),
    filters_area(new QScrollArea(this)),
    edit_button(new QPushButton("✏️ Edit", this)),
    source_layout(new QVBoxLayout()),
    filters_layout(new QVBoxLayout()),
    sources(new std::vector<std::string>),
    filters(new std::vector<filterPair>)
{
    move(left, top);

    QHBoxLayout *destinations = new QHBoxLayout();
    QLabel *destination_text = new QLabel("Destination:",this);
    destination_text->setGeometry(20,10,111,20);
    destination_label->setDisabled(true);
    destination_label->setGeometry(90, 10, 581, 24);
    destination_label->setFixedSize(581, 24);
    edit_button->setGeometry(680, 10, 80, 24);
    destinations->addWidget(destination_text);
    destinations->addWidget(destination_label);
    destinations->addWidget(edit_button);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    QWidget *source_widget = new QWidget(this);
    source_widget->setLayout(source_layout);
    source_area->setWidget(source_widget);
    source_area->setWidgetResizable(true);
    source_area->setGeometry(10, 60, 370, 181);
    horizontalLayout->addWidget(source_area);

    QWidget *filters_widget = new QWidget(this);
    filters_widget->setLayout(filters_layout);
    filters_area->setWidget(filters_widget);
    filters_area->setWidgetResizable(true);

    filters_area->setGeometry(390, 60, 370, 181);
    filters_area->setFixedWidth(370);
    filters_widget->setFixedWidth(370);
    horizontalLayout->addWidget(filters_area);


    filters_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    source_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    container = new QVBoxLayout(this);
    container->addLayout(destinations);

    container->addLayout(horizontalLayout);

    setLayout(container);

    setFixedSize(771, 191);

    connect(edit_button,&QPushButton::clicked,this,[this](){
        rulesetCreate *createWindow  = new rulesetCreate(filters,sources,destination,this);

        connect(createWindow, &QObject::destroyed, this, [this]() {
            this->setEnabled(true);
            mainWindow->drawEntries();
        });
        createWindow->show();
        // createWindow->raise();
        // this->setEnabled(false);
        // createWindow->setEnabled(true);
        // createWindow->setFocus();

    });
}
void rule_set_container::setDestination(std::string destination){
    destination_label->setText(QString::fromStdString(destination));
    this->destination=destination;
}
void rule_set_container::setSources(std::vector<std::string> *sources){
    for(std::string source:*sources){
        this->sources->push_back(source);
        QTextEdit* source_label = new QTextEdit(source_area);
        source_label->setEnabled(false);
        source_label->setText(QString::fromStdString(source));
        source_label->setFixedHeight(30);
        source_layout->addWidget(source_label);
    }
}
void rule_set_container::setFilters(std::vector<filterPair> *filters){

    for(filterPair filter:*filters){
        this->filters->push_back(filter);
        QTextEdit *type = new QTextEdit();
        QTextEdit *filter_label = new QTextEdit();
        QFrame *container = new QFrame(filters_area);
        container->setFixedSize(340,30);

        QHBoxLayout *containerLayout = new QHBoxLayout(container);
        containerLayout->setSpacing(0);
        containerLayout->setContentsMargins(0,0,0,0);

        filter_label->setText(QString::fromStdString(filter.filter));
        filter_label->setEnabled(false);
        filter_label->setFixedSize(382,30);
        containerLayout->addWidget(filter_label);

        type->setText(QString::fromStdString(filter.type));
        type->setFixedSize(90,30);
        type->setEnabled(false);
        containerLayout->addWidget(type);
        filters_layout->addWidget(container);
    }
}


