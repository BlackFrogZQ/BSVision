#pragma once
#include <QFrame>
#include <QVariant>
#include "pluginCoreDef.h"

class QCheckBox;
class QRadioButton;
class QComboBox;
class QDoubleSpinBox;
class QStackedWidget;
namespace BS_2DVision_Template
{
    struct CCircleParas;
    class CCircleTemplateMarkParaWidget : public QFrame
    {
        Q_OBJECT

    public:
        CCircleTemplateMarkParaWidget(QWidget *p = nullptr);
        void attachMarkPara(CCircleParas *pCircleParas);

    protected:
        void initLayout();

    protected:
        CCircleParas *m_paras = nullptr;
        QCheckBox *m_pOutsideCheckBox = nullptr;
        QDoubleSpinBox *m_pDiameterSpinBox = nullptr;
        QDoubleSpinBox *m_pWidthSpinBox = nullptr;
    };
}