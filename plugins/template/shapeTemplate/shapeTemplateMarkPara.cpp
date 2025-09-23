#include "shapeTemplateMarkPara.h"
#include "service/typeRegistrar.h"
RegisterType(BS_2DVision_Template, CShapeTemplateMarkParas);
RegStreamOperator(BS_2DVision_Template, CShapeTemplateMarkParas);
QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CShapeTemplateMarkParas &paras)
{
    uint16_t ver = 0;
    uint32_t flag = qHash(QString("BS_2DVision_Template::CShapeTemplateMarkParas"));
    arch
        << flag
        << ver;
    arch << paras.usedGlobalCamPara
         << paras.modelId
         << paras.minScore;
    return arch;
}

QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CShapeTemplateMarkParas &paras)
{
    uint32_t flag = 0;
    arch >> flag;
    if (flag == qHash(QString("BS_2DVision_Template::CShapeTemplateMarkParas")))
    {
        uint16_t ver = 0;
        arch >> ver;
        switch (ver)
        {
        case 0:
            arch >> paras.usedGlobalCamPara >> paras.modelId >> paras.minScore;
            break;
        default:
            arch.setStatus(QDataStream::ReadCorruptData);
            break;
        }
    }
    else
    {
        arch.setStatus(QDataStream::ReadCorruptData);
    }
    return arch;
}

#include "Ui/widget/comboBox.h"
#include "Ui/widget/spinBox.h"
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
namespace BS_2DVision_Template
{
    CShapeTemplateMarkParas::CShapeTemplateMarkParas(bool p_usedGlobal, int p_modelId)
        : usedGlobalCamPara(p_usedGlobal),
          modelId(p_modelId)
    {
    }

    CShapeTemplateMarkParas::CShapeTemplateMarkParas(const QVariant &p_var)
    {
        if (p_var.canConvert<BS_2DVision_Template::CShapeTemplateMarkParas>())
        {
            *this = p_var.value<BS_2DVision_Template::CShapeTemplateMarkParas>();
        }
    }

    CShapeTemplateMarkParas::operator QVariant() const
    {
        return QVariant::fromValue(*this);
    }

    CShapeTemplateMarkParas &CShapeTemplateMarkParas::operator=(const QVariant &p_var)
    {
        if (p_var.canConvert<BS_2DVision_Template::CShapeTemplateMarkParas>())
        {
            *this = p_var.value<BS_2DVision_Template::CShapeTemplateMarkParas>();
        }
        return *this;
    }
}
#include "Ui/widget/spinBox.h"
namespace BS_2DVision_Template
{
    using namespace BS_QT_Ui;

    CShapeTemplateMarkParaWidget::CShapeTemplateMarkParaWidget(QWidget *p)
        : IMarkTemplateParaWidget(p)
    {
        initLayout();
    }

    void CShapeTemplateMarkParaWidget::attachMarkPara(QVariant *pMarkPara)
    {
        disconnect(m_pUsedGlobalCheckBox, &QCheckBox::toggled, this, nullptr);
        disconnect(m_pUsedModelIDSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, nullptr);
        disconnect(m_pMinScoreSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, nullptr);

        if (!pMarkPara->isValid() || !pMarkPara->canConvert<BS_2DVision_Template::CShapeTemplateMarkParas>())
        {
            *pMarkPara = CShapeTemplateMarkParas();
        }

        IMarkTemplateParaWidget::attachMarkPara(pMarkPara);

        m_paras = (BS_2DVision_Template::CShapeTemplateMarkParas *)(pMarkPara->data());

        m_pUsedGlobalCheckBox->setChecked(m_paras->usedGlobalCamPara);
        m_pMinScoreSpinBox->setValue(m_paras->minScore);
        connect(m_pUsedGlobalCheckBox, &QCheckBox::toggled, this, [this](bool p_on)
                { m_paras->usedGlobalCamPara = p_on; });
        connect(m_pUsedModelIDSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int p_id)
                { m_paras->modelId = p_id; });
        connect(m_pMinScoreSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double p)
                { m_paras->minScore = float(p); });
        m_pUsedModelIDSpinBox->setValue(m_paras->modelId);
    }

    void CShapeTemplateMarkParaWidget::slotModelCountChanged(int p_currCount, int p_oldCount)
    {
        m_pMinScoreSpinBox->setMaximum(p_currCount);
    }

    void CShapeTemplateMarkParaWidget::initLayout()
    {
        m_pMinScoreSpinBox = new BS_QT_Ui::CCustomDoubleSpinBox(this);
        m_pMinScoreSpinBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        m_pMinScoreSpinBox->setPrefix(tr("Score") + ":");
        m_pMinScoreSpinBox->setDecimals(2);
        m_pMinScoreSpinBox->setRange(0.01, 100);
        m_pMinScoreSpinBox->setAlignment(Qt::AlignCenter);
        m_pMinScoreSpinBox->setSuffix("");

        m_pUsedGlobalCheckBox = new QCheckBox(tr("Used Global Cam Para"), this);
        m_pUsedModelIDSpinBox = new BS_QT_Ui::CCustomSpinBox(this);
        m_pUsedModelIDSpinBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        m_pUsedModelIDSpinBox->setPrefix(tr("Model") + ":");
        m_pUsedModelIDSpinBox->setRange(1, 100);
        m_pUsedModelIDSpinBox->setAlignment(Qt::AlignCenter);
        m_pUsedModelIDSpinBox->setSuffix("");

        QGridLayout *pLayout = new QGridLayout(this);
        pLayout->addWidget(m_pUsedModelIDSpinBox, 0, 0, 1, 2);
        pLayout->addWidget(m_pMinScoreSpinBox, 0, 2, 1, 2);
        pLayout->addWidget(m_pUsedGlobalCheckBox, 1, 0, 1, 4);
        pLayout->setAlignment(m_pUsedGlobalCheckBox, Qt::AlignCenter);
        pLayout->setMargin(0);
        pLayout->setSpacing(3);
    }
}