#ifndef RULESETCREATE_H
#define RULESETCREATE_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <QVBoxLayout>
#include "clickableframe.h"
#include <QTextEdit>
extern "C" {
#include "script.h" // write_entry and update_entry
}

namespace Ui {
class rulesetCreate;
}

class rulesetCreate : public QMainWindow
{
    Q_OBJECT

public:

    explicit rulesetCreate(QWidget *parent = nullptr); // const id smthsmth
    explicit rulesetCreate(std::vector<filterPair> *filters,std::vector<std::string> *sources
                           , std::string destination, QWidget *parent = nullptr);
    ~rulesetCreate();
    void setSelectedType(QString type);
    void setSelectedFilter(QString filter);

    void addFilter(QString filter, QString type);
    QString getSelectedFilter();
    QString getSelectedType();
    QVBoxLayout *getFiltersLayout();


private:
    Ui::rulesetCreate *ui;
    QVBoxLayout *filtersLayout;
    QVBoxLayout *sourcesLayout;
    // std::unordered_map<std::string,std::string>* filters;
    std::vector<filterPair>* filters;
    std::vector<std::pair<QTextEdit*,QTextEdit*>>* filters_labels;
    std::vector<ClickableFrame*>* createdframe;
    std::vector<ClickableFrame*>* sourceframe;
    std::vector<std::string>* sources;
    std::vector<QTextEdit*>* source_labels;
    std::string destination;
    std::string selected_source;
    QString selected_filter;
    QString selected_type;
    void addFilters();
    void addSource();
    void editFilter();
    void editSource();
    void selectDestination();
    void deleteSource();
    void deleteFilter();
    void addEntry();
    void editEntry(entry* prev_entry);
    void selectSource();
    void selectFilter();
};

#endif // RULESETCREATE_H
