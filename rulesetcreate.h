#ifndef RULESETCREATE_H
#define RULESETCREATE_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <QVBoxLayout>


namespace Ui {
class rulesetCreate;
}

class rulesetCreate : public QMainWindow
{
    Q_OBJECT

public:
    explicit rulesetCreate(QWidget *parent = nullptr); // const id smthsmth
    ~rulesetCreate();
    void addFilters();
    void addSource();
    void editFilter();
    void editSource();
    void selectDestination();
    void deleteSource();
    void deleteFilter();
    void addEntry();
    void addFilter(QString filter, QString type);
    void setSelectedFilter(QString filter);
    void setSelectedType(QString type);
    QString getSelectedFilter();
    QString getSelectedType();
    QVBoxLayout *getFiltersLayout();

private:
    Ui::rulesetCreate *ui;
    QVBoxLayout *filtersLayout;
    // std::unordered_map<std::string,std::string>* filters;
    std::vector<std::pair<std::string,std::string>>* filters;
    std::string *destination;
    QString selected_filter;
    QString selected_type;
};

#endif // RULESETCREATE_H
