#include "featureIconButton.h"
#include "pluginCoreDef.h"
#include "ui/editLabel/editLabel.h"

#include "Ui/uiHelper.h"

#include <QToolButton>
#include <QVBoxLayout>
CFeatureIconButton::CFeatureIconButton(int *p_id, QWidget *parent) : CIconTextButton(p_id, parent)
{
    auto getToolButton = [](QString icon)
    {
        auto pTool = new QToolButton;
        pTool->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        pTool->setToolButtonStyle(Qt::ToolButtonIconOnly);
        pTool->setIconSize(QSize(24, 24));
        pTool->setIcon(QIcon(icon));
        pTool->setStyleSheet("QToolButton{background:transparent;border:none;}"
                             "QToolButton:hover:!checked{background-color:rgb(133,183,222);}");
        return pTool;
    };
    m_pDel = getToolButton(cProjectEditDelIcon);
    m_pDel->setFixedWidth(25);
    connect(m_pDel, &QToolButton::clicked, this, [this]()
            { emit sigDelFeature(id()); });
    m_pEdit = getToolButton(cProjectEditEditIcon);
    m_pEdit->setFixedWidth(25);
    connect(m_pEdit, &QToolButton::clicked, this, [this]()
            { emit sigEditFeature(id()); });
    initLyout();
}

CFeatureIconButton::~CFeatureIconButton()
{
}

void CFeatureIconButton::setCanEdit(bool p_edit)
{
    m_pDel->setVisible(p_edit);
    m_pEdit->setVisible(p_edit);
}

void CFeatureIconButton::initLyout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(this->layout());
    QHBoxLayout *pLayout = new QHBoxLayout;
    {
        {
            QVBoxLayout *pLayoutIcon = new QVBoxLayout;
            {
                QHBoxLayout *pLableLayout = new QHBoxLayout;
                pLableLayout->addStretch();
                pLableLayout->addWidget(m_pIconLabel);
                pLableLayout->addStretch();
                pLableLayout->setSpacing(0);
                pLableLayout->setMargin(0);
                pLayoutIcon->addLayout(pLableLayout);
            }
            pLayoutIcon->addWidget(m_pTitleLabel);
            pLayoutIcon->setSpacing(0);
            pLayoutIcon->setMargin(0);
            pLayout->addLayout(pLayoutIcon);
        }

        {
            QVBoxLayout *pLayoutTool = new QVBoxLayout;
            pLayoutTool->addWidget(m_pDel);
            pLayoutTool->addWidget(m_pEdit);
            pLayoutTool->setSpacing(1);
            pLayoutTool->setMargin(0);
            pLayout->addLayout(pLayoutTool);
        }
    }
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    this->setLayout(pLayout);
    updateWidth();
}

void CFeatureIconButton::updateWidth()
{
    this->setFixedWidth(qMax(m_pIconLabel->width(), m_pTitleLabel->fontMetrics().width(m_pTitleLabel->text())) + 10 + qMax(m_pDel->width(), m_pEdit->width()));
}
