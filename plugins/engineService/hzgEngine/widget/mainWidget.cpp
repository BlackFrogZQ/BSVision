#include "mainWidget.h"
#include "../qssDef.h"
#include "cameraCreate.h"
#include "system/camera.h"
#include "service/serviceManager.h"
#include "ui/workWidget/stdWorkWidget.h"
#include "ui/projectEditWidget/projectEditWidget.h"

#include "Ui/uiManager.h"
#include "message/msgManager.h"

#include <QLabel>
#include <QToolButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTabBar>
using namespace BS_QT_Ui;

CHzgMainWidget *hzgMainWindow()
{
    return qobject_cast<CHzgMainWidget *>(BS_QT_Service::CServiceManager::GetService(cHzgMainWidget));
}

#define cWorkTagName QObject::tr("Workspace")
#define cProjectEditTagName QObject::tr("Mark Editing")
CHzgMainWidget::CHzgMainWidget(QWidget *p) : QWidget(p)
{
    setObjectName(cHzgMainWidget);
    BS_QT_Service::CServiceManager::RegistService(cHzgMainWidget, this);
}

CHzgMainWidget::~CHzgMainWidget()
{
}

void CHzgMainWidget::init()
{
    initLayout();
}

void CHzgMainWidget::initLayout()
{
    m_pWorkWindow = new CStdWorkWidget(this);
    m_pWorkWindow->init();
    m_pWorkWindow->initLayout();

    m_pProjectEditWidget = new CProjectEditWidget(this);

    m_pTagWidget = new BS_QT_Ui::CCustomTabWidget(this);

    auto pTagCornerCameraBtn = m_pTagWidget->addCornerBtn(tr("Camera"));
    connect(pTagCornerCameraBtn, &QToolButton::clicked, this, [this]
            {
                    if (visionCamera()->isAcquire())
                    {
                        Out_SyncMsgBox << tr("The camera is in use and cannot be configured");
                        return;
                    }
                    auto pDialog = BS_Hal_Camera::CCameraCreater::newConfigCameraDialog(VisionCameraID,this);
                    CUIManager::moveToCenter(pDialog);
                    pDialog->exec();
                    pDialog->deleteLater(); });
    connect(visionCamera(), &BS_Hal_Camera::ICameraControl::sigStateChanged, this, [this, pCameraBtn = pTagCornerCameraBtn](BS_Hal_Camera::CCameraStateInfo p_stateInfo)
        { pCameraBtn->setChecked(p_stateInfo.isConnected); }, Qt::QueuedConnection);

    m_pTagWidget->addTab(m_pWorkWindow, cWorkTagName);
    m_pTagWidget->addTab(m_pProjectEditWidget, cProjectEditTagName);
    // m_pTagWidget->tabBar()->setVisible(false);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pTagWidget);
    pLayout->setMargin(1);
    pLayout->setSpacing(0);
}

void CHzgMainWidget::showOnlyEditWindow(bool p_only)
{

}