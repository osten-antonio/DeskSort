#include "./headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include "./headers/rulesetCreate.h"
#include <QDebug>
#include <QThread>
#include <nlohmann/json.hpp>
#include <fstream>
#include <QStyle>
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <qdir.h>
#include <shlobj.h>

using json = nlohmann::json;


HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc)
{ // yea idk man https://stackoverflow.com/questions/3906974/how-to-programmatically-create-a-shortcut-using-win32
    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
}
QString getStartupShortcutPath(const QString &shortcutName) {
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, path))) {
        QString startupPath = QString::fromWCharArray(path);
        return QDir(startupPath).filePath(shortcutName + ".lnk");
    }
    return QString();
}

class ScriptThread : public QThread {
    void run() override {
        while(true){
            qDebug() << "ran";
            std::ifstream file("config.json");
            json j;
            file >> j;
            main_script();
            int interval = j["interval"].get<int>() < 5 ? 5 : j["interval"].get<int>();
            if(j["minimize"]){
                QString executable_path = QCoreApplication::applicationFilePath();
                QString exe_name = QFileInfo(QCoreApplication::applicationFilePath()).baseName();
                QString shortcut_path = getStartupShortcutPath(exe_name);

                if (!QFile::exists(shortcut_path)) {
                    CreateLink(reinterpret_cast<LPCWSTR>(executable_path.utf16()),
                               shortcut_path.toStdString().c_str(),
                               L"organizer");
                }
            }
            QThread::sleep(60*interval);
        }
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    containerWidget(new QWidget())
{
    ui->setupUi(this);
    std::ifstream file("config.json");
    json j;
    file >> j;
    ui->interval->setValue(j["interval"].get<int>() < 5 ? 5 : j["interval"].get<int>());
    ui->minimizeOnClose->setChecked(j["minimize"].get<bool>());
    drawEntries();
    ui->mainScroll->setWidget(containerWidget);
    ui->mainScroll->setWidgetResizable(true);

    trayIconMenu = new QMenu(this);
    QAction *restoreAction = new QAction("Restore", this);
    QAction *quitAction = new QAction("Quit", this);

    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon = style()->standardIcon(QStyle::SP_ComputerIcon);
    trayIcon->setIcon(icon);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("Organizer");
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason){
        if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
            this->showNormal();
            this->raise();
            this->activateWindow();
        }
    });


    connect(ui->newRuleset,&QPushButton::clicked,this,&MainWindow::openCreateWindow);
    connect(ui->interval, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateInterval);
    connect(ui->minimizeOnClose, &QCheckBox::toggled,this, &MainWindow::updateMinimize);
    ScriptThread *thread = new ScriptThread();
    thread->start();
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)
{

    std::ifstream file("config.json");
    json j;
    file >> j;
    if(j["minimize"].get<bool>()){
        if (trayIcon->isVisible()) {
            hide();
            event->ignore(); // Prevents closing
        }
    }
}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() & Qt::WindowMinimized) {
            if (trayIcon->isVisible()) {
                hide();
                event->ignore();
            }
        }
    }

    QMainWindow::changeEvent(event);
}


void MainWindow::updateMinimize(){
    std::ifstream inputFile("config.json");
    if (!inputFile.is_open()) {
        return;
    }
    json j;
    inputFile >> j;
    inputFile.close();
    j["minimize"]=ui->minimizeOnClose->isChecked();
    std::ofstream outputFile("config.json");

    if (outputFile.is_open()) {
        outputFile << j.dump(4);
        outputFile.close();
    } else {
        return ;
    }
}

void MainWindow::updateInterval(){
    qDebug() << "Pressed";
    std::ifstream inputFile("config.json");
    if (!inputFile.is_open()) {
        qDebug() << "Could not open config.json for reading.";
        return;
    }
    json j;
    inputFile >> j;
    inputFile.close();
    j["interval"]=ui->interval->value();
    std::ofstream outputFile("config.json");

    if (outputFile.is_open()) {
        outputFile << j.dump(4);
        outputFile.close();
    } else {
        return ;
    }
}

void MainWindow::openCreateWindow() {
    rulesetCreate *createWindow  = new rulesetCreate(this); // Edit params later
    connect(createWindow, &QObject::destroyed, this, [this]() {
        this->setEnabled(true);
        drawEntries();
    });
    createWindow->show();
    createWindow->raise();
    this->setEnabled(false);
    createWindow->setEnabled(true);
    createWindow->setFocus();

}
void MainWindow::drawEntries(){
    if (containerWidget->layout()) {
        QLayout* oldLayout = containerWidget->layout(); // since container widget has a layout attached already, remove old layout first to prevent crash
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) { // checks for the first item in the layout
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }
    QVBoxLayout *layout = new QVBoxLayout(containerWidget);
    int destination_size=0;
    char** destinations = get_destination(&destination_size);
    if(destinations!=NULL){
        for(int i =0;i<destination_size;i++){
            rule_set_container *container = new rule_set_container(this);
            if(destinations[i]!=NULL){
                container->setDestination(destinations[i]);
            }
            int source_size=0;
            char** sources = get_sources(destinations[i],&source_size);

            std::vector<std::string> sources_vector;
            sources_vector.assign(sources,sources+source_size);
            free(sources);

            int filter_size=0;
            filterPair* filters = get_filters(destinations[i],&filter_size);
            std::vector<filterPair> filters_vector;
            filters_vector.assign(filters,filters+filter_size);
            container->setFilters(&filters_vector);
            free(filters);
            container->setSources(&sources_vector);

            container->setFixedSize(771, 191);
            layout->addWidget(container);
        }
    }
    containerWidget->setLayout(layout);
}

