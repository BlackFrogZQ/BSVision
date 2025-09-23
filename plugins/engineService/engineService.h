#pragma once
#include <QObject>
class QWidget;
#define IEngineService_ObjectName "engineService"
#define IEngineService_Interface_iid "BSVision.Plugin.EngineInterface"

class IEngineService
{
public:
    virtual ~IEngineService() {};
    virtual bool init() = 0;
    virtual void release() = 0;

    virtual QWidget *newMainWidget(QWidget *p = nullptr) = 0;
    virtual void sigOnlyShow(bool p_only) = 0;

    virtual void slotUpdateParas() = 0;
};
Q_DECLARE_INTERFACE(IEngineService, IEngineService_Interface_iid)
