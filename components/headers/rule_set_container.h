#ifndef RULE_SET_CONTAINER_H
#define RULE_SET_CONTAINER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QVBoxLayout>
extern "C"{
#include "../../script.h"
}

class MainWindow;

class rule_set_container : public QWidget
{
    Q_OBJECT
public:
    explicit rule_set_container(MainWindow* parent, int top = 10, int left= 10);

    void addSourceLabel(const QString &labelText);

    void addFilterLabel(const QString &labelText);

    void setDestination(std::string destination);
    void setSources(std::vector<std::string> *sources);
    void setFilters(std::vector<filterPair> *filters);

private:
    QBoxLayout *container;
    QLineEdit *destination_label;
    QScrollArea *source_area;
    QScrollArea *filters_area;
    QPushButton *edit_button;
    QVBoxLayout *source_layout;
    QVBoxLayout *filters_layout;
    std::string destination;
    std::vector<std::string> *sources;
    std::vector<filterPair> *filters;
    MainWindow* mainWindow;

signals:
    // Add functionality later

};

#endif // RULE_SET_CONTAINER_H

// 771 191
