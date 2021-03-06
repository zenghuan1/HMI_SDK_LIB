#include "SettingsBTUI.h"
#include "Home/data/Settings/SettingsBTData.h"
#include "Home/app/Home.h"
#include "HMIFrameWork/log_interface.h"
#include "HMIFrameWork/HMIFrameWork.h"
SettingsBTUI::SettingsBTUI(QWidget *parent)
    :QWidget(parent)
    ,CView(Home::eViewId_Settings_BT)
    ,m_pBackBtn(NULL)
    ,m_pTitleLabel(NULL)
    ,m_iBTStatus(BT_OFF)
{
    this->setGeometry(QRect(0,40,800,440));
    this->setStyleSheet("QWidget{border:none;background:transparent;}");

    m_pBackBtn = new CPushButton(this);
    m_pBackBtn->setStyleSheet("QPushButton{border:none;background:transparent;}");
    m_pBackBtn->setGeometry(QRect(16,21,198,29));
    m_pBackBtn->SetText(QRect(38,0,160,29),tr("Setting"),22,Qt::AlignLeft|Qt::AlignVCenter,QColor(255,255,255,204));
    m_pBackBtn->SetIcon(QRect(0,0,29,29),":/Settings/button_back.png");
    m_pBackBtn->setFocusPolicy(Qt::NoFocus);
    m_pBackBtn->show();

    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->setGeometry(QRect(220,21,360,29));
    m_pTitleLabel->setStyleSheet("QLabel{color:#4BA9FF;font-size:24px;border:none;background:transparent;}");
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    m_pTitleLabel->setText(tr("Bluetooth"));
    m_pTitleLabel->show();

    //list
    m_pVlist = new CVListWidget(this);
    m_pVlist->setGeometry(54,67,718,347);
    m_pVlist->SetLeftMargin(0);
    m_pVlist->SetSplitLine(":/Settings/line.png",":/Settings/line.png");
    m_pVlist->SetScrollBarStyle(4);
    m_pVlist->SetItemBackgroundInfo("",":/Settings/list_push_bg.png","");
    m_pVlist->AutoSetSelected(false);
    this->SetBTStatus(m_iBTStatus, true);
    m_pVlist->show();

    connect(m_pVlist,SIGNAL(listButtonReleased(int,int,int)),this,SLOT(OnListButtonReleased(int,int,int)),static_cast<Qt::ConnectionType>(Qt::QueuedConnection|Qt::UniqueConnection));
    connect(m_pVlist,SIGNAL(listItemReleased(int,int)),this,SLOT(OnListItemClicked(int,int)),static_cast<Qt::ConnectionType>(Qt::QueuedConnection|Qt::UniqueConnection));

    connect(this,SIGNAL(SigBTStatusChanged(int)),this,SLOT(OnBTStatusChanged(int)));
    connect(this,SIGNAL(SigBTStatusChanged(int)),SettingsBTData::GetInstance(),SLOT(OnBTStatusChanged(int)));
    connect(m_pBackBtn,SIGNAL(clicked()),this,SLOT(OnBack()),Qt::UniqueConnection);
    connect(SettingsBTData::GetInstance(), SIGNAL(deviceListUpdate()), this, SLOT(OnDeviceListUpdate()));
}

SettingsBTUI::~SettingsBTUI()
{

}

void SettingsBTUI::viewAction(int state)
{
    Q_UNUSED(state)
}

void SettingsBTUI::OnBack()
{
    if(Home::Inst()->GetNotifyBTShowStatus())
    {
        HMIFrameWork::Inst()->AppShow(Home::Inst()->GetNotifyBTShowId(),"Default");
        Home::Inst()->SetNotifyBTShowStatus(false);
    }
    else
    {
        Home::Inst()->ViewBack();
    }
}

void SettingsBTUI::OnListButtonReleased(int index, int btnIndex)
{
    Q_UNUSED(index)
    Q_UNUSED(btnIndex)
}

void SettingsBTUI::OnListButtonReleased(int index, int btnIndex, int specifiedID)
{
    if(0 == index)
    {
        if(0 == btnIndex)
        {
            if(0 == specifiedID)
            {
                this->SetBTStatus(BT_ON);
            }
            else
            {
                this->SetBTStatus(BT_OFF);
            }
        }
        return;
    }

    if(0 == m_pVlist->GetSpecifiedID(index))
    {
        INFO("my/other equipment clicked, do nothing");
        return;
    }

    const BTDeviceInfo &info = SettingsBTData::GetInstance()->GetBTPairedListInfo(index-specifiedID);
    SettingsBTData::GetInstance()->SetSelectedBTInfo(info);

    //TODO: show option popup
    switch (info.status) {
    case BTStatus_PAIRED:
        ShowPopUpRemoveFromPairdList();
        break;
    case BTStatus_CONNECTING:
    case BTStatus_CONNECTED:
        ShowPopUpDisConnect();
        break;
    default:
        break;
    }
    INFO("OnListButtonReleased, device name: %s", info.name.toStdString().c_str());
}

