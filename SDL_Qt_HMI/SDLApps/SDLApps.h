#ifndef SDLAPPS_H
#define SDLAPPS_H
#include "HMIFrameWork/App.h"
#include "HMIFrameWork/AppConfig.h"
#include "SDLVFactory.h"
#include "SDLAppsWindow.h"
class SDLApps:public QObject,public App
{
    Q_OBJECT
public:
    enum eViewId{
        eViewId_SDL_Main
    };
    SDLApps();
    static SDLApps* Inst();

    void onAppShow(string appId,string viewId) ;
    void onAppHide();
    void onNotify(string appId,map<string,string> parameter);
public slots:
    void OnAppShow(string appId,string viewId);
    void OnAppHide();
    void OnNotify(string appId, map<string, string> parameter);

public:signals:
    void SigAppShow(string appId,string viewId);
    void SigAppHide();
    void SigNotify(string appId, map<string, string> parameter);
private:
    static SDLApps *m_pInst;
};

#endif // SDLAPPS_H
