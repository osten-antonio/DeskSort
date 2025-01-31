#ifndef RULE_SET_CONTAINER_H
#define RULE_SET_CONTAINER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QVBoxLayout>

class rule_set_container : public QWidget
{
    Q_OBJECT
public:
    explicit rule_set_container(QWidget *parent = nullptr, int top = 10, int left= 10);
    void updateDestinationLabel(const QString &newText);

    void addSourceLabel(const QString &labelText);

    void addFilterLabel(const QString &labelText);

private:
    QBoxLayout *container;
    QLineEdit *destination_label;
    QScrollArea *source_area;
    QScrollArea *filters_area;
    QPushButton *destination_edit;
    QVBoxLayout *source_layout;
    QVBoxLayout *filters_layout;


signals:
    // Add functionality later

};

#endif // RULE_SET_CONTAINER_H

// 771 191
