#pragma once
#include "stdWorkWidget.h"
#include "ui/imageWidget/measureImageWidget.h"
class CMeasureWorkWidget : public CStdWorkWidget
{
    Q_OBJECT
public:
    using CStdWorkWidget::CStdWorkWidget;
    virtual void init() override;

protected:
    virtual void newImageWidget() override;
    virtual void initToolBar() override;
    virtual void imageValidChanged(bool p_valid) override;

public:
    CMeasureImageWidget *m_pMeasureImageWidget = nullptr;
    QToolButton *m_pLinkButton = nullptr;
    QToolButton *m_pMeasureButton = nullptr;
};
CMeasureWorkWidget *measureWorkWindow();
