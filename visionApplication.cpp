#include "visionApplication.h"
#include "visionMainWindow.h"

#include "para/paraManager.h"
#include "message/msgManager.h"
#include "service/serviceManager.h"
#include "service/enumMap.h"
#include "logReceiver/logReceiver.h"
#include "Ui/uiManager.h"
#include "Ui/widgetJson.h"
#include "Ui/widget/mainAlarmBox.h"

#include "fileHelper/appHelper.h"
#include "fileHelper/fileHelper.h"
#include "fileHelper/jsomHelper.h"
#include "user/userHelper.h"
#include "dongleHelperCreater.h"
#include "ftsofe/ftsofeDongle.h"
#include "gitVersion.h"
#include "version.h"

#include "plugins/pluginCoreDef.h"
#include "plugins/engineService/engineService.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDialogButtonBox>
#include <QDesktopWidget>
RegisterMetaEnum(CVisionApplication, CLangType);
RegisterMetaEnum(CVisionApplication, CEngineType);
using namespace BS_QT_Framework;
using namespace BS_QT_FlieHelper;
using namespace BS_QT_Service;
using namespace BS_QT_Message;
using namespace BS_QT_Ui;

CUiAlarm sDongleAlarmAlarm = {"CVisionApplication::cDongleAlarmAlarm", true, QObject::tr("Encryption has expired, please contact the administrator."), QObject::tr("Detailed Info"), cBsAppService, "showDongleialog()"};
CUiAlarm sDongleTimeAlarm = {"CVisionApplication::cDongleTimeAlarm", false};
const QString &cSysParamWndName = "sysParamDialog";

bool CVisionApplication::initParaManager()
{
    CGitVersion::attach(realVersion());
    CParaManager::setFilePath(CAppFileHelper::configDirPath() + "/" + CParaManager::GetValue<QString>(cCoreParaFile));
    CParaManager::AddParaInfo(CAppFileHelper::appDirPath() + "/framework.ParaDef.json");
    CParaManager::AddParaInfo(CAppFileHelper::appDirPath() + "/user.ParaDef.json");
    CParaManager::load();
    return true;
}

bool CVisionApplication::initTranslation()
{
    auto appLang = CParaManager::GetValue<QString>(cAppLang, "English");
    auto files = CFileHelper::scanfDir(CAppFileHelper::langDirPath(), QStringList() << QString("*.%1.qm").arg(appLang), QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name);
    for (auto file : files)
    {
        QTranslator *translator = new QTranslator;
        translator->load(file);
        qApp->installTranslator(translator);
    }
    return true;
}

bool CVisionApplication::initMsgManager()
{
    CMsgManager::init(CParaManager::GetValue<int>(cCoreMsgTimeOut, 200));

    auto pLog = BS_QT_LogReceiver::createLogReceiver(BS_QT_LogReceiver::ILogReceiver::YAOLOG, CAppFileHelper::loggerDirPath(), QDateTime::currentDateTime().toString("yyyy-MM-dd"), "log");
    const BS_QT_Message::CReceiverFlags msgReceiverFlags =
        {
            {BSMessage::msfLog, {"Log", CParaManager::getVarColor(cMsgNormalColor)}},
            {BSMessage::msfInfo, {"Info", CParaManager::getVarColor(cMsgNormalColor)}},
            {BSMessage::msfWarning, {"Warning", CParaManager::getVarColor(cMsgWarningColor)}},
            {BSMessage::msfError, {"Error", CParaManager::getVarColor(cMsgErrorColor)}},
            {BSMessage::msfHint, {"Hint", CParaManager::getVarColor(cMsgHintColor)}}};
    pLog->regMsgType(msgReceiverFlags);
    pLog->removeObsoleteLog(CParaManager::GetValue<int>(cLogMaxSize, 365));
    CMsgManager::attachReceiver(pLog);
    CServiceManager::RegistService("bsLogService", pLog);
    return true;
}

