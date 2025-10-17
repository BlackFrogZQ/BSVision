#pragma once
#include "setTemplateParaWindow.h"
struct CCreateTemplateParasOld;
class QDoubleSpinBox;
class QSpinBox;
class QComboBox;
class QRadioButton;
class CSetCreateParaWindow : public ISetTemplatePParaWindow
{
public:
    using ISetTemplatePParaWindow::ISetTemplatePParaWindow;
    void setCreateParas(CCreateTemplateParasOld *pCreateParas);

protected:
    virtual QLayout *initPataLayout() override;
    virtual void initPatas() override;
    virtual void updatePatas() override;

protected:
    CCreateTemplateParasOld *m_pCreateParas = nullptr;
    QDoubleSpinBox *m_pStartAngle;
    QDoubleSpinBox *m_pStartExtent;
    QDoubleSpinBox *m_pStartStep;
    QRadioButton *m_pUseSacle;
    QDoubleSpinBox *m_pScaleMin;
    QDoubleSpinBox *m_pScaleMax;
    QDoubleSpinBox *m_pScaleStep;
    QSpinBox *m_pNumFeaturesSB;
    QComboBox *m_pOptimizationTypeCB;
    QComboBox *m_pPolarityTypeCB;
};