#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,m_pQuickLanuch(NULL)
{
    ui->setupUi(this);

    this->resize(800,480);

    m_pAppViewManager = new AppViewManager(this);
    m_pAppViewManager->setGeometry(QRect(0,0,800,480));
    m_pAppViewManager->show();
    HMIFrameWork::Inst()->SetMain(reinterpret_cast<void*>(m_pAppViewManager));

    InitHome();
    InitQuickLanuch();
    InitSDLApps();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitHome()
{

    HMIFrameWork::Inst()->RegisterApp(dynamic_cast<App*> (Home::Inst()));
    HMIFrameWork::Inst()->AppShow(HOME_ID);
}

void MainWindow::InitQuickLanuch()
{
    HMIFrameWork::Inst()->RegisterApp(dynamic_cast<App*> (QuickLanuch::Inst()),this);
    HMIFrameWork::Inst()->AppShow(QUICKLANUCH_ID);
}

void MainWindow::InitSDLApps()
{
    HMIFrameWork::Inst()->RegisterApp(dynamic_cast<App*> (SDLApps::Inst()));
}


void MainWindow::OnEdit(bool bStatus)
{
    if(bStatus)
    {
        m_pQuickLanuch->lower();
        m_pQuickLanuch->show();
    }else
    {
        m_pQuickLanuch->raise();
        m_pQuickLanuch->show();
    }
}
