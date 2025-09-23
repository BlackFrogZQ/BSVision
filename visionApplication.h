#pragma once
#include "iApplication.h"

class CVisionApplication : public BS_QT_Framework::IApplication
{
    Q_OBJECT
public:
    enum CLangType : int32_t
    {
        Chinese,
        English
    };
    Q_ENUM(CLangType);

    enum CEngineType : int32_t
    {
        HZGEngine,
        JCZEngine,
    };
    Q_ENUM(CEngineType);

public:
    using BS_QT_Framework::IApplication::IApplication;

public slots:
    virtual void aboutSys() override;
    void showParamDialog();
    void showLoginDialog();
    void showDongleialog();

protected:
    virtual bool initParaManager() override;
    virtual bool initTranslation() override;
    virtual bool initMsgManager() override;
    virtual bool registPluginService() override;
    virtual bool initUi() override;
    virtual bool initStateManager() override;
    virtual bool checkRunEnvironment() override;
    virtual void registRcc() override;
    void sfds();

protected:
    QMetaObject::Connection m_showLoginOnceConnection;
};