bool CVisionApplication::registPluginService()
{
    using namespace BS_Hal_Donlge;
    // 路径环境
    auto cEnginePluginDir = CAppFileHelper::appDirPath() + "/plugins";
    QStringList originalPaths;
    originalPaths << cEnginePluginDir;
    originalPaths << cEnginePluginDir + "/Camera";
    originalPaths << cEnginePluginDir + "/visionEngine";
    qputenv("PATH", originalPaths.join(';').toStdString().c_str());

    qputenv("GENICAM_LOG_CONFIG_V3_1", (cEnginePluginDir + "/Camera/log.txt").toStdString().c_str());

    // 插件
    // addLibraryPath 只对插件有效，对插件所依赖的dll无效
    qApp->addLibraryPath(cEnginePluginDir);
    CParaManager::SetValue(cPluginDir, cEnginePluginDir);
    // 加载失败可能是第三方dll没有扫描到
    QString enginePluginName;
    CFtsofeInitPara ftsofeInitPara;
    switch (CEnumManager::getEnumInfo<CEngineType>(CParaManager::GetValue<QString>(cEngineType), HZGEngine))
    {
    case HZGEngine:
        enginePluginName = "hzgEngine.dll";
        ftsofeInitPara.PID = 0xDBA58FD4;
        break;
    case JCZEngine:
        enginePluginName = "jczEngine.dll";
        ftsofeInitPara.PID = 0xDBA58FD4;
        break;
    default:
        break;
    }
    ftsofeInitPara.currentVer = CGitVersion::getVersion();
    //
    // ftsofeInitPara.Type = 0x00;
    // ftsofeInitPara.DevType = 0x00;
    // ftsofeInitPara.HID = 0x3917EB1A2B011234;
    // ftsofeInitPara.UID = 0xFFFFFFFF;
    CDongleHelperCreater::createDongleHelper(IDongleHelper::FtsofeDongle, QVariant(ftsofeInitPara));
    bsDongleHelper()->init();
    connect(
        bsDongleHelper(), &IDongleHelper::sigEmpowerChanged, this, [this](bool p_isEmpower)
        {
            if (p_isEmpower)
            {
                bsAlarmBoxRemoveAlarm(sDongleAlarmAlarm);
                auto secsCount = bsDongleHelper()->secsCount();
                if (secsCount >= 0)
                {
                    int day = 0;
                    int hour = 0;
                    int minute = 0;
                    BS_Hal_Donlge::IDongle::formatTime(secsCount,day,hour,minute);
                    // 参数化
                    if ((day+1) < 200)
                    {
                        sDongleTimeAlarm.msg = tr("There is %1 day, %2 hours and %3 minutes left in the authorization time. Please contact the administrator in time to authorize.").arg(day).arg(hour).arg(minute);
                        sDongleTimeAlarm.dateTime = QDateTime::currentDateTime();
                        bsAlarmBoxAddAlarm(sDongleTimeAlarm);
                    }
                }
            }
            else
            {
                bsAlarmBoxRemoveAlarm(sDongleTimeAlarm);
                sDongleAlarmAlarm.dateTime = QDateTime::currentDateTime();
                bsAlarmBoxAddAlarm(sDongleAlarmAlarm);
            } },
        Qt::QueuedConnection);

    return CServiceManager::RegistPluginService(enginePluginName, IEngineService_ObjectName);
}

bool CVisionApplication::initUi()
{
    CUIManager::addUiInfo(CAppFileHelper::appDirPath() + "/messageBox.uidef.json");
    CUIManager::addUiInfo(CAppFileHelper::appDirPath() + "/mainWindow.uidef.json");
    BS_QT_Ui::CCustomMainWindow *p = new CVisionMainWindow;
    // 标题栏
    {
        auto mainwindowInfo = CUIManager::getUIInfo("mainwindow", "mainwindow");
        CUIManager::registMainWindow(mainwindowInfo[cObjectName], p);
        mainwindowInfo.insert(cWndName, "mainwindow");
        mainwindowInfo.insert(cTopParaName, "mainwindow");
        CJsonWidgetCreater::loadMainWindow(mainwindowInfo, p);
        CUIManager::registUiLogicMap(CAppFileHelper::appDirPath() + "/mainWindow.serviceMapDef.json");
    }
    // 用户
    using namespace BS_User;
    CUserHelper::loadUserInfo();
    CUserHelper::attachFacturerInfo(QList<CUserInfo>() << CUserInfo{"BS", "1149"});
    connect(
        CUserHelper::sendSignaler(), &CUserHelperSignals::sigUserChanged_name, this, [this](QString p_name)
        {
            auto pUserBtn = CUIManager::GetUIObject("mainwindow.TitleToolBar.UserBtn");
            if (pUserBtn != nullptr)
            {
                pUserBtn->setProperty("text",QObject::tr(p_name.toUtf8()));
            } },
        Qt::QueuedConnection);

    m_showLoginOnceConnection = connect(
        p, &BS_QT_Ui::CCustomMainWindow::sigShow, this, [this]()
        {
            disconnect(m_showLoginOnceConnection);
            auto currId = CUserHelper::currentUser();
            CUserHelper::setCurrentUser(currId);
            if(currId<0)
            {
                showLoginDialog();
            } },
        Qt::QueuedConnection);

    auto createSysParamWnd = []
    {
        auto paramDlg = CUIManager::GetUIObject(cSysParamWndName);
        if (paramDlg == nullptr)
        {
            CUIManager::addUiInfo(CAppFileHelper::appDirPath() + "/sysParamDialog.uidef.json");
            CUIInfo uiInfo = CUIManager::getUIInfo(cSysParamWndName, cSysParamWndName);
            uiInfo.insert(cWndName, cSysParamWndName);
            uiInfo.insert(cTopParaName, cSysParamWndName);
            paramDlg = CJsonWidgetCreater::NewWidget(uiInfo, CUIManager::curMainWnd());
            CUIManager::registUiLogicMap(CAppFileHelper::appDirPath() + "/sysParamDialog.serviceMapDef.json");
            CServiceManager::BindMetaMethod("sysParamDialog.sysParaWidget", "sigUpdateSavePara()", IEngineService_ObjectName, "slotUpdateParas()", Qt::QueuedConnection);
        }
    };
    createSysParamWnd();
    return true;
}

