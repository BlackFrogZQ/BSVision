#include "jczEngine.h"
#include "widget/mainWidget.h"

#include "Ui/uiManager.h"
#include "Ui/widgetJson.h"
#include "Ui/widget/mainAlarmBox.h"

#include "para/paraManager.h"
#include "message/msgUiAlarm.h"
#include "tool/saveFile.h"
#include "desktopServices/urlHandler/fileInExplorerUrl.h"
#include "qssDef.h"

#include "system/basic.h"
#include "template/templateCreater.h"
#include "template/templateHelper.h"

#include "iApplication.h"
#include <QLabel>
#include <QCoreApplication>
#include <QDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolTip>
using namespace BS_QT_Ui;
using namespace BS_QT_Framework;

CJczEnginePlugin::CJczEnginePlugin(QObject *parent)
    : QObject(parent), IEngineService()
{
}

CJczEnginePlugin::~CJczEnginePlugin()
{
}

bool CJczEnginePlugin::init()
{
    initCamera();
    BS_2DVision_Template::CTemplateCreater::createTemplate();
    return CUIManager::addRegistQss(CParaManager::GetValue<QString>(cPluginDir) + "/UiStyle.jcz.qss");
}

void CJczEnginePlugin::release()
{
    releaseCamera();
}

QWidget *CJczEnginePlugin::newMainWidget(QWidget *p)
{
    auto pWidget = new CJczMainWidget(p);
    pWidget->init();
    pWidget->setMinimumHeight(500);
    connect(pWidget, &CJczMainWidget::sigOnlyShow, this, &CJczEnginePlugin::sigOnlyShow, Qt::QueuedConnection);
    return pWidget;
}

void CJczEnginePlugin::slotUpdateParas()
{
    updateCameraParas();
}

void CJczEnginePlugin::newFile()
{
    jczMainWindow()->onlyShowTabWidget(true,jczMainWindow()->m_pWorkWindow);
}

void CJczEnginePlugin::importFile()
{
    QFileInfo fileInfo(templateHelper()->m_currFile);
    QString fileName = QFileDialog::getOpenFileName(jczMainWindow(), tr("Import File"), fileInfo.absolutePath(), "jcz " + tr("vision files") + "(*.jczbscv)");
    if (!fileName.isEmpty())
    {
        if (templateHelper()->load(fileName))
        {
            Out_SyncHInt << tr("Import file(%1) successfully.").arg(fileName);
        }
        else
        {
            Out_Warn_MsgBox << tr("Import failed. file:%1. error:%2")
                                       .arg(fileName)
                                       .arg(templateHelper()->errorMsg()) +
                                   ".";
        }
    }
}

bool CJczEnginePlugin::exportFile()
{
    QFileInfo fileInfo(templateHelper()->m_currFile);
    QString fileName = QFileDialog::getSaveFileName(jczMainWindow(), tr("Export File"), fileInfo.absoluteFilePath(), "jcz " + tr("vision files") + "(*.jczbscv)");
    if (!fileName.isEmpty())
    {
        if (templateHelper()->saveAs(fileName))
        {
            fileInfo.setFile(fileName);
            Out_SyncHInt << tr("Export file successfully. Path:%2/%1.")
                                .arg(fileInfo.fileName())
                                .arg(CBSToolFun::geFileLinkUrl(
                                    BS_QT_UrlHandler::CFileInExplorerUrlHandle::scheme_static(), fileInfo.absolutePath(),
                                    fileInfo.absoluteFilePath(),
                                    CParaManager::getVarColor(cMsgHintColor)));
            return true;
        }
        else
        {
            Out_Warn_MsgBox << tr("Export failed. file:%1.").arg(fileName);
        }
    }
    return false;
}

void CJczEnginePlugin::exitMsgBoxSys()
{
    if (templateHelper()->isChanged())
    {
        auto rtVal = CJsonWidgetCreater::ShowMsgBox(cSystemUnsavedMsgBox, CUIManager::curMainWnd());
        switch (rtVal)
        {
        case QMessageBox::Yes:
        case QMessageBox::Ok:
        case QMessageBox::Save:
        {
            auto file = templateHelper()->m_currFile;
            if (file.isEmpty())
            {
                if (!exportFile())
                {
                    return;
                }
            }
            else
            {
                if (!templateHelper()->saveAs(file))
                {
                    Out_Warn_MsgBox << QObject::tr("Save failed. file:%1.").arg(file);
                    return;
                }
                Out_SyncLog << tr("Save file(%1) successfully.").arg(file);
            }
        }
        break;
        case QMessageBox::Cancel:
            return;
            break;

        default:
            break;
        }
        bsApp->exitSys();
    }
    else
    {
        bsApp->exitMsgBoxSys();
    }
}