void SettingsBTUI::OnListItemClicked(int index, int specifiedID)
{
    if(0 == index)
    {
        INFO("BT Switch Item clicked");
        return;
    }

    if(0 == m_pVlist->GetSpecifiedID(index))
    {
        INFO("other equipment clicked, do nothing");
        return;
    }

    const BTDeviceInfo &info = (BTStatus_NORMAL == m_pVlist->GetSpecifiedIDStatus(index))?
                SettingsBTData::GetInstance()->GetBTSearchListInfo(index-specifiedID):
                SettingsBTData::GetInstance()->GetBTPairedListInfo(index-specifiedID);
    SettingsBTData::GetInstance()->SetSelectedBTInfo(info);

    //TODO: connect BT
    switch (info.status) {
    case BTStatus_NORMAL:
        ShowPopUpPair();
        break;
    case BTStatus_PAIRED:
        //TODO: connect device
        SettingsBTData::GetInstance()->ConnectDevice(info.id);
        break;
    default:
        break;
    }

    INFO("OnListItemClicked, device name: %s", info.name.toStdString().c_str());
}

void SettingsBTUI::OnBTStatusChanged(int status)
{
    if(BT_ON == status)
    {
        this->StartBT();
    }
}

void SettingsBTUI::OnDeviceListUpdate()
{
    this->UpdateBTList();
}

void SettingsBTUI::SetBTStatus(int status, bool init)
{
    if(!init)
    {
        if(status == m_iBTStatus)
        {
            return;
        }
    }
    m_iBTStatus = status;
    m_pVlist->RemoveAllItems();

    //add BT switch item
    {
        CListWidgetItem item(QSize(718,57));
        item.AddText(QRect(0,0,300,57),QString(tr("Bluetooth")),Qt::AlignLeft|Qt::AlignVCenter,24);
        QStringList list;
        if(0 == status)
        {
            list<<":/Settings/button_h_close.png"<<":/Settings/button_h_close.png"<<":/Settings/button_h_close.png";
        }
        else
        {
            list<<":/Settings/button_h_on.png"<<":/Settings/button_h_on.png"<<":/Settings/button_h_on.png";
        }
        item.AddButton(QRect(621,123-107,71,25), list);
        item.SetSpecifiedID(status);
        //just make the item unclickable
        list.clear();
        list<<"none"<<"none"<<"none";
        item.AddButton(QRect(0,0,718,67),list);
        m_pVlist->InsertItem(0,item);
    }

    emit SigBTStatusChanged(m_iBTStatus);
}

void SettingsBTUI::StartBT()
{
    this->UpdateBTList();
}

void SettingsBTUI::UpdateBTList()
{
    if(!m_iBTStatus)
    {
        return;
    }
    m_pVlist->RemoveItems(1, m_pVlist->count());

    bool bHasPairedList = false;
    //add my equipment item
    if(0 < SettingsBTData::GetInstance()->GetBTPairedListSize())
    {
        bHasPairedList = true;
        CListWidgetItem item (QSize(718,67));
        QStringList list;
        list<<"none"<<"none"<<"none";
        item.SetSpecifiedID(0);
        item.AddText(QRect(0,0,800-54-94-150-50,67),QString(tr("My equipment")),Qt::AlignLeft|Qt::AlignVCenter,26,QColor(255,255,255,255*0.8));
        //just make the item unclickable
        item.AddButton(QRect(0,0,718,67),list);
        m_pVlist->InsertItem(m_pVlist->count(),item);
    }

    //paired device list
    for(int i = 0 ; i < SettingsBTData::GetInstance()->GetBTPairedListSize(); ++i)
    {
        const BTDeviceInfo &info = SettingsBTData::GetInstance()->GetBTPairedListInfo(i);
        CListWidgetItem item(QSize(718,57));

        item.AddText(QRect(0,0,800-54-94-150-50,57),info.name,Qt::AlignLeft|Qt::AlignVCenter,24);

        if(bHasPairedList)
        {
            item.SetSpecifiedID(2);
        }
        else
        {
            item.SetSpecifiedID(1);
        }
        item.SetSpecifiedIDStatus(info.status);
        switch (info.status) {
        case BTStatus_PAIRED:
            item.AddText(QRect(800-54-94-150,0,150,57),QString("Unconnected"),Qt::AlignRight|Qt::AlignVCenter,20,QColor(255,255,255,204));
            break;
        case BTStatus_CONNECTING:
            item.AddIcon(QRect(800-54-54-(94-54)-35,(57-35)/2,35,35),QPixmap(":/Settings/icon_connect.png"));
            break;
        case BTStatus_CONNECTED:
            item.AddText(QRect(800-54-94-150,0,150,57),QString("Connected"),Qt::AlignRight|Qt::AlignVCenter,20,QColor(255,255,255,204));
            break;
        default:
            break;
        }

        QStringList list;
        list<<":/Settings/icon_notice.png"<<":/Settings/icon_notice.png"<<":/Settings/icon_notice.png";
        item.AddButton(QRect(800-54-54-(63-54)-22,(57-22)/2,22,22),list);
        m_pVlist->InsertItem(i,item);
    }

    //other equipment
    CListWidgetItem item (QSize(718,67));
    QStringList list;
    list<<"none"<<"none"<<"none";
    item.SetSpecifiedID(0);
    item.AddText(QRect(0,0,800-54-94-150-50,67),QString(tr("Other equipment")),Qt::AlignLeft|Qt::AlignVCenter,26,QColor(255,255,255,255*0.8));
    //just make the item unclickable
    item.AddButton(QRect(0,0,718,67),list);
    m_pVlist->InsertItem(m_pVlist->count(),item);

    //other equipment list
    for(int i = 0 ; i < SettingsBTData::GetInstance()->GetBTSearchListSize() ;i++)
    {
        const BTDeviceInfo &info = SettingsBTData::GetInstance()->GetBTSearchListInfo(i);
        CListWidgetItem item(QSize(718,57));
        item.AddText(QRect(0,0,800-54-94-150-50,57),info.name,Qt::AlignLeft|Qt::AlignVCenter,24);

        if(bHasPairedList)
        {
            item.SetSpecifiedID(SettingsBTData::GetInstance()->GetBTPairedListSize()+3);
        }
        else
        {
            item.SetSpecifiedID(SettingsBTData::GetInstance()->GetBTPairedListSize()+2);
        }

        item.SetSpecifiedIDStatus(info.status);
        m_pVlist->InsertItem(i,item);
    }
}

