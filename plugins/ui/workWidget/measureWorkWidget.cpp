#include "measureWorkWidget.h"
#include "pluginCoreDef.h"

#include "service/serviceManager.h"
#include "message/msgManager.h"

#include <QHBoxLayout>
#include <QToolButton>
CMeasureWorkWidget *measureWorkWindow()
{
    return qobject_cast<CMeasureWorkWidget *>(BS_QT_Service::CServiceManager::GetService(cStdWorkWidget));
}

void CMeasureWorkWidget::init()
{
    CStdWorkWidget::init();
    m_pLinkButton = getToolBtn(tr("Linkage"), QIcon(cWorkWidgetLinkageIcon));
    m_pLinkButton->setCheckable(true);
    connect(m_pLinkButton, &QToolButton::toggled, m_pMeasureImageWidget, &CMeasureImageWidget::setPixelLinkage);

    m_pMeasureButton = getToolBtn(tr("Measure"), QIcon(cWorkWidgetMeasureIcon));
    m_pMeasureButton->setCheckable(true);
    connect(m_pMeasureButton, &QToolButton::toggled, m_pMeasureImageWidget, &CMeasureImageWidget::setMeasure);

    connect(m_pMeasureImageWidget, &CMeasureImageWidget::sigPixelLinkageChanged, this, [this](bool p_link)
    {
        if (p_link)
        {
            m_pMeasureButton->setChecked(false);
            m_pMeasureButton->toggled(false);
        }
    });
    connect(m_pMeasureImageWidget, &CMeasureImageWidget::sigMeasureChanged, this, [this](bool p_measure)
    {
        if (p_measure)
        {
            m_pLinkButton->setChecked(false);
            m_pLinkButton->toggled(false);
        }
    });

    m_pMeasureButton->setEnabled(false);
    m_pLinkButton->setEnabled(false);
    m_pMeasureImageWidget->setEnabled(false);
}

void CMeasureWorkWidget::newImageWidget()
{
    assert(m_pImageWidget == nullptr && m_pMeasureImageWidget == nullptr);

    m_pMeasureImageWidget = new CMeasureImageWidget(this);
    m_pImageWidget = m_pMeasureImageWidget;
    connect(m_pMeasureImageWidget, &CMeasureImageWidget::sigPixelLinkage, this, [this](double p_x, double p_y)
            { Out_Info << tr("Pixel linkage click coordinates: (%1,%2)").arg(p_x).arg(p_y); });
}

void CMeasureWorkWidget::initToolBar()
{
    assert(m_pToolBar == nullptr);
    m_pToolBar = new QWidget(this);
    m_pToolBar->setObjectName(cStdWorkWidgetToolBar);
    QHBoxLayout *pToolBarLayout = new QHBoxLayout(m_pToolBar);
    pToolBarLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pToolBarLayout->addWidget(m_pSaveCurrentImagePb);
    pToolBarLayout->addWidget(m_pMeasureButton);
    pToolBarLayout->addWidget(m_pLinkButton);
    pToolBarLayout->addWidget(m_pAcquireButton);
    pToolBarLayout->addWidget(m_pCameParaPb);
    pToolBarLayout->addStretch();
    pToolBarLayout->setMargin(3);
    pToolBarLayout->setSpacing(5);
}

void CMeasureWorkWidget::imageValidChanged(bool p_valid)
{
    CStdWorkWidget::imageValidChanged(p_valid);
    if (m_pMeasureButton->isEnabled() != p_valid)
    {
        m_pMeasureButton->setEnabled(p_valid);
        m_pLinkButton->setEnabled(p_valid);
    }

    if (m_pMeasureImageWidget->isEnabled() != p_valid)
    {
        m_pMeasureImageWidget->setEnabled(p_valid);
    }
}
