#ifndef RULESETCREATE_H
#define RULESETCREATE_H

#include <QMainWindow>
#include <vector>
#include <string>

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


private:
    Ui::rulesetCreate *ui;

    std::vector<std::string>* sources;
    std::vector<std::string>* filters;
    std::string *destination;

};

#endif // RULESETCREATE_H
