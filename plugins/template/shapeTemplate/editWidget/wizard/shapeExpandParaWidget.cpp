#include "shapeExpandParaWidget.h"

#include "shapeDef.h"

#include "Ui/widget/spinBox.h"
#include "Ui/widget/rangeSlider.h"
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>
namespace BS_2DVision_Template
{
    CShapeExpandParaWidget::CShapeExpandParaWidget(QWidget *pWidget)
    {
        auto newDoubleSpinBox = [this](double p_min, double p_max, double p_value, double p_step, int p_decimals)
        {
            auto pSpinBox = new BS_QT_Ui::CCustomDoubleSpinBox(this);
            pSpinBox->setAlignment(Qt::AlignCenter);
            pSpinBox->setDecimals(p_decimals);
            pSpinBox->setSingleStep(p_step);
            pSpinBox->setRange(p_min, p_max);
            pSpinBox->setValue(p_value);
            return pSpinBox;
        };

        auto newRangeSpinBox = [this](double p_min, double p_max, double p_valueLower, double p_valueUpper, double p_step, int p_decimals)
        {
            auto pSlider = new BS_QT_Ui::CCustomRangeSlider(Qt::Horizontal, BS_QT_Ui::CCustomRangeSlider::DoubleHandles, this);
            pSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            pSlider->setDecimals(p_decimals);
            pSlider->setSingleStep(p_step);
            pSlider->setRange(p_min, p_max);
            pSlider->setLowerValue(p_valueLower);
            pSlider->setUpperValue(p_valueUpper);
            return pSlider;
        };

        m_pAngleRange = newRangeSpinBox(0, 360, 0, 360, 0.5, 1);
        m_pAngleStep = newDoubleSpinBox(0.5, 180, 0.5, 0.5, 1);
        m_pAngleStep->setPrefix(tr("Step") + ":");
        m_pScaleRange = newRangeSpinBox(0.5, 1.5, 0.98, 1.02, 0.01, 2);
        m_pScaleStep = newDoubleSpinBox(0.01, 1.0, 0.01, 0.01, 2);
        m_pScaleStep->setPrefix(tr("Step") + ":");

        m_pUseAngle = new QCheckBox(tr("Use Angle"), this);
        m_pUseAngle->setCheckable(true);
        connect(m_pUseAngle, &QCheckBox::toggled, this, [this](bool checked)
                {
                    if (m_pShapeParas != nullptr)
                    {
                        m_pShapeParas->useAngle = checked;
                    }

                    m_pAngleRange->setEnabled(checked);
                    m_pAngleStep->setEnabled(checked); });

        m_pUseSacle = new QCheckBox(tr("Use Sacle"), this);
        m_pUseSacle->setCheckable(true);
        connect(m_pUseSacle, &QCheckBox::toggled, this, [this](bool checked)
                {
                    if (m_pShapeParas != nullptr)
                    {
                        m_pShapeParas->useScale = checked;
                    }
                    m_pScaleRange->setEnabled(checked);
                    m_pScaleStep->setEnabled(checked); });

        m_pNumFeaturesSB = newDoubleSpinBox(5, 500, 20, 5, 0);
        m_pNumFeaturesSB->setPrefix(tr("Min Features") + ":");

        QGridLayout *pLayout = new QGridLayout(this);
        pLayout->setMargin(20);
        pLayout->setSpacing(2);

        pLayout->addWidget(m_pNumFeaturesSB, 0, 0, 1, 1);

        pLayout->addWidget(m_pUseAngle, 1, 0, 1, 1);
        pLayout->addWidget(m_pAngleStep, 1, 1, 1, 1);
        pLayout->addWidget(m_pAngleRange, 2, 0, 1, 3);

        pLayout->addWidget(m_pUseSacle, 3, 0, 1, 1);
        pLayout->addWidget(m_pScaleStep, 3, 1, 1, 1);
        pLayout->addWidget(m_pScaleRange, 4, 0, 1, 3);
    }

    void CShapeExpandParaWidget::setParas(CCreateTemplateParas *pShapeParas)
    {
        m_pShapeParas = pShapeParas;
        m_pNumFeaturesSB->setValue(pShapeParas->numFeatures);
        m_pUseAngle->setChecked(pShapeParas->useAngle);
        m_pUseSacle->setChecked(pShapeParas->useScale);
        m_pAngleRange->setLowerValue(pShapeParas->startAngle);
        m_pAngleRange->setUpperValue(pShapeParas->startAngle + pShapeParas->angleExtent);
        m_pScaleRange->setLowerValue(pShapeParas->scaleMin);
        m_pScaleRange->setUpperValue(pShapeParas->scaleMax);
    }
}
