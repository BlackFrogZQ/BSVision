#pragma once
#include "Ui/widget/mainWindow.h"
class IEngineService;
class QDockWidget;
class CVisionMainWindow : public BS_QT_Ui::CCustomMainWindow
{
    Q_OBJECT
public:
    CVisionMainWindow(QWidget *p = nullptr);
    ~CVisionMainWindow();

public slots:
    void onlyShowCenterWidget(bool p_only);

protected:
    void init();

protected slots:
    void showLogWidget();

protected:
    IEngineService *m_pEngineService = nullptr;
    QWidget* m_pCenterWidget = nullptr;
    QDockWidget* m_pLogDockWidget = nullptr;
};
