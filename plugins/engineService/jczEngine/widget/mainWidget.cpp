#include "mainWidget.h"
#include "../qssDef.h"
#include "cameraCreate.h"
#include "system/camera.h"

#include "template/templateHelper.h"

#include "Ui/uiManager.h"
#include "service/serviceManager.h"
#include "message/msgManager.h"

#include "Ui/uiManager.h"
#include <QLabel>
#include <QTabBar>
#include <QToolButton>
#include <QTabWidget>
#include <QVBoxLayout>
using namespace BS_QT_Ui;

CJczMainWidget *jczMainWindow()
{
    return qobject_cast<CJczMainWidget *>(BS_QT_Service::CServiceManager::GetService(cJczMainWidget));
}

#define cWorkTagName QObject::tr("Workspace")
#define cProjectEditTagName QObject::tr("Mark Editing")
CJczMainWidget::CJczMainWidget(QWidget *p) : QWidget(p)
{
    setObjectName(cJczMainWidget);
    BS_QT_Service::CServiceManager::RegistService(cJczMainWidget, this);
}

CJczMainWidget::~CJczMainWidget()
{
}

void CJczMainWidget::init()
{
    initLayout();
}

void CJczMainWidget::initLayout()
{
    m_pWorkWindow = new CJczWorkWidget(this);
    m_pWorkWindow->init();
    m_pWorkWindow->initLayout();
    m_pTagWidget = new BS_QT_Ui::CCustomTabWidget(this);

    auto pTagCornerCameraBtn = m_pTagWidget->addCornerBtn(tr("Vision Camera"));
    connect(pTagCornerCameraBtn, &QToolButton::clicked, this, [this]
            {
                if (visionCamera()->isAcquire())
                {
                    Out_SyncMsgBox << tr("The vision camera is in use and cannot be configured");
                    return;
                }
                auto pDialog = BS_Hal_Camera::CCameraCreater::newConfigCameraDialog(VisionCameraID,this);
                CUIManager::moveToCenter(pDialog);
                pDialog->exec();
                pDialog->deleteLater(); });

    connect(
        visionCamera(), &BS_Hal_Camera::ICameraControl::sigStateChanged, this, [this, pCameraBtn = pTagCornerCameraBtn](BS_Hal_Camera::CCameraStateInfo p_stateInfo)
        { pCameraBtn->setChecked(p_stateInfo.isConnected); },
        Qt::QueuedConnection);
    m_pTagWidget->addTab(m_pWorkWindow, cWorkTagName);
    for (auto pTemplate : templateHelper()->m_templateMap)
    {
        auto pEditWidget = pTemplate->newEditWidget(this);
        if (pEditWidget != nullptr)
        {
            m_pTagWidget->addTab(pEditWidget, pTemplate->typeString());
            connect(pEditWidget,&BS_2DVision_Template::ITemplateEditWidget::sigEditChanged,this,&CJczMainWidget::onlyShowTabWidget,Qt::QueuedConnection);
        }
    }

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pTagWidget);
    pLayout->setMargin(1);
    pLayout->setSpacing(0);
}

void CJczMainWidget::onlyShowTabWidget(bool p_only, QWidget *pWnd)
{
    auto pTabBar = m_pTagWidget->tabBar();
    assert(pTabBar != nullptr);
    if (p_only)
    {
        pTabBar->hide();
    }
    else
    {
        if (pWnd != nullptr)
        {
            assert(m_pTagWidget->indexOf(pWnd) >= 0 && m_pTagWidget->count() > 0);
            if (m_pTagWidget->currentWidget() != pWnd)
            {
                m_pTagWidget->setCurrentWidget(pWnd);
            }
        }
        pTabBar->show();
    }
    emit sigOnlyShow(p_only);
}