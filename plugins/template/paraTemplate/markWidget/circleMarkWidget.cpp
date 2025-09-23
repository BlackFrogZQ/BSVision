#include "circleMarkWidget.h"
#include "../paraTemplateMarkPara.h"

#include "Ui/widget/spinBox.h"
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
namespace BS_2DVision_Template
{
    using namespace BS_QT_Ui;
    CCircleTemplateMarkParaWidget::CCircleTemplateMarkParaWidget(QWidget *p)
        : QFrame(p)
    {
        initLayout();
    }

    void CCircleTemplateMarkParaWidget::attachMarkPara(CCircleParas *pMarkPara)
    {
        assert(pMarkPara != nullptr);
        disconnect(m_pDiameterSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, nullptr);
        disconnect(m_pWidthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, nullptr);
        m_paras = pMarkPara;
        m_pOutsideCheckBox->setCheckable(m_paras->priorityOuterContour);
        m_pDiameterSpinBox->setValue(m_paras->d);
        m_pWidthSpinBox->setValue(m_paras->width);
        connect(m_pDiameterSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double p)
                { m_paras->d = p; });
        connect(m_pWidthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double w)
                { m_paras->width = w; });
    }

    void CCircleTemplateMarkParaWidget::initLayout()
    {
        m_pOutsideCheckBox = new QCheckBox(tr("Prioritize Outer Contour"), this);
        m_pOutsideCheckBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        m_pDiameterSpinBox = new CCustomDoubleSpinBox(this);
        m_pDiameterSpinBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        m_pDiameterSpinBox->setPrefix("Φ:");
        m_pDiameterSpinBox->setDecimals(3);
        m_pDiameterSpinBox->setRange(0, 9999);
        m_pDiameterSpinBox->setAlignment(Qt::AlignCenter);
        m_pDiameterSpinBox->setSuffix("mm");

        m_pWidthSpinBox = new CCustomDoubleSpinBox(this);
        m_pWidthSpinBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        m_pWidthSpinBox->setPrefix("w:");
        m_pWidthSpinBox->setRange(0, 9999);
        m_pWidthSpinBox->setDecimals(3);
        m_pWidthSpinBox->setAlignment(Qt::AlignCenter);
        m_pWidthSpinBox->setSuffix("mm");

        QGridLayout *pLayout = new QGridLayout(this);
        pLayout->addWidget(m_pDiameterSpinBox, 0, 0, 1, 1);
        pLayout->addWidget(m_pWidthSpinBox, 0, 1, 1, 1);
        pLayout->addWidget(m_pOutsideCheckBox, 1, 0, 1, 2, Qt::AlignCenter);
        pLayout->setMargin(0);
        pLayout->setSpacing(3);
    }
}