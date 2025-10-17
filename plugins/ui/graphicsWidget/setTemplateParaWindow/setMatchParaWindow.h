#pragma once
#include "setTemplateParaWindow.h"
struct CMatchTemplateParas;
class QDoubleSpinBox;
class QSpinBox;
class QComboBox;
class QRadioButton;
class CSetMatchParaWindow : public ISetTemplatePParaWindow
{
public:
    using ISetTemplatePParaWindow::ISetTemplatePParaWindow;
    void setMatchParas(CMatchTemplateParas *pMatchParas);

protected:
    virtual QLayout *initPataLayout() override;
    virtual void initPatas() override;
    virtual void updatePatas() override;

protected:
    CMatchTemplateParas *m_pMatchParas = nullptr;
    QDoubleSpinBox *m_pMaxOverlapDSB;
    QDoubleSpinBox *m_pGreedinessDSB;
    QSpinBox *m_pNumLevelsSB;
    QComboBox *m_pSubPixelTypeCB;
};