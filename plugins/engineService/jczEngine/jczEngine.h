#pragma once
#include "../engineService.h"
class CJczEnginePlugin : public QObject, public IEngineService
{
    Q_OBJECT
    Q_INTERFACES(IEngineService)
    Q_PLUGIN_METADATA(IID IEngineService_Interface_iid)
public:
    explicit CJczEnginePlugin(QObject *parent = 0);
    ~CJczEnginePlugin();
    Q_INVOKABLE virtual bool init() override;
    Q_INVOKABLE virtual void release() override;
    virtual QWidget *newMainWidget(QWidget *p = nullptr) override;

public slots:
    virtual void slotUpdateParas() override;
    void newFile();
    void importFile();
    bool exportFile();
    void exitMsgBoxSys();

signals:
    void sigOnlyShow(bool p_only);
};
