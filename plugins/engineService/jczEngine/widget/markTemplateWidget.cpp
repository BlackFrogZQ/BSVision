#include "markTemplateWidget.h"
#include "template/templateHelper.h"
#include "Ui/widget/comboBox.h"
#include "Ui/widget/stackedWidget.h"

#include <QMetaEnum>
#include <QStackedWidget>
#include <QGridLayout>

using namespace BS_QT_Ui;
using namespace BS_2DVision_Template;
CMarkTemplateWidget::CMarkTemplateWidget(QWidget *p)
    : QFrame(p)
{
    initLayout();
}

CMarkTemplateWidget::~CMarkTemplateWidget()
{
}

void CMarkTemplateWidget::attachMark(CMarkPoint *pMark)
{
    // assert(pMark != nullptr && pMark->templateParas.keys() == m_pMakeParaWidgets.keys() && "attachMark 检查");
    m_pMark = nullptr;
    for (auto key : m_pMakeParaWidgets.keys())
    {
        m_pMakeParaWidgets[key]->attachMarkPara(&pMark->templateParas[key]);
    }
    m_pTemplateComboBox->setCurrentIndex(m_comboBoxIdMap[pMark->templateType]);
    m_pMark = pMark;
}

QRectF CMarkTemplateWidget::roi() const
{
    return m_pMark->roi;
}

bool CMarkTemplateWidget::usedRoi() const
{
    return m_pMark->usedRoi;
}

void CMarkTemplateWidget::setRoi(QRectF p_roi)
{
    m_pMark->roi = p_roi;
}

void CMarkTemplateWidget::setUsedRoi(bool p_usedRoi)
{
    m_pMark->usedRoi = p_usedRoi;
}

void CMarkTemplateWidget::initLayout()
{
    m_pTemplateComboBox = new CCustomComboBox(this);
    m_pTemplateComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto metaEnum = QMetaEnum::fromType<CTemplateDef::CTemplateType>();

    m_pParaStackedWidget = new CCustomStackedWidget(this);

    for (auto key : templateHelper()->m_templateMap.keys())
    {
        auto pTemplate = templateHelper()->m_templateMap[key];
        assert(pTemplate != nullptr && "模板指针检查");
        m_comboBoxIdMap[key] = m_pTemplateComboBox->count();
        m_pTemplateComboBox->addItem(pTemplate->typeString());

        m_pMakeParaWidgets[key] = pTemplate->newMarkParaWidget(this);
        m_pParaStackedWidget->addWidget(m_pMakeParaWidgets[key]);
    }
    connect(m_pTemplateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int p_id)
            {
                m_pParaStackedWidget->setCurrentIndex(p_id);
                if(m_pMark != nullptr)
                {
                    m_pMark->templateType = m_comboBoxIdMap.key(p_id);
                } }, Qt::QueuedConnection);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(m_pTemplateComboBox, 0, 1, 1, 2);
    pLayout->addWidget(m_pParaStackedWidget, 1, 0, 1, 4);
    pLayout->setSpacing(3);
    pLayout->setMargin(0);
}
