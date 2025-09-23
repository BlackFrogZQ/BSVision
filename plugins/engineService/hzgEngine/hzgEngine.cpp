#include "hzgEngine.h"
#include "widget/mainWidget.h"

#include "Ui/uiManager.h"
#include "para/paraManager.h"
#include "qssDef.h"

#include "system/basic.h"
#include "iApplication.h"

#include <QLabel>
#include <QCoreApplication>
#include <QDialog>
#include <QMessageBox>
#include <QToolTip>
using namespace BS_QT_Ui;
using namespace BS_QT_Framework;

CHzgEnginePlugin::CHzgEnginePlugin(QObject *parent)
    : QObject(parent), IEngineService()
{
}

CHzgEnginePlugin::~CHzgEnginePlugin()
{
}

bool CHzgEnginePlugin::init()
{
    initCamera();
    return CUIManager::addRegistQss(CParaManager::GetValue<QString>(cPluginDir) + "/UiStyle.hzg.qss");
}

void CHzgEnginePlugin::release()
{
    releaseCamera();
}

QWidget *CHzgEnginePlugin::newMainWidget(QWidget *p)
{
    auto pWidget = new CHzgMainWidget(p);
    pWidget->init();
    pWidget->setMinimumHeight(500);
    return pWidget;
}

void CHzgEnginePlugin::newFile()
{
}

void CHzgEnginePlugin::slotUpdateParas()
{
    updateCameraParas();
}

void CHzgEnginePlugin::importFile()
{
}

bool CHzgEnginePlugin::exportFile()
{
    return false;
}

void CHzgEnginePlugin::exitMsgBoxSys()
{
    bsApp->exitMsgBoxSys();
}