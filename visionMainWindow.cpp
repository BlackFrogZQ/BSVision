#include "visionMainWindow.h"
#include "plugins/engineService/engineService.h"
#include "Ui/uiHelper.h"
#include "Ui/uiManager.h"
#include "Ui/textEditReceiver.h"
#include "Ui/msgBoxReceiver.h"
#include "Ui/widget/tabWidget.h"
#include "Ui/widget/mainAlarmBox.h"

#include "logReceiver/logReceiver.h"
#include "logReceiver/logWidget.h"

#include "service/serviceManager.h"
#include "message/receiverTextEdit.h"
#include "message/msgManager.h"
#include "para/paraManager.h"
#include "fileHelper/jsomHelper.h"

#include "guiDef.h"

#include <QDockWidget>
#include <QLabel>

using namespace BS_QT_Ui;
using namespace BS_QT_Message;
using namespace BS_QT_Service;
using namespace BS_QT_Framework;
using namespace BS_QT_FlieHelper;

CVisionMainWindow::CVisionMainWindow(QWidget *p) : BS_QT_Ui::CCustomMainWindow(true, true, p), m_pEngineService(nullptr)
{
    m_pEngineService = qobject_cast<IEngineService *>(CServiceManager::GetService(IEngineService_ObjectName));
    assert(m_pEngineService != nullptr);
    m_pEngineService->init();
    init();
}

CVisionMainWindow::~CVisionMainWindow()
{
    if (m_pEngineService != nullptr)
    {
        m_pEngineService->release();
    }
}

void CVisionMainWindow::init()
{
    m_pCenterWidget = m_pEngineService->newMainWidget();
    setCentralWidget(m_pCenterWidget);
    CReceiverTextEdit *newReceiverTextEdit = new CReceiverTextEdit(this);
    newReceiverTextEdit->setMaxMsgSize(500);
    CServiceManager::RegistService("mainwindow." + newReceiverTextEdit->objectName(), newReceiverTextEdit);
    const CReceiverFlags msgReceiverFlags =
        {
            {BSMessage::msfInfo, {tr("Info"), CParaManager::getVarColor(cMsgNormalColor)}},
            {BSMessage::msfWarning, {tr("Warning"), CParaManager::getVarColor(cMsgWarningColor)}},
            {BSMessage::msfHint, {tr("Hint"), CParaManager::getVarColor(cMsgHintColor)}}};
    auto pReceiver = new CReceiverTextEditReceiver(newReceiverTextEdit);
    pReceiver->regMsgType(msgReceiverFlags);
    CMsgManager::attachUiReceiver(pReceiver, newReceiverTextEdit);

    auto *tabWidget = new CCustomTabWidget(this);

    tabWidget->addTab(newReceiverTextEdit, tr("System Info"));
    tabWidget->addTab(bsAlarmBox()->newAlarmInfoWidget(tabWidget), tr("Alarm Info"));

    m_pLogDockWidget = new QDockWidget(this);
    m_pLogDockWidget->setTitleBarWidget(new QWidget(this));
    m_pLogDockWidget->setWidget(tabWidget);
    m_pLogDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_pLogDockWidget);

    m_pLogDockWidget->setFixedHeight(150);
    setFixedSize(800, 800);

    connect(newReceiverTextEdit, &CReceiverTextEdit::sigDoubleClicked, this, &CVisionMainWindow::showLogWidget, Qt::QueuedConnection);
}

void CVisionMainWindow::onlyShowCenterWidget(bool p_only)
{
    if (p_only)
    {
        if (m_titleBar != nullptr)
        {
            m_titleBar->hide();
        }
        m_pLogDockWidget->hide();
    }
    else
    {
        if (m_titleBar != nullptr)
        {
            m_titleBar->show();
        }
        m_pLogDockWidget->show();
    }
}

void CVisionMainWindow::showLogWidget()
{
    auto *pLogDialog = static_cast<BS_QT_LogReceiver::CLogDialog *>(BS_QT_Ui::CUIManager::GetUIObject(LogDialog));
    if (pLogDialog == nullptr)
    {
        auto pLogService = qobject_cast<BS_QT_LogReceiver::ILogReceiver *>(CServiceManager::GetService("bsLogService"));
        if (pLogService != nullptr)
        {
            pLogDialog = new BS_QT_LogReceiver::CLogDialog(pLogService, this);
            BS_QT_Ui::CUIManager::InsertObjectInfo(LogDialog, pLogDialog);
        }
        else
        {
            Out_Info << tr("The service bsLogService is not registered, so it cannot display logs.");
            return;
        }
    }

    if (pLogDialog != nullptr && !pLogDialog->isVisible())
    {
        pLogDialog->raise();
        pLogDialog->show();
        BS_QT_Ui::CUIManager::moveToCenter(pLogDialog);
        pLogDialog->updateLogs();
    }
}
