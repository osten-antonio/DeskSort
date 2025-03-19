#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rule_set_container.h"
extern "C" {
#include "script.h"
}



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawEntries();
private:
    Ui::MainWindow *ui;
    QWidget *containerWidget;


private slots:
    void openCreateWindow();

};
#endif // MAINWINDOW_H
