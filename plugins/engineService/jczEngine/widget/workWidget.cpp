#include "workWidget.h"
#include "pluginCoreDef.h"
#include "service/serviceManager.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QIcon>
CJczWorkWidget *jczWorkWindow()
{
    return qobject_cast<CJczWorkWidget *>(BS_QT_Service::CServiceManager::GetService(cStdWorkWidget));
}

CJczWorkWidget::CJczWorkWidget(QWidget *p_pParent) : CMeasureWorkWidget(p_pParent)
{
}

CJczWorkWidget::~CJczWorkWidget()
{
}

void CJczWorkWidget::init()
{
    CMeasureWorkWidget::init();
    m_pMarkWidget = new CJczMarkWidget(this);
    m_pMarkWidget->hide();
    m_pMarkWidgetBtn = getToolBtn(tr("Mark Ctrl"), QIcon(cWorkWidgetMarkCtrlIcon));
    m_pMarkWidgetBtn->setCheckable(true);
    connect(m_pMarkWidgetBtn, &QToolButton::toggled, m_pMarkWidget, &CJczMarkWidget::setVisible);
}

void CJczWorkWidget::initLayout()
{
    initToolBar();
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pImageWidget);
    pHLayout->addWidget(m_pMarkWidget);
    pHLayout->setSpacing(0);
    pHLayout->setMargin(0);
    QVBoxLayout *pLauout = new QVBoxLayout(this);
    pLauout->addWidget(m_pToolBar);
    pLauout->addLayout(pHLayout);
    pLauout->setSpacing(0);
    pLauout->setMargin(0);
}

void CJczWorkWidget::initToolBar()
{
    assert(m_pToolBar == nullptr);
    m_pToolBar = new QWidget(this);
    m_pToolBar->setObjectName(cStdWorkWidgetToolBar);
    QHBoxLayout *pToolBarLayout = new QHBoxLayout(m_pToolBar);
    pToolBarLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pToolBarLayout->addWidget(m_pSaveCurrentImagePb);
    pToolBarLayout->addWidget(m_pMeasureButton);
    pToolBarLayout->addWidget(m_pLinkButton);
    pToolBarLayout->addWidget(m_pMarkWidgetBtn);
    pToolBarLayout->addWidget(m_pAcquireButton);
    pToolBarLayout->addWidget(m_pCameParaPb);
    pToolBarLayout->addStretch();
    pToolBarLayout->setMargin(3);
    pToolBarLayout->setSpacing(5);
}