void SettingsBTUI::ShowPopUpPair()
{
    INFO("SettingsBTUI::ShowPopUpPair");
    const BTDeviceInfo &info = SettingsBTData::GetInstance()->GetSelectedBTInfo();

    map<string,string> msg;
    msg.insert(make_pair("PopUpType","General"));
    msg.insert(make_pair("PopUpId","BTPair"));
    msg.insert(make_pair("Show","True"));
    msg.insert(make_pair("FromAppId",HOME_ID));
    msg.insert(make_pair("ButtonA","Confirm"));
    msg.insert(make_pair("ReplyButtonA","BTPair"));
    msg.insert(make_pair("ButtonB","Cancel"));
    msg.insert(make_pair("ReplyButtonB","Cancel"));
    QString contextA = QString("Waiting for \"").append(info.name).append("\" to be paired\n");
    msg.insert(make_pair("ContextA",contextA.toStdString()));
    QString contextALight = QString("\"").append(info.name).append("\"");
    msg.insert(make_pair("ContextALight",contextALight.toStdString()));
    QString contextB = QString("Please make sure that the code displayed on the \"").append(info.name).append("\" matches the following code");
    msg.insert(make_pair("ContextB",contextB.toStdString()));
    QString contextBLight = QString("\"").append(info.name).append("\"");
    msg.insert(make_pair("ContextBLight",contextBLight.toStdString()));
    msg.insert(make_pair("Password",info.password.toStdString()));

    HMIFrameWork::Inst()->Notify(POPUP_ID,msg);
}

void SettingsBTUI::ShowPopUpDisConnect()
{
    INFO("SettingsBTUI::ShowPopUpDisConnect");
    const BTDeviceInfo &info = SettingsBTData::GetInstance()->GetSelectedBTInfo();

    map<string,string> msg;
    msg.insert(make_pair("PopUpType","General"));
    msg.insert(make_pair("PopUpId","BTDisConnect"));
    msg.insert(make_pair("Show","True"));
    msg.insert(make_pair("FromAppId",HOME_ID));
    msg.insert(make_pair("ButtonA","Break"));
    msg.insert(make_pair("ReplyButtonA","BTDisConnect"));
    msg.insert(make_pair("ButtonB","Cancel"));
    msg.insert(make_pair("ReplyButtonB","Cancel"));
    QString contextA = QString("Break the Bluetooth connect with \"").append(info.name).append("\" ?");
    msg.insert(make_pair("ContextA",contextA.toStdString()));
    QString contextALight = QString("\"").append(info.name).append("\"");
    msg.insert(make_pair("ContextALight",contextALight.toStdString()));

    HMIFrameWork::Inst()->Notify(POPUP_ID,msg);
}

void SettingsBTUI::ShowPopUpRemoveFromPairdList()
{
    INFO("SettingsBTUI::ShowPopUpRemoveFromPairdList");
    const BTDeviceInfo &info = SettingsBTData::GetInstance()->GetSelectedBTInfo();

    map<string,string> msg;
    msg.insert(make_pair("PopUpType","General"));
    msg.insert(make_pair("PopUpId","BTRemoveFromPairdList"));
    msg.insert(make_pair("Show","True"));
    msg.insert(make_pair("FromAppId",HOME_ID));
    msg.insert(make_pair("ButtonA","Remove"));
    msg.insert(make_pair("ReplyButtonA","BTRemove"));
    msg.insert(make_pair("ButtonB","Cancel"));
    msg.insert(make_pair("ReplyButtonB","Cancel"));
    QString contextA = QString("Rmove \"").append(info.name).append("\" from paired list?");
    msg.insert(make_pair("ContextA",contextA.toStdString()));
    QString contextALight = QString("\"").append(info.name).append("\"");
    msg.insert(make_pair("ContextALight",contextALight.toStdString()));

    HMIFrameWork::Inst()->Notify(POPUP_ID,msg);
}
