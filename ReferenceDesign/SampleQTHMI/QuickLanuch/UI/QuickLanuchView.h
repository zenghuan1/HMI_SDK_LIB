#ifndef QUICKLANUCHVIEW_H
#define QUICKLANUCHVIEW_H

#include <QWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QTimer>
#include <QList>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include "HMIFrameWork/log_interface.h"
#include <QCoreApplication>
#include "HMIWidgets/CCButton.h"

#include "HMIFrameWork/CView.h"
#include "HMIFrameWork/AppConfig.h"
#include <QLabel>

class QuickLanuchView : public QWidget ,public CView
{
    Q_OBJECT
    Q_PROPERTY(QPoint position READ getPosition WRITE setPosition)
public:
    enum ePlaceMode
    {
        ePlaceMode_Horizontal,
        ePlaceMode_vertical
    };
    explicit QuickLanuchView(QWidget *parent = 0);
    ~QuickLanuchView();
    virtual void viewAction(int state);

    bool MouseEvent(QObject *, QEvent*);
    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);
public:
    void InitQuickLanuchView();
    void InitQuickLanuchData();
    void SetViewGeometry(QRect rect);
    void InsertQucikLanuch(int index,CCButton*);
    void ReplaceQucikLanuch(int x,int y, QString type);
    bool containsType(QString type);
    void EnterEditStatus();
    void ExitEditStatus();
    bool GetEditStatus();
    void CreatePullBack(CCButton* btn);
    void CreateVoice(CCButton* btn);
    void SortByIndex(int PressIndex,int SelectIndex);
    CCButton* Index(int x,int y);

    int findAppInfo(QString type );

    CCButton *GetApp();
    void SetApp(CCButton *app);

private:
    QPoint getPosition();
    void setPosition(QPoint position);
    void MoveAnimation(const QPoint startPos, const QPoint endPos,int time);
    void PullBackAnimation(const QPoint startPos, const QPoint endPos,int time);

signals:
    void SigRelease(int,QString,QString);
    void SigPageNum(int);
    void SigIsEnterEdit(bool);
    void SigSortByIndex(int PressIndex,int SelectIndex);
    void SigPress(int,QString,QString);
    void SigMovingPos(int x ,int y,QString Type);
    void SigReleasePos(int x,int y,QString Type);

    void SigVoiceClick();
    void SigPllBack(QString);
public slots:
    void OnReplaceInfo(int x,int y,string type);
    void OnAppClick(int,QString,QString);
    void OnPllBack(QString type);
    void OnEnterQuickStatus(QString status,QString type);
    void OnEnterEditStatus();

    void OnPullBackFinish();
private:

    QPropertyAnimation      *m_pMoveAnimation;
    QRect                    m_viewRect;
    CCButton                *m_pAppBtn;
    int                      m_iPosX;
    int                      m_iPosY;
    QList<CCButton*>         m_ListBtn;
    QList<AppInfo>           m_ListData;
    QMap<int,QRect>          m_MapRect;

    CCButton                *m_pPullBackBtn;
    QPropertyAnimation      *m_pPullBackAnimation;
    CCButton                *m_pVoiceBtn;
    QLabel                  *m_pLine1;
    QTimer                   m_pressTimer;
    bool                     m_bEditStatus;
    int                      m_iPosDiffX;
    int                      m_iPosDiffY;
    QPoint                   m_stayPos;
    int                      m_MovingSelectIndex;


};

#endif // QUICKLANUCHVIEW_H
