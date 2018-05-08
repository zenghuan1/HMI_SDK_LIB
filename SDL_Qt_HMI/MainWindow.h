#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Home/HomeWindow.h"
#include "Home/QuickLanuchView.h"
#include "Home/Home.h"
#include "Home/HomeVFactory.h"
#include "Home/QuickLanuch.h"
#include "HMIFrameWork/AppManager.h"
#include "HMIFrameWork/HMIFrameWork.h"
#include "AppViewManager.h"
#include "SDLApps/SDLApps.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void InitHome();
    void InitQuickLanuch();
    void InitSDLApps();
private slots:

    void OnEdit(bool bStatus);
private:
    Ui::MainWindow *ui;
    AppViewManager *m_pAppViewManager;
    QuickLanuchView *m_pQuickLanuch;

};

#endif // MAINWINDOW_H
