#ifndef APPVIEWMANAGER_H
#define APPVIEWMANAGER_H

#include <QWidget>
#include <Home/HomeView.h>
#include <QProcess>
#include "HMIFrameWork/AppConfig.h"
#include "SDLApps/Gen3UIManager.h"
#include "SDLApps/Common/AppBase.h"
#include <QDialog>
#include "HMIFrameWork/AppManager.h"

class AppViewManager:public QWidget
{
    Q_OBJECT

public:
    AppViewManager(QWidget *parent = 0);
    ~AppViewManager();

private slots:
    void OnAppPress(int index,QString type,QString name);
    void OnEditStatus(bool IsEnterStatus);
    void OnEnterQucikLanuchEdit(bool bStatus);
    void OnTriggerDomain(int,int,CCButton::CCBtnInfo);
private:
    void InitPageIndicator(int nAppsSize);
public:signals:
    void SigEditStatus(bool);
    void SigEnterQuickLanuchEdit(bool);
    void SigTriggerDomain(int,int,CCButton::CCBtnInfo);

private:

};

#endif // APPVIEWMANAGER_H
