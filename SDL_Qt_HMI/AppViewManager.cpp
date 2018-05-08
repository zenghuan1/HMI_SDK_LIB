#include "AppViewManager.h"
#include "HMIFrameWork/HMIFrameWork.h"
AppViewManager::AppViewManager(QWidget *parent)
    :QWidget(parent)
{
    this->setStyleSheet("QWidget{background:transparent;}");

    setGeometry(QRect(0,0,800,480));

    this->show();
}

AppViewManager::~AppViewManager()
{
}

void AppViewManager::OnAppPress(int index, QString type, QString name)
{
    INFO("AppViewManager::OnAppPress index=%d,type=%s,name=%s",index,type.toStdString().c_str(),name.toStdString().c_str());
}

void AppViewManager::OnEditStatus(bool IsEnterStatus)
{
    if(IsEnterStatus)
    {
        this->raise();
        this->show();
    }else
    {
        this->lower();
        this->show();
    }
    emit SigEditStatus(IsEnterStatus);
}

void AppViewManager::OnEnterQucikLanuchEdit(bool bStatus)
{
    emit SigEnterQuickLanuchEdit(bStatus);
}

void AppViewManager::OnTriggerDomain(int x ,int y ,CCButton::CCBtnInfo info)
{
    emit SigTriggerDomain(x,y,info);
}

