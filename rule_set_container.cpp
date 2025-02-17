#include "rule_set_container.h"

rule_set_container::rule_set_container(QWidget *parent, int top, int left)
    : QWidget(parent),
    destination_label(new QLineEdit("", this)),
    source_area(new QScrollArea(this)),
    filters_area(new QScrollArea(this)),
    destination_edit(new QPushButton("✏️ Edit", this)),
    source_layout(new QVBoxLayout()),
    filters_layout(new QVBoxLayout())
{
    move(left, top);

    QHBoxLayout *destinations = new QHBoxLayout();
    QLabel *destination = new QLabel("Destination:",this);
    destination->setGeometry(20,10,111,20);
    destination_label->setDisabled(true);
    destination_label->setGeometry(90, 10, 581, 24);
    destination_label->setFixedSize(581, 24);
    destination_edit->setGeometry(680, 10, 80, 24);
    destinations->addWidget(destination);
    destinations->addWidget(destination_label);
    destinations->addWidget(destination_edit);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    QWidget *source_widget = new QWidget(this);
    source_widget->setLayout(source_layout);
    source_area->setWidget(source_widget);
    source_area->setWidgetResizable(false);
    source_area->setGeometry(10, 60, 370, 181);
    horizontalLayout->addWidget(source_area);

    QWidget *filters_widget = new QWidget(this);
    filters_widget->setLayout(filters_layout);
    filters_area->setWidget(filters_widget);
    filters_area->setWidgetResizable(false);
    filters_area->setGeometry(390, 60, 370, 181);
    horizontalLayout->addWidget(filters_area);

    container = new QVBoxLayout(this);
    container->addLayout(destinations);

    container->addLayout(horizontalLayout);

    setLayout(container);

    setFixedSize(771, 191);
}
void rule_set_container::updateDestinationLabel(const QString &newText){
    destination_label->setText(newText);
}


