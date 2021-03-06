#include "SettingsSpeechRecognitionUI.h"
#include "HMIFrameWork/log_interface.h"
#include "Home/app/Home.h"
SettingsSpeechRecognitionUI::SettingsSpeechRecognitionUI(QWidget *parent)
    :QWidget(parent)
    ,CView(Home::eViewId_Settings_SpeechRecogniton)
    ,m_pBackBtn(NULL)
    ,m_pTitleLabel(NULL)
{
    this->setGeometry(QRect(0,40,800,440));
    this->setStyleSheet("QWidget{border:none;background:transparent;}");
    m_pBackgroundLabel = new QLabel(this);
    m_pBackgroundLabel->setGeometry(QRect(0,0,800,440));
    m_pBackgroundLabel->setStyleSheet("QLabel{border-image:url(:/images/bg.png);background:transparent;}");
    m_pBackgroundLabel->lower();
    m_pBackgroundLabel->show();
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
    m_pTitleLabel->setText(tr("Speech Recognition"));
    m_pTitleLabel->show();


    m_pVlist = new CVListWidget(this);
    m_pVlist->setGeometry(54,67,718,386);
    m_pVlist->SetLeftMargin(0);
    m_pVlist->SetSplitLine(":/Settings/line.png",":/Settings/line.png");
    m_pVlist->SetScrollBarStyle(4);
    m_pVlist->SetItemBackgroundInfo("","","");
    m_pVlist->AutoSetSelected(false);
    QStringList titieList;
    titieList <<tr("Voice Prompt") << tr("Speed") ;;
    for(int i = 0 ; i < titieList.size() ;i++)
    {
        CListWidgetItem item (QSize(718,57));
        if(0 == i)
        {
            item.SetSpecifiedID(-1);
            item.AddText(QRect(0,0,300,57),titieList.at(i),Qt::AlignLeft|Qt::AlignVCenter,24);
            item.SetSpecifiedID(-1);
            QStringList text;
            text<<tr("Brief")<<tr("Detailed");
            m_pVlist->AddComboBox(i,":/Settings/select_b.png",text,QRect(418,10,252,37),QSize(718,57),22);
        }
        else if(1 == i)
        {
            item.SetSpecifiedID(-1);
            item.AddText(QRect(0,0,300,57),titieList.at(i),Qt::AlignLeft|Qt::AlignVCenter,24);
            item.SetSpecifiedID(-1);
            QStringList text;
            text<<tr("Standard")<<tr("Fast")<<tr("Slow");
            m_pVlist->AddComboBox(i,":/Settings/select_b.png",text,QRect(418,10,252,37),QSize(718,57),22);
        }
        m_pVlist->InsertItem(i,item);

    }
    m_pVlist->show();

    InitConnect();
}

SettingsSpeechRecognitionUI::~SettingsSpeechRecognitionUI()
{

}

void SettingsSpeechRecognitionUI::InitConnect()
{
    connect(m_pBackBtn,SIGNAL(clicked()),this,SLOT(OnBack()),Qt::UniqueConnection);

    connect(m_pVlist,SIGNAL(listButtonReleased(int,int)),this,SLOT(OnListBtnClick(int,int)),Qt::UniqueConnection);
    connect(m_pVlist,SIGNAL(listItemReleased(int)),this,SLOT(OnListClick(int)),Qt::UniqueConnection);

}

void SettingsSpeechRecognitionUI::OnBack()
{
  Home::Inst()->ViewBack();
}

void SettingsSpeechRecognitionUI::viewAction(int state)
{
    Q_UNUSED(state)
}



void SettingsSpeechRecognitionUI::OnListBtnClick(int index, int btnIndex)
{
    Q_UNUSED(index)
    Q_UNUSED(btnIndex)
}

void SettingsSpeechRecognitionUI::OnListClick(int index)
{
    Q_UNUSED(index)
}