bool CVisionApplication::initStateManager()
{
    return true;
}

bool CVisionApplication::checkRunEnvironment()
{
    return true;
}

void CVisionApplication::registRcc()
{
    CUIManager::registRcc(CAppFileHelper::appDirPath() + "/res.rcc");
    CUIManager::registQss(CAppFileHelper::appDirPath() + "/UiStyle.qss");
    CUIManager::addRegistQss(CAppFileHelper::appDirPath() + "/UiStyle.camera.qss");
}

#include "QRCode/qrcode.h"
#include "Ui/widget/messageBox.h"
#include <QMessageBox>
#include <QPushButton>
void CVisionApplication::aboutSys()
{
    const QString &cAboutWndName = "CVisionApplication_AboutDialog_Slot_Dialog";
    auto pObjDlg = CUIManager::GetUIObject(cAboutWndName);
    if (pObjDlg == nullptr)
    {
        auto appName = CParaManager::GetValue<QString>(cCoreCheckAppName);
        QString detailedText = CGitVersion::getToolTip();
        auto pBox = new CCustomMessageBox(CUIManager::curMainWnd());
        CUIManager::InsertObjectInfo(cAboutWndName, pBox);
        pBox->setStandardButtons(QMessageBox::StandardButton::Ok);
        auto pDongle = pBox->addButton(tr("Authorization"), QMessageBox::YesRole);
        connect(pDongle, &QPushButton::clicked, this, &CVisionApplication::showDongleialog, Qt::QueuedConnection);
        pBox->setIconPixmap(QPixmap::fromImage(BS_QT_QRCode::CQRcodeHelper::encode(CGitVersion::getDetailedInfo(), QSize(150, 150), QImage(cAppIcon))));
        pBox->setWindowTitle("About " + appName);
        pBox->setText(appName + " V" + CGitVersion::getVersion().toString());
        pBox->setInformativeText(QString("Copyright (c) 2023 %1. All rights reserved.").arg("BSVision"));
        pBox->setDetailedText(CGitVersion::getDetailedInfo());
        pBox->adjustSize();
        pObjDlg = pBox;
    }

    if (pObjDlg != nullptr)
    {
        auto pDlg = static_cast<QDialog *>(pObjDlg);
        CUIManager::moveToCenter(pDlg);
        pDlg->exec();
    }
}

void CVisionApplication::showParamDialog()
{
    auto paramDlg = CUIManager::GetUIObject(cSysParamWndName);
    if (paramDlg != nullptr)
    {
        auto pParamDlg = static_cast<QDialog *>(paramDlg);
        CUIManager::moveToCenter(pParamDlg);
        pParamDlg->exec();
    }
}

#include "user/userLoginDialog.h"
void CVisionApplication::showLoginDialog()
{
    const QString &cUserLoginDialogWndName = "UserLoginDialogWndName";
    auto loginDlg = CUIManager::GetUIObject(cUserLoginDialogWndName);
    if (loginDlg == nullptr)
    {
        loginDlg = new BS_User::CLoginDialog;
        CUIManager::InsertObjectInfo(cUserLoginDialogWndName, loginDlg);
    }

    if (loginDlg != nullptr)
    {
        auto ploginDlg = static_cast<QDialog *>(loginDlg);
        BS_User::CUserHelper::setCurrentUser(BS_User::UnLogin);
        ploginDlg->adjustSize();
        CUIManager::moveToCenter(ploginDlg);
        ploginDlg->exec();
    }
}

void CVisionApplication::showDongleialog()
{
    const QString &cDongleInfoDialog = "dongleInfoDialog";
    auto paramDlg = CUIManager::GetUIObject(cDongleInfoDialog);
    if (paramDlg == nullptr)
    {
        paramDlg = bsDongleHelper()->newInfoWidget(CUIManager::curMainWnd());
        CUIManager::InsertObjectInfo(cDongleInfoDialog, paramDlg);
    }

    if (paramDlg != nullptr)
    {
        auto pParamDlg = static_cast<QDialog *>(paramDlg);
        pParamDlg->adjustSize();
        CUIManager::moveToCenter(pParamDlg);
        pParamDlg->exec();
    }
}
