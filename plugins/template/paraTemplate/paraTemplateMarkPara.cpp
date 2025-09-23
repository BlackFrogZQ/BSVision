#include "paraTemplateMarkPara.h"
#include "markWidget/circleMarkWidget.h"

#include "Ui/widget/stackedWidget.h"
#include "service/typeRegistrar.h"

#include <QStackedWidget>
#include <QCheckBox>
#include <QGridLayout>

RegisterType(BS_2DVision_Template, CParaTemplateParas);
RegStreamOperator(BS_2DVision_Template, CParaTemplateParas);
RegistEqualsComparator(BS_2DVision_Template, CParaTemplateParas);

RegisterType(BS_2DVision_Template, CCircleParas);
RegStreamOperator(BS_2DVision_Template, CCircleParas);
RegistEqualsComparator(BS_2DVision_Template, CCircleParas);

QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CCircleParas &paras)
{
    uint16_t ver = 0;
    uint32_t flag = qHash(QString("BS_2DVision_Template::CCircleParas"));
    arch
        << flag
        << ver;
    arch << paras.priorityOuterContour << paras.d << paras.width;
    return arch;
}

QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CCircleParas &paras)
{
    uint32_t flag = 0;
    arch >> flag;
    if (flag == qHash(QString("BS_2DVision_Template::CCircleParas")))
    {
        uint16_t ver = 0;
        arch >> ver;
        switch (ver)
        {
        case 0:
            arch >> paras.priorityOuterContour >> paras.d >> paras.width;
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

QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CParaTemplateParas &paras)
{
    uint16_t ver = 0;
    uint32_t flag = qHash(QString("BS_2DVision_Template::CParaTemplateParas"));
    arch
        << flag
        << ver;
    arch << int32_t(paras.type);
    arch << paras.circleParas;
    return arch;
}

QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CParaTemplateParas &paras)
{
    uint32_t flag = 0;
    arch >> flag;
    if (flag == qHash(QString("BS_2DVision_Template::CParaTemplateParas")))
    {
        uint16_t ver = 0;
        arch >> ver;
        switch (ver)
        {
        case 0:
            arch >> (int32_t &)paras.type;
            arch >> paras.circleParas;
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

namespace BS_2DVision_Template
{
    bool CCircleParas::operator==(const CCircleParas &other) const
    {
        return priorityOuterContour == other.priorityOuterContour &&
               qFuzzyIsNull(d - other.d) &&
               qFuzzyIsNull(width - other.width);
    }

    CParaTemplateParas::CParaTemplateParas(CParaTemplateDef::CTemplateType type)
        : type(type)
    {
    }

    CParaTemplateParas::CParaTemplateParas(const QVariant &p_var)
    {
        if (p_var.canConvert<BS_2DVision_Template::CParaTemplateParas>())
        {
            *this = p_var.value<BS_2DVision_Template::CParaTemplateParas>();
        }
    }

    CParaTemplateParas::operator QVariant() const
    {
        return QVariant::fromValue(*this);
    }

    CParaTemplateParas &CParaTemplateParas::operator=(const QVariant &p_var)
    {
        if (p_var.canConvert<BS_2DVision_Template::CParaTemplateParas>())
        {
            *this = p_var.value<BS_2DVision_Template::CParaTemplateParas>();
        }
        return *this;
    }
    bool CParaTemplateParas::operator==(const CParaTemplateParas &other) const
    {
        return type == other.type &&
               circleParas == other.circleParas;
    }
}
namespace BS_2DVision_Template
{
    using namespace BS_QT_Ui;
    CParaTemplateMarkParaWidget::CParaTemplateMarkParaWidget(QWidget *p)
        : IMarkTemplateParaWidget(p)
    {
        initLayout();
    }

    void CParaTemplateMarkParaWidget::attachMarkPara(QVariant *pMarkPara)
    {
        if (!pMarkPara->isValid() || !pMarkPara->canConvert<BS_2DVision_Template::CParaTemplateParas>())
        {
            *pMarkPara = CParaTemplateParas();
        }

        IMarkTemplateParaWidget::attachMarkPara(pMarkPara);

        m_paras = (BS_2DVision_Template::CParaTemplateParas *)(pMarkPara->data());
        assert(m_paras != nullptr);
        m_pCircleWidget->attachMarkPara(&m_paras->circleParas);
        m_subdivisionCheckBox[m_paras->type]->setChecked(true);
    }

    void CParaTemplateMarkParaWidget::initLayout()
    {
        m_pStackedWidget = new CCustomStackedWidget(this);
        m_pCircleWidget = new CCircleTemplateMarkParaWidget(this);
        m_pStackedWidget->addWidget(m_pCircleWidget);

        auto pCircle = new QCheckBox("◯", this);
        pCircle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pCircle->setAutoExclusive(true);
        connect(pCircle, &QCheckBox::toggled, this, [this](bool p_on)
                {
                    if (p_on)
                    {
                        m_pStackedWidget->setCurrentWidget(m_pCircleWidget);
                        m_paras->type = CParaTemplateDef::Circle;
                    } });

        auto pCrossLine = new QCheckBox("✚", this);
        pCrossLine->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pCrossLine->setAutoExclusive(true);
        // connect(m_pCircle, &QCheckBox::toggled, this, [this](bool p_on)
        //         { if (p_on) m_pStackedWidget->setCurrentWidget(m_pCircleWidget); });

        auto pLLine = new QCheckBox("∟", this);
        pLLine->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pLLine->setAutoExclusive(true);
        // connect(m_pCircle, &QCheckBox::toggled, this, [this](bool p_on)
        //         { if (p_on) m_pStackedWidget->setCurrentWidget(m_pCircleWidget); });

        QGridLayout *pLayout = new QGridLayout(this);
        pLayout->addWidget(pCircle, 0, 0, 1, 1);
        pLayout->addWidget(pCrossLine, 0, 1, 1, 1);
        pLayout->addWidget(pLLine, 0, 2, 1, 1);

        pLayout->setAlignment(pCircle, Qt::AlignCenter);
        pLayout->setAlignment(pCrossLine, Qt::AlignCenter);
        pLayout->setAlignment(pLLine, Qt::AlignCenter);

        pLayout->addWidget(m_pStackedWidget, 1, 0, 1, 3);
        pLayout->setMargin(0);
        pLayout->setSpacing(3);

        pCrossLine->setEnabled(false);
        pLLine->setEnabled(false);

        m_subdivisionCheckBox[CParaTemplateDef::Circle] = pCircle;
        m_subdivisionCheckBox[CParaTemplateDef::LLine] = pLLine;
        m_subdivisionCheckBox[CParaTemplateDef::CrossLine] = pCrossLine;
    }
}