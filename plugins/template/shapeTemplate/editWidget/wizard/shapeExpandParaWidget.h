#pragma once
#include <QWidget>
class QDoubleSpinBox;
class QSpinBox;
class QComboBox;
class QCheckBox;
namespace BS_QT_Ui
{
    class CCustomRangeSlider;
}

namespace BS_2DVision_Template
{
    struct CCreateTemplateParas;
    class CShapeExpandParaWidget : public QWidget
    {
        Q_OBJECT
    public:
        CShapeExpandParaWidget(QWidget *pWidget = nullptr);
        ~CShapeExpandParaWidget() = default;
        void setParas(CCreateTemplateParas *pShapeParas);

    protected:
        QCheckBox *m_pUseAngle = nullptr;
        BS_QT_Ui::CCustomRangeSlider *m_pAngleRange = nullptr;
        QDoubleSpinBox *m_pAngleStep = nullptr;

        QCheckBox *m_pUseSacle = nullptr;
        BS_QT_Ui::CCustomRangeSlider *m_pScaleRange = nullptr;
        QDoubleSpinBox *m_pScaleStep = nullptr;
        QDoubleSpinBox *m_pNumFeaturesSB = nullptr;
        CCreateTemplateParas *m_pShapeParas = nullptr;
    };
}
