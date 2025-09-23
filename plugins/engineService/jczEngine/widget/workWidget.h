#pragma once
#include "markWidget.h"
#include "ui/workWidget/measureWorkWidget.h"
class CJczWorkWidget : public CMeasureWorkWidget
{
    Q_OBJECT
public:
    CJczWorkWidget(QWidget *p_pParent = nullptr);
    ~CJczWorkWidget();
    virtual void init() override;
    virtual void initLayout() override;

protected:
    virtual void initToolBar() override;

public:
    CJczMarkWidget *m_pMarkWidget = nullptr;
    QToolButton *m_pMarkWidgetBtn = nullptr;
};

CJczWorkWidget *jczWorkWindow